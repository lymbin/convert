/*
 * MainWin.h
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#ifndef MAINWIN_H_
#define MAINWIN_H_

#include "main.h"
#include "Object.h"
#include "Track.h"
#include "Convert.h"
#include "CommonStringFunctions.h"
#include "NotificationSystem.h"

// Объявим классы для того чтобы использовать их далее.
class cConvert;
class cMainWin;
struct cTrackBox;

// Переопредилим вектор боксов с треками и его интератор.
typedef std::vector<cTrackBox *> tTracks;
typedef std::vector<cTrackBox *>::iterator tTracksIt;


// Структура трек бокса с виджетами окна.
struct cTrackBox{
	GtkWidget *mwBox;
	cMainWin *mMainWin;
	cTrack *mTrack;
};

// Структура с виджетами MP3 страницы.
struct cMP3BoxWidgets
{
	GtkWidget *mwQuality;
};

// Структура с виджетами OGG страницы.
struct cOGGBoxWidgets
{
	GtkWidget *mwQuality;
};

// Структура с виджетами WAV страницы.
struct cWAVBoxWidgets
{

};

// Класс основного окна и виджетов окна.
// Также содержит указатель на экземпляр класса cConvert, отвечающего за конвертацию файлов.
// Класс является потомком 3-х классов - iObject - отвечающего за построение и уничтожение экземпляра класса, а также содержащий его глобальное имя,
// 		cCommonStringFunctions - необходимого для выполнения различных операций над строками, и, cNotificationSystem - систему визуального оповещения.
class cMainWin: public iObject, public cCommonStringFunctions, public cNotificationSystem{

	friend cConvert;

	GtkWidget *mwWindow;			// Виджет окна.
	GtkWidget *mwMainBox;			// Главный бокс программы.

	GtkWidget *mwFormat;			// Комбо бокс для выбора формата конверации.
	GtkWidget *mwSpinner;			// Спиннер для визуального отображения процесса конвертации - кушает много ресурсов.
	GtkWidget *mwNewFileNameEntry;	// Поле с новым именем файла - не используется.
	GtkWidget *mwTrackNameBox;		// Бокс со всеми файлами на конвертацию

	std::string msTitle;	// Заголовок окна.

	tTracks mTracks;		// Список всех записей на конвертацию.

	cConvert *mConvert;		// Указатель на экземпляр класса cConvert.

	cMP3BoxWidgets mMP3Widgets;	// Подкласс с виджетами MP3 страницы.
	cOGGBoxWidgets mOGGWidgets;	// Подкласс с виджетами OGG страницы.
	cWAVBoxWidgets mWAVWidgets;	// Подкласс с виджетами WAV страницы.

	bool mbIsAdditionalSettingsVisible; // Показываем ли страницу с доп. настройками для аудио файлов.

public:
	cMainWin(std::string asTitle);	// Конструктор.
	virtual ~cMainWin();			// Виртуальный деструктор, т.к. используем виртуальные методы.

	virtual void Create();			// Основная функция построения класса, в которой происходит заполнение окна виджетами.
	virtual void Destroy();			// Уничтожаем всё что создано в Create. Зачищаем выделенную память, обнуляем поля и т.п.

	void AddNewTrack(cTrack *aTrack);	// Метод добавления трека в список треков.

	static void OnAbout(GtkMenuItem *menuitem, cMainWin *aMainWin);					// Обработчик нажатия на кнопку О программе в главном меню.
	static void OnOpenFile(GtkWidget *widget, cMainWin *aMainWin);					// Обработчик нажатия кнопки Открыть файл.
	static void OnConvert(GtkWidget *widget, cMainWin *aMainWin);					// Обработчик нажатия кнопки Конвертировать.
	static void OnShowAdditionalSettings(GtkWidget *widget, cMainWin *aMainWin);	// Обработчик нажатия кнопки Дополнительно.

	static void OnChangeFormat(GtkComboBox *widget, cMainWin *aMainWin);						// Обработчик смены формата конвертации.
	static void OnQualityChanged (GtkRange *aRange, cMainWin *aMainWin);						// Обработчик изменения значения слайдера.
	static void OnAdjustBoundsQuality(GtkRange *range, gdouble adValue, cMainWin *aMainWin);	// Обработчик изменения границ слайдера.

	static gboolean OnDeleteTrack(GtkWidget *awEventBox, GdkEventButton *aEvent, cTrackBox *aTrack);	// Обработчик удаления трека из списка в окне.

};


#endif /* MAINWIN_H_ */
