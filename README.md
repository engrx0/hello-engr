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



![anon](https://github.com/user-attachments/assets/320cb28f-c790-491a-b5df-8e5d10e1147a)
