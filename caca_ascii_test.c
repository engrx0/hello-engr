#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include <caca.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#define BORDER_DELAY 10000
#define TEXT_DELAY   100000

// #define VIDEO_FILE "media/input.mov" 
#define VIDEO_FILE "media/input_square.mov" 

static void print_border(caca_canvas_t *canvas, caca_display_t *display,
    int xo, int yo, int width, int height);

static void print_text(caca_canvas_t *canvas, caca_display_t *display,
        int xo, int yo, const char *msg);

static void display_video_ascii(caca_canvas_t *canvas, caca_display_t *display,
            const char *filename, int col_start, int row_start, int canvas_width, int canvas_height);

int main() 
{

    avformat_network_init();

    int canvas_width = 80;
    int canvas_height = 30;

    caca_canvas_t *cv = caca_create_canvas(canvas_width, canvas_height);
    if(!cv) 
    {
        fprintf(stderr, "failed to create caca canvas.\n");
        return 1;
    }

    caca_display_t *dp = caca_create_display(cv);
    if(!dp)
    {
        fprintf(stderr, "failed to create caca display.\n");
        caca_free_canvas(cv);
        return 1;
    }

    caca_set_display_title(dp, "hello engr with libcaca");
    caca_clear_canvas(cv);

    /* draw main border */
    int outer_border_xo = 0;
    int outer_border_yo = 0;
    print_border(cv, dp, outer_border_xo, outer_border_yo, canvas_width, canvas_height);

    /* draw header border */
    int header_xo       = 10;
    int header_yo       = 2;
    int header_width    = 60;
    int header_height   = 4;
    print_border(cv, dp, header_xo, header_yo, header_width, header_height);

    /* draw ascii video border */
    int ascii_xo = 5;
    int ascii_yo = 8;
    int ascii_width = 70;
    int ascii_height = 20;
    print_border(cv, dp, ascii_xo, ascii_yo, ascii_width, ascii_height);

    /* print header message */
    print_text(cv, dp, header_xo + 1, header_yo + 1, "hello engr");

    // Display video as ASCII using libcaca’s dither
    // Note that we pass the dimensions for the entire canvas, but you can
    // adapt the code if you want to “blit” into a smaller subregion instead.
    display_video_ascii(cv, dp, "media/input_square.mov", ascii_xo, ascii_yo, ascii_width, ascii_height);

    /* blok until key press */
    caca_event_t ev;
    while(1)
    {
        if(caca_get_event(dp, CACA_EVENT_KEY_PRESS | CACA_EVENT_QUIT, &ev, -1))
        {
            if(caca_get_event_type(&ev) == CACA_EVENT_QUIT ||
               caca_get_event_type(&ev) == CACA_EVENT_KEY_PRESS)
                break;
        }
    }

    caca_free_display(dp);
    caca_free_canvas(cv);
    avformat_network_deinit();

    printf("\033[1;31mhello engr\033[0m\n");
    return 0;
}

static void print_border(caca_canvas_t *canvas, caca_display_t *display,
    int xo, int yo, int width, int height)
{
    int xf = xo + width;
    int yf = yo + height;

    // top-left corner
    caca_put_char(canvas, xo, yo, '+');
    caca_refresh_display(display);
    usleep(BORDER_DELAY);

    // top horizontal line
    for(int col = xo + 1; col < xf; col++)
    {
        caca_put_char(canvas, col, yo, '-');
        caca_refresh_display(display);
        usleep(BORDER_DELAY);
    }

    // top-right corner
    caca_put_char(canvas, xf, yo, '+');
    caca_refresh_display(display);
    usleep(BORDER_DELAY);

    // right vertical line
    for(int row = yo + 1; row < yf; row++)
    {
        caca_put_char(canvas, xf, row, '|');
        caca_refresh_display(display);
        usleep(BORDER_DELAY);
    }

    // bottom-right corner
    caca_put_char(canvas, xf, yf, '+');
    caca_refresh_display(display);
    usleep(BORDER_DELAY);

    // bottom horizontal line
    for(int col = xf - 1; col > xo; col--)
    {
        caca_put_char(canvas, col, yf, '-');
        caca_refresh_display(display);
        usleep(BORDER_DELAY);
    }

    // bottom-left corner
    caca_put_char(canvas, xo, yf, '+');
    caca_refresh_display(display);
    usleep(BORDER_DELAY);

    // left vertical line
    for(int row = yf - 1; row > yo; row--)
    {
        caca_put_char(canvas, xo, row, '|');
        caca_refresh_display(display);
        usleep(BORDER_DELAY);
    }
}


