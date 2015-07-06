/*
 * main.cpp
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#include "main.h"
#include "MainWin.h"

// Задаём название окна программы.
#define SYS_WIN_TITLE "Аудио конвертер"

// Главная функция программы, содержит главный цикл gtk.
// Здесь создаём объект cMainWin и забиваем его виджетами.

int main(int argc, char *argv[])
{
	gtk_init (&argc, &argv);	// Инициализация gtk.
	gst_init (&argc, &argv);	// Инициализация gstreamer.

	// Создаём новый экземпляр класса cMainWin, отвечающий за построение главного окна.
	// В конструкторе обнуляем и инициализируем поля класса.
	cMainWin *aMainWin = new cMainWin(SYS_WIN_TITLE);

	// Основная функция построения класса, в которой происходит заполнение окна виджетами.
	aMainWin->Create();

	gtk_main();		// Основной цикл gtk.

	// После завершения основного цикла gtk выходим из программы.
	std::cout << "Exited!" << std::endl;
	delete aMainWin;

	return 0;
}


