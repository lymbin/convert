all: convert

clean:
	rm ./Convert.o ./MainWin.o ./Track.o ./main.o ./convert
	
convert: ./Convert.o ./MainWin.o ./Track.o ./main.o
	g++ `pkg-config --libs gtk+-3.0 gsoap++ libxml-2.0 openssl x11 gstreamer-1.0 gstreamer-video-1.0 gstreamer-app-1.0 gstreamer-audio-1.0` -o "convert"  ./Convert.o ./MainWin.o ./Track.o ./main.o
	
Convert.o:
	g++ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 gstreamer-1.0  gsoap++ libxml-2.0` -pthread -MMD -MP -MF"Convert.d" -MT"Convert.d" -o "Convert.o" "./Convert.cpp"
	
MainWin.o:
	g++ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 gstreamer-1.0  gsoap++ libxml-2.0` -pthread -MMD -MP -MF"MainWin.d" -MT"MainWin.d" -o "MainWin.o" "./MainWin.cpp"

Track.o:
	g++ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 gstreamer-1.0  gsoap++ libxml-2.0` -pthread -MMD -MP -MF"Track.d" -MT"Track.d" -o "Track.o" "./Track.cpp"

main.o:
	g++ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 gstreamer-1.0  gsoap++ libxml-2.0` -pthread -MMD -MP -MF"main.d" -MT"main.d" -o "main.o" "./main.cpp"