static void print_text(caca_canvas_t *canvas, caca_display_t *display, int xo, int yo, const char *msg)
{
    int i = 0;
    while(msg[i] != '\0')
    {
        // toggle bold
        if(i % 2 == 0) 
        {
            caca_set_color_ansi(canvas, CACA_LIGHTGREEN, CACA_BLACK);
            caca_put_char(canvas, xo + i, yo, msg[i]);
        }
        else 
        {
            caca_set_color_ansi(canvas, CACA_WHITE, CACA_BLACK);
            caca_put_char(canvas, xo + i, yo, msg[i]);
        }

        caca_refresh_display(display);
        usleep(TEXT_DELAY);
        i++;
    }

    caca_set_color_ansi(canvas, CACA_WHITE, CACA_BLACK);
}

/** decodes video frames (using FFmpeg) and uses libcaca’s dithering to
 *  convert and display each frame to ASCII in the terminal  */
static void display_video_ascii(caca_canvas_t *canvas, caca_display_t *display,
    const char *filename, int col_start, int row_start, int canvas_width, int canvas_height)
{
    AVFormatContext *fmt_ctx = NULL;

    if(avformat_open_input(&fmt_ctx, filename, NULL, NULL) != 0)
    {
        caca_put_str(canvas, 0, 0, "Could not open video file");
        caca_refresh_display(display);
        return;
    }

    if(avformat_find_stream_info(fmt_ctx, NULL) < 0)
    {
        caca_put_str(canvas, 0, 0, "Could not find stream info");
        caca_refresh_display(display);
        avformat_close_input(&fmt_ctx);
        return;
    }

    int video_stream_index = -1;
    for(int i = 0; i < fmt_ctx->nb_streams; i++)
    {
        if(fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
            break;
        }
    }

    if(video_stream_index < 0)
    {
        caca_put_str(canvas, 0, 0, "No video stream found");
        caca_refresh_display(display);
        avformat_close_input(&fmt_ctx);
        return;
    }

    AVCodecParameters *codec_params = fmt_ctx->streams[video_stream_index]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codec_params->codec_id);
    if(!codec)
    {
        caca_put_str(canvas, 0, 0, "Unsupported codec");
        caca_refresh_display(display);
        avformat_close_input(&fmt_ctx);
        return;
    }

    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codec_ctx, codec_params);
    if(avcodec_open2(codec_ctx, codec, NULL) < 0)
    {
        caca_put_str(canvas, 0, 0, "Could not open codec");
        caca_refresh_display(display);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return;
    }

    // prepare a scaling context to convert frames to a format libcaca supports well (e.g. RGBA)
    struct SwsContext *sws_ctx = sws_getContext(codec_ctx->width, codec_ctx->height,
                        codec_ctx->pix_fmt,
                        canvas_width, canvas_height,
                        AV_PIX_FMT_RGBA,
                        SWS_BICUBIC, NULL, NULL, NULL);
    if(!sws_ctx)
    {
        caca_put_str(canvas, 0, 0, "Could not initialize sws context");
        caca_refresh_display(display);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        return;
    }

    // allocate frames
    AVFrame *frame = av_frame_alloc();
    AVFrame *frame_rgba = av_frame_alloc();

    // buffer to hold converted frames (RGBA)
    int num_bytes = av_image_get_buffer_size(AV_PIX_FMT_RGBA, canvas_width, canvas_height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(num_bytes);
    av_image_fill_arrays(frame_rgba->data, frame_rgba->linesize, buffer, AV_PIX_FMT_RGBA,
        canvas_width, canvas_height, 1);

    // create a libcaca dither. specify 8 bits per component in RGBA.
    caca_dither_t *dither = caca_create_dither(
        32,                  // bits per pixel
        canvas_width,        // width
        canvas_height,       // height
        frame_rgba->linesize[0], // pitch (bytes per line)
        0x000000ff,          // mask for red
        0x0000ff00,          // mask for green
        0x00ff0000,          // mask for blue
        0xff000000           // mask for alpha
        );

    AVPacket packet;
    while(av_read_frame(fmt_ctx, &packet) >= 0)
    {
        if(packet.stream_index == video_stream_index)
        {
            if(avcodec_send_packet(codec_ctx, &packet) == 0)
            {
                while(avcodec_receive_frame(codec_ctx, frame) == 0)
                {
                    // convert the original frame to RGBA
                    sws_scale(sws_ctx,
                        (const uint8_t * const *)frame->data,
                        frame->linesize,
                        0,
                        codec_ctx->height,
                        frame_rgba->data,
                        frame_rgba->linesize);

                    // use libcaca to dither (convert) the RGBA image into ASCII
                    // and draw onto the canvas.
                    caca_dither_bitmap(canvas,
                            col_start,  // left position in the canvas
                            row_start,  // top position in the canvas (offset to show below border?)
                            canvas_width,
                            canvas_height,
                            dither,
                            frame_rgba->data[0]);

                    caca_refresh_display(display);
                    usleep(1000000 / 24); // ~24 fps

                    av_frame_unref(frame);
                }
            }
        }
        av_packet_unref(&packet);
    }

    // cleanup
    caca_free_dither(dither);
    av_free(buffer);
    av_frame_free(&frame_rgba);
    av_frame_free(&frame);
    sws_freeContext(sws_ctx);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
}
