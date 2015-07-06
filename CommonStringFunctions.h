/*
 * CommonStringFunctions.h
 *
 *  Created on: 10.11.2014
 *      Author: dmitry
 */

#ifndef COMMONSTRINGFUNCTIONS_H_
#define COMMONSTRINGFUNCTIONS_H_

// Класс общих функций для работы со строками.
class cCommonStringFunctions
{
public:
	// Метод поиска и замены определённого куска текста asFindText на другой asReplaceText в asText.
	std::string FindNReplace(std::string &asText, std::string asFindText, std::string asReplaceText)
	{
		if(asFindText == "")	// Только для не нулевого текста для поиска.
			return asText;

		unsigned int Pos = asText.find(asFindText);		// Получаем позицию начала куска текста, который мы хотим заменить.
		unsigned int ReplaceSize = asFindText.size();	// Получаем его размер.

		if(!ReplaceSize)	// Если размер не получен, то установим его равным 1.
			ReplaceSize = 1;

		// Далее цикл будет идти до тех пор, пока позиция не достигнет конца основного текста или не будет достигнут конец куска текста, который нужно заменить.
		while(Pos < asText.size() && Pos != std::string::npos)
		{
			asText.replace(Pos, ReplaceSize, asReplaceText);	// Заменяем символы в позиции Pos основного текста символами из текста asReplaceText.
			Pos = asText.find(asFindText);						// Снова получаем позицию куска текста, который мы хотим заменить.
		}

		return asText;	// Выводим полученный текст.
	}

	// Метод поиска текста после определённого куска текста asFindText в основном asText.
	std::string FindLastNCopyAfter(std::string asText, std::string asFindText)
	{
		if(asFindText == "")	 // Только для не нулевого текста для поиска.
			return "";

		unsigned int Pos = asText.find_last_of(asFindText);	// Получаем позицию начала куска текста с конца, который мы хотим заменить.

		std::string asCopyText;	// Тот текст который хотим получить.
		std::copy(asText.begin() + Pos, asText.end(), std::back_inserter(asCopyText));	// Копируем в него все символы, начиная с Pos и до конца текста.

		return asCopyText;	// Выводим полученный текст.
	}

	// Метод поиска текста до определённого куска текста asFindText в основном asText.
	std::string FindLastNCopy(std::string asText, std::string asFindText)
	{
		if(asFindText == "")	 // Только для не нулевого текста для поиска.
			return "";

		unsigned int Pos = asText.find_last_of(asFindText);	// Получаем позицию начала куска текста с конца, который мы хотим заменить.

		std::string asCopyText;	// Тот текст который хотим получить.
		std::copy(asText.begin(), asText.begin() + Pos, std::back_inserter(asCopyText));	// Копируем в него все символы, начиная с begin и до Pos текста.

		return asCopyText;	// Выводим полученный текст.
	}

	// Метод поиска, который выводит содержится ли кусок текста в тексте asText.
	bool Find(std::string asText, std::string asFindText)
	{
		if(asFindText == "")	// Только для не нулевого текста для поиска.
			return false;

		unsigned int Pos = asText.find(asFindText);		// Получаем позицию начала куска текста, который мы хотим ищем.

		// Если нашли, то выводим true.
		if(Pos < asText.size() && Pos != std::string::npos)
		{
			return true;
		}

		return false;
	}


};


#endif /* COMMONSTRINGFUNCTIONS_H_ */
