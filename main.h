/*
 * main.h
 *
 *  Created on: 03.03.2015
 *      Author: dmitry

 	Copyright 2016 Dmitry Kilchanov <dmitrykilchanov@gmail.com>

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <map>
#include <vector>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <gdk/gdk.h>
#include <gdk/gdkx.h> //for GDK_WINDOW_XID

#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/app/gstappsrc.h>
#include <gst/video/videooverlay.h>

// Тут задаём номер версии программы.
#define SYS_VERSION "0.13"
#define SYS_BUILD 	"013"

#endif /* MAIN_H_ */
