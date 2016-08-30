/*
 * Track.h
 *
 *  Created on: 05.03.2015
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

#ifndef TRACK_H_
#define TRACK_H_

#include "main.h"
#include "Object.h"

// Перечисление всех известных форматов трека.
enum eFormat
{
	eFormat_MP3 = 0,
	eFormat_WAV,
	eFormat_OGG,
	eFormat_Unknown,
};

// Форматы в виде текста.
static std::string asFormat[] =
{
	"MP3", "WAV", "OGG"
};

// Класс трека.
class cTrack: public iObject{

	eFormat meFormat;
	unsigned int mdID;
	std::string msFullUri;
	std::string msFileName;
	std::string msFolderUri;

public:
	// Конструктор трека. Содержит полный путь до файла, имя файла, формат и полный путь до папки с файлом.
	cTrack(std::string asFullUri, std::string asFileName = "", std::string asFormat = "", std::string asFolderUri = "", int adID = -1);

	~cTrack();	// Деструктор.

	virtual void Create();		// Метод построения.
	virtual void Destroy();		// Метод уничтожения.

	std::string GetFullUri();	// Получаем полный путь.
	std::string GetFileName();	// Получаем имя файла.
	std::string GetFolderUri();	// Получаем путь до папки с файлом.
	eFormat GetFormat();		// Получаем формат.
	unsigned int GetID();		// Получаем ID трека.
};


#endif /* TRACK_H_ */
