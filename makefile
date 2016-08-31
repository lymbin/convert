#	Copyright 2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com>
#
#	Licensed under the Apache License, Version 2.0 (the "License");
#	you may not use this file except in compliance with the License.
#	You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#	Unless required by applicable law or agreed to in writing, software
#	distributed under the License is distributed on an "AS IS" BASIS,
#	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#	See the License for the specific language governing permissions and
#	limitations under the License.
#

all: convert

clean:
	rm ./Convert.o ./MainWin.o ./Track.o ./main.o ./convert
	
convert: ./Convert.o ./MainWin.o ./Track.o ./main.o
	g++ `pkg-config --libs gtk+-3.0 gstreamer-1.0 gstreamer-audio-1.0` -o "convert"  ./Convert.o ./MainWin.o ./Track.o ./main.o
	rm ./Convert.d ./MainWin.d ./Track.d ./main.d
	
Convert.o:
	g++ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 gstreamer-1.0` -pthread -MMD -MP -MF"Convert.d" -MT"Convert.d" -o "Convert.o" "./Convert.cpp"
	
MainWin.o:
	g++ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 gstreamer-1.0` -pthread -MMD -MP -MF"MainWin.d" -MT"MainWin.d" -o "MainWin.o" "./MainWin.cpp"

Track.o:
	g++ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 gstreamer-1.0` -pthread -MMD -MP -MF"Track.d" -MT"Track.d" -o "Track.o" "./Track.cpp"

main.o:
	g++ -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 gstreamer-1.0` -pthread -MMD -MP -MF"main.d" -MT"main.d" -o "main.o" "./main.cpp"
