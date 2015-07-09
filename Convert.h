/*
 * Convert.h
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#ifndef CONVERT_H_
#define CONVERT_H_

#include "main.h"
#include "MainWin.h"
#include "Track.h"

class cMainWin;

// Все доступные качества конвертируемого файла в MP3.
enum eMP3Quality{
	eMP3Quality_low = 64,
	eMP3Quality_mid = 128,
	eMP3Quality_good = 192,
	eMP3Quality_best = 320,
	eMP3Quality_count = 4
};

// Все доступные качества конвертируемого файла в OGG.
enum eOGGQuality{
	eOGGQuality_low = 64,
	eOGGQuality_mid = 128,
	eOGGQuality_good = 192,
	eOGGQuality_best = 250,
	eOGGQuality_count = 4
};

// Все доступные качества конвертируемого файла в MP3 в виде текста.
static std::string asMP3Quality[] =
{
	"64", "128", "192", "320"
};

// Все доступные качества конвертируемого файла в OGG в виде текста.
static std::string asOGGQuality[] =
{
	"64", "128", "192", "250"
};

// Класс конвертации файлов.
class cConvert{

	int mdQuality;			// Качество выходного файла.
	eFormat meFormat;		// Формат файла на выходе.

	cMainWin *mMainWin;		// Указатель на класс главного окна.

	// Элементы gStreamer.
	GstElement 	*mPipeline,
				*mFilesrc,	// Файловый источник.
				*mEncode,	// Энкодер.
				*mFilesink;	// Получатель файла.

	bool mbIsConvertStarted;	// Начата ли конвертация.
	bool mbIsNeedStop;			// Нужно ли остановить конвертацию.

public:
	cConvert(cMainWin *aMainWin);	// Конструктор.
	~cConvert();					// Деструктор.

	void SetQuality(std::string asQuality);	// Установим качество выходного файла из текста.
	void SetQuality(int adQuality);			// Установим качество выходного файла из int.

	void SetFormat(std::string asFormat);	// Установка формата выходного файла.

	std::string CreatePipelineLine();		// Построение пипелайна.

	eFormat GetFormat();	// Получение формата.


	bool Convert();	// Главная функция конвертации.
};

#endif /* CONVERT_H_ */
