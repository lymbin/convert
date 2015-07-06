/*
 * Track.h
 *
 *  Created on: 05.03.2015
 *      Author: dmitry
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
	std::string msFullUri;
	std::string msFileName;
	std::string msFolderUri;

public:
	// Конструктор трека. Содержит полный путь до файла, имя файла, формат и полный путь до папки с файлом.
	cTrack(std::string asFullUri, std::string asFileName = "", std::string asFormat = "", std::string asFolderUri = "");

	~cTrack();	// Деструктор.

	virtual void Create();		// Метод построения.
	virtual void Destroy();		// Метод уничтожения.

	std::string GetFullUri();	// Получаем полный путь.
	std::string GetFileName();	// Получаем имя файла.
	std::string GetFolderUri();	// Получаем путь до папки с файлом.
	eFormat GetFormat();		// Получаем формат.
};


#endif /* TRACK_H_ */
