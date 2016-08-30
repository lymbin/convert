/*
 * main.cpp
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


