for /f "tokens=1 delims=." %%a in ('dir /s /B *.mp3') do ffmpeg -i "%%a.x.mp3" -y -af "volume=4.0" -c:v copy "%%a.mp3"