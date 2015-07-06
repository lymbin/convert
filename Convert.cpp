/*
 * Convert.cpp
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#include "Convert.h"

// Конструктор. Зададим параметры по умолчанию.
cConvert::cConvert(cMainWin *aMainWin)
{
	mMainWin = aMainWin;	// Установим указатель на главное окно.
	mdQuality = 320;		// Установим стандартное наивысшее качество сжатия.
	meFormat = eFormat_MP3;	// По умолчанию конвертируем в MP3.

	mbIsConvertStarted = false;	// Конвертация не начата.
	mbIsNeedStop = false;		// Не нужна остановка.

	// Обнуляем поля gStreamer элементов.
	mPipeline = 0;
	mFilesrc = 0;
	mFilesink = 0;
	mEncode = 0;
}

// Деструктор.
cConvert::~cConvert()
{
	mbIsNeedStop = true; // Останавливаем конвертацию.

	while(mbIsConvertStarted)	// Теперь ждём остановки конвертации.
	{
		g_usleep(200000);
	}

	// Удаляем Pipeline.
	if(mPipeline)
	{
		gst_element_set_state (mPipeline, GST_STATE_NULL);
		gst_object_unref (mPipeline);
		mPipeline = 0;
	}
}

// Установим качество выходного файла из текста.
void cConvert::SetQuality(std::string asQuality)
{
	std::stringstream asStream;
	asStream << asQuality;
	asStream >> mdQuality;
}

// Установим качество выходного файла из int.ы
void cConvert::SetQuality(int adQuality)
{
	mdQuality = adQuality;
}

// Установка формата выходного файла.
void cConvert::SetFormat(std::string asFormatt)
{
	for(int i = 0; i < eFormat_Unknown; i++)
	{
		if(asFormatt == asFormat[i])
		{
			meFormat = (eFormat)i;
			return;
		}
	}

	meFormat = eFormat_Unknown;
}

// Построение пипелайна.
std::string cConvert::CreatePipelineLine()
{
	std::stringstream asConvertLine;

	// Берём пипелайн из текста.
	asConvertLine << "filesrc name=convert_src ! decodebin ! audioconvert ! ";

	// Добавляем к пипелайну особый энкодера для каждого типа файлов.
	if(meFormat == eFormat_MP3)
	{
		asConvertLine << "lamemp3enc name=convert_format ! xingmux ! id3v2mux";
	}
	else if(meFormat == eFormat_WAV)
	{
		asConvertLine << "wavenc name=convert_format";
	}
	else if(meFormat == eFormat_OGG)
	{
		asConvertLine << "vorbisenc name=convert_format ! oggmux";
	}

	// И закончим элементом выходного файла.
	asConvertLine << " ! filesink name=convert_sink ";

	return asConvertLine.str();	// Выведем строку текста пипелайна.
}

// Получение формата.
eFormat cConvert::GetFormat()
{
	return meFormat;
}

// Главная функция конвертации.
bool cConvert::Convert()
{
	tTracksIt aIt = mMainWin->mTracks.begin();
	std::string asConvertLine = CreatePipelineLine();	// Построим общую пипелайн линию.
	std::stringstream asNewTuneUri;						// Путь до нового файла.

	bool abIsConvertComplete = true;					// Конвертация завершена. Это мы будем выводить в return'е.

	GstMessage *aMsg = 0;	// Сообщение gStreamer'а.
	GstBus *aBus = 0;		// Шина данных пипелайна.
	GError *aError = NULL;	// Указатель на ошибку.

	// Цикл по всем трекам в окне.
	for(; aIt < mMainWin->mTracks.end(); ++aIt)
	{
		// Пока для MP3 и OGG форматов, а также для WAV, но только если текущий формат и формат в который конвертируем не равны WAV одновременно.
		if(meFormat == eFormat_Unknown)
			continue;
		if(meFormat == eFormat_WAV && (*aIt)->mTrack->GetFormat() == eFormat_WAV)
		{
			continue;
		}

		mPipeline = gst_parse_launch (asConvertLine.c_str(), &aError);	// Запускаем пипелайн из строки.
		if (!mPipeline) {
		    g_print ("Parse error: %s\n", aError->message);
		    abIsConvertComplete = false;
		    continue;
		}

		// Устанавливаем ссылку на источник файла.
		mFilesrc = gst_bin_get_by_name (GST_BIN (mPipeline), "convert_src");
		// Установим местоположение источника файла.
		g_object_set (mFilesrc, "location", (*aIt)->mTrack->GetFullUri().c_str(), NULL);

		// Устанавливаем ссылку на энкодер.
		mEncode = gst_bin_get_by_name (GST_BIN (mPipeline), "convert_format");

		// Формируем полный путь до файла без формата. Не забываем зачистить после использования.
		asNewTuneUri << (*aIt)->mTrack->GetFolderUri() << "/" << mMainWin->FindLastNCopy((*aIt)->mTrack->GetFileName(), ".");

		// Добавляем формат к файлу.
		if(meFormat == eFormat_MP3)
		{
			g_object_set (mEncode, "bitrate", mdQuality, NULL);
			asNewTuneUri << ".mp3";
		}
		else if(meFormat == eFormat_WAV)
		{
			asNewTuneUri << ".wav";
		}
		else if(meFormat == eFormat_OGG)
		{
			if(mdQuality*1000 > 250000)	// Максимальное качество OGG - 250 кбит.
				g_object_set (mEncode, "bitrate", 250000, NULL);
			else
				g_object_set (mEncode, "bitrate", mdQuality*1000, NULL);

			asNewTuneUri << ".ogg";
		}

		// Теперь полученный путь используем для настройки получателя.
		std::cout << asNewTuneUri.str() << std::endl;
		mFilesink = gst_bin_get_by_name (GST_BIN (mPipeline), "convert_sink");
		g_object_set (mFilesink, "location", asNewTuneUri.str().c_str(), NULL);


		// Установим пипелайн в состояние Play.
		gst_element_set_state (mPipeline, GST_STATE_PLAYING);

		// Получим шину данных пипелайна, для получения сообщений.
		aBus = gst_element_get_bus (mPipeline);

		// Ждём сообщений с пипелайна.
		// Конечно тут можно распараллелить программу, для того чтобы каждый файл конвертировался в своём потоке - это ускорит программу.
		// Принимаем сообщения только EOS - конец потока и ERROR - ошибка.
		aMsg = gst_bus_poll (aBus, (GstMessageType) (GST_MESSAGE_EOS | GST_MESSAGE_ERROR), -1);

		// Сообщение получено. Рассмотрим его.
		switch (GST_MESSAGE_TYPE (aMsg)) {
			case GST_MESSAGE_EOS: {
				g_print ("EOS\n");	// Если конец потока, то выходим.
				break;
			}
			case GST_MESSAGE_ERROR: {
				// Если какая-либо ошибка, то выведем её в дебаг окно.
				GError *err = NULL; /* error to show to users                 */
				gchar *dbg = NULL;  /* additional debug string for developers */

				gst_message_parse_error (aMsg, &err, &dbg);	// Парсим ошибку в GError и gchar.
				if (err) {
					g_printerr ("ERROR: %s\n", err->message);	// Покажем в консоли сообщение об ошибке ...
					g_error_free (err);
				}
				if (dbg) {
					g_printerr ("[Debug details: %s\n", dbg);	// ... с подробными деталями.
					g_free (dbg);
				}
			}
			default:
				// По умолчанию - не известный тип сообщения, который мы не обрабатываем.
				g_printerr ("Unexpected message of type %d", GST_MESSAGE_TYPE (aMsg));
				break;
		}

		gst_message_unref (aMsg);	// Освобождаем память.
		gst_element_set_state (mPipeline, GST_STATE_NULL);	// Останавливаем пипелайн.
		gst_object_unref (mPipeline);	// Освобождаем память пипелайна.
		gst_object_unref (aBus);		// Освобождаем память шины данных.

		// Обнуляем пипелайн и шину.
		mPipeline = 0;
		aBus = 0;

		// Зачищаем строковый поток.
		asNewTuneUri.str(std::string());
		asNewTuneUri.clear();
	}

	return abIsConvertComplete;
}
