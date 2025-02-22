## install 
sudo apt-get update
sudo apt-get install \
    gcc \
    libcaca-dev \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libswscale-dev

## run
gcc hello_engr.c -lavformat -lavcodec -lavutil -lswscale -lcaca -o hello_engr.exe && ./hello_engr.exe