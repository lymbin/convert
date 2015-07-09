/*
 * Track.cpp
 *
 *  Created on: 05.03.2015
 *      Author: dmitry
 */

#include "Track.h"

// Конструктор трека. Содержит полный путь до файла, имя файла, формат и полный путь до папки с файлом.
cTrack::cTrack(std::string asFullUri, std::string asFileName, std::string asFormat, std::string asFolderUri, int adID) : iObject (asFullUri)
{
	static unsigned int index = 0;		//статичный индекс для формирования внутреннего ID

	msFullUri = asFullUri;		// Полный путь до файла.
	msFolderUri = asFolderUri;	// Путь до папки с файлом.

	if(!asFileName.empty())
		msFileName = asFileName;	// Имя файла может отсутствовать.

	meFormat = eFormat_Unknown;

	// Добавим формат файла.
	if(!asFormat.empty())
	{
		if((asFormat == "mp3") || (asFormat == "MP3"))
		{
			meFormat = eFormat_MP3;
		}
		else if((asFormat == "wav") || (asFormat == "WAV"))
		{
			meFormat = eFormat_WAV;
		}
		else if((asFormat == "ogg") || (asFormat == "OGG"))
		{
			meFormat = eFormat_OGG;
		}
	}

	// Укажем индекс трека для лучшего поиска.
	if(adID != -1)
	{
		mdID = (unsigned int)adID;	// Индекс задан в качестве параметра конструктора.
	}
	else
	{
		mdID = index;
		index++;		// Увеличим статичный индекс.
	}
}

// Деструктор.
cTrack::~cTrack()
{

}

// Метод построения.
void cTrack::Create()
{

}

// Метод уничтожения.
void cTrack::Destroy()
{

}

// Получаем полный путь.
std::string cTrack::GetFullUri()
{
	return msFullUri;
}

// Получаем имя файла.
std::string cTrack::GetFileName()
{
	return msFileName;
}

// Получаем путь до папки с файлом.
std::string cTrack::GetFolderUri()
{
	return msFolderUri;
}

// Получаем формат.
eFormat cTrack::GetFormat()
{
	return meFormat;
}
unsigned int cTrack::GetID()
{
	return mdID;
}
