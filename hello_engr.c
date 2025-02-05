#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <caca.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#define VIDEO_FILE "media/onyx1.mov" 

void display_video(const char *filename) 
{
    avformat_network_init();
    AVFormatContext *format_ctx = NULL;
    
    // if (avformat_open_input(&format_ctx, filename, NULL, NULL) != 0) 
    if (avformat_open_input(&format_ctx, "/dev/video0", NULL, NULL) != 0) 
    {
        fprintf(stderr, "Could not open video file\n");
        return;
    }

    if (avformat_find_stream_info(format_ctx, NULL) < 0) 
    {
        fprintf(stderr, "Could not find stream info\n");
        avformat_close_input(&format_ctx);
        return;
    }

    int video_stream_index = -1;
    for (unsigned i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }
    if (video_stream_index == -1) {
        fprintf(stderr, "Could not find a video stream\n");
        avformat_close_input(&format_ctx);
        return;
    }

    AVCodecParameters *codec_params = format_ctx->streams[video_stream_index]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    if (!codec) {
        fprintf(stderr, "Unsupported codec\n");
        avformat_close_input(&format_ctx);
        return;
    }

    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, codec_params);
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) 
    {
        fprintf(stderr, "Could not open codec\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&format_ctx);
        return;
    }

    struct SwsContext *sws_ctx = sws_getContext(
        codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
        80, 40, AV_PIX_FMT_RGB24, SWS_BILINEAR, NULL, NULL, NULL
    );

    caca_canvas_t *canvas = caca_create_canvas(80, 40);
    caca_display_t *display = caca_create_display(canvas);
    if (!display) 
    {
        fprintf(stderr, "Could not create caca display\n");
        return;
    }

    AVPacket packet;
    AVFrame *frame = av_frame_alloc();
    AVFrame *rgb_frame = av_frame_alloc();
    int num_bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, 80, 40, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(num_bytes);
    av_image_fill_arrays(rgb_frame->data, rgb_frame->linesize, buffer, AV_PIX_FMT_RGB24, 80, 40, 1);

    while (av_read_frame(format_ctx, &packet) >= 0) 
    {
        if (packet.stream_index == video_stream_index) 
        {
            if (avcodec_send_packet(codec_ctx, &packet) == 0) 
            {
                while (avcodec_receive_frame(codec_ctx, frame) == 0) 
                {
                    sws_scale(sws_ctx, (const uint8_t *const *)frame->data, frame->linesize, 0,
                              codec_ctx->height, rgb_frame->data, rgb_frame->linesize);

                    caca_dither_t *dither = caca_create_dither(24, 80, 40, 80 * 3, 0xFF0000, 0x00FF00, 0x0000FF, 0);
                    caca_dither_bitmap(canvas, 0, 0, 80, 40, dither, rgb_frame->data[0]);
                    caca_refresh_display(display);
                    usleep(1000000 / 24); // Adjust for frame rate
                    caca_free_dither(dither);
                }
            }
        }
        av_packet_unref(&packet);
    }

    av_free(buffer);
    av_frame_free(&frame);
    av_frame_free(&rgb_frame);
    sws_freeContext(sws_ctx);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&format_ctx);
    caca_free_display(display);
}


int main() 
{
    display_video(VIDEO_FILE);
    printf("\033[1;31mhello engr\033[0m\n");
    return 0;
}