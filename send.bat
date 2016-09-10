@rem %FFMPEG_DIR%\ffmpeg -i c:\samples\sample.mp4 -r 15 -acodec copy -g 10 -vcodec libx264 -s 854x480 -metadata duration=24.00 -metadata streamName=mov000448 -f flv tcp://127.0.0.1:6666?pkt_size=1400

%FFMPEG_DIR%\ffmpeg -y -f vfwcap -r 15 -i 0 -g 10 -vcodec libx264 -s 320x240 -metadata duration=24.00 -metadata streamName=mov000448 -f flv rtmp://127.0.0.1/live/mystream

