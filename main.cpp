/*
 * main.cpp
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#include "main.h"

#include "MainWin.h"

#define SYS_WIN_TITLE "Аудио конвертер"


int main(int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	gst_init (&argc, &argv);

	cMainWin *aMainWin = new cMainWin(SYS_WIN_TITLE);
	aMainWin->Create();

	gtk_main();

	std::cout << "Exited!" << std::endl;
	delete aMainWin;

	return 0;
}


