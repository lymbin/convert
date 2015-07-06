/*
 * NotificationSystem.h
 *
 *  Created on: 10.03.2015
 *      Author: dmitry
 */

#ifndef NOTIFICATIONSYSTEM_H_
#define NOTIFICATIONSYSTEM_H_

#include "main.h"

// enum для типов сообщений.
enum eNotifType{
	eNotifTypeError = 0,	// Ошибка.
	eNotifTypeWarning,		// Предупреждение.
	eNotifTypeNotification,	// Сообщение.
	eNotifTypeNone			// Не известно.
};

// Класс системы сообщений.
class cNotificationSystem{

	// Метод вывода типа сообщения. По сути переводчик из enum в текст.
	std::string NotifyType(eNotifType eNType)
	{
		if(eNType == eNotifTypeError)
			return "Ошибка";
		else if(eNType == eNotifTypeWarning)
			return "Предупреждение";
		else if(eNType == eNotifTypeNotification)
			return "Уведомление";
		else
			return " ";
	}


public:
	// Отображение сообщения в диалоговом окне.
	void PrintNotification(std::string asNotification, eNotifType eNType = eNotifTypeNone) //Выводим ошибку в отдельное диалоговое окно
	{
		// Создаём новое диалоговое окно с заголовком, зависящим от типа сообщения.
		GtkWidget *dialogwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		GtkWidget *dialog = gtk_dialog_new_with_buttons (NotifyType(eNType).c_str(),
														  GTK_WINDOW(dialogwin),
														  GTK_DIALOG_MODAL,
														  "Закрыть",
														  GTK_RESPONSE_REJECT,
														  NULL);

		// Вводим общий вертикальный бокс.
		GtkWidget *awVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);

		// Теперь построим виджет сообщения и добавим его в вертикальный бокс.
		std::stringstream asString;
		asString << "<b>  " << asNotification << "   </b>";

		GtkWidget *awLabel;
		awLabel = gtk_label_new (NULL);										// Новый заголовок.
		gtk_label_set_markup ((GtkLabel *)awLabel, asString.str().c_str());	// Установим разметку текста.
		gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 5);		// Добавим тест в вертикальный бокс.

		// Вертикальный бокс добавим в контейнер диалогового окна.
		gtk_container_add((GtkContainer *)gtk_dialog_get_content_area ((GtkDialog *)dialog), awVbox);

		// Покажем все виджеты контейнера диалогового окна.
		gtk_widget_show_all(gtk_dialog_get_content_area ((GtkDialog *)dialog));

		// Запустим поток диалогового окна. Тут же вызовется функция отображения всех виджетов окна.
		// Поток будет выполняться до получения сигнала ответа или уничтожения диалогового окна.
		gint result = gtk_dialog_run((GtkDialog *)dialog);

		// Далее можно будет обработать все события диалогового окна.
		switch(result)
		{
			default:
				break;
		}

		gtk_widget_destroy(dialog);	// После использования окна, можем его уничтожить.
	}

	//Выводим ошибку как лейбл в существующий виджет (подразумевается что это не контейнер типа frame). Пока не используем.
	void PrintError(std::string asError, GtkWidget *aWidget)
	{

	}

};


#endif /* NOTIFICATIONSYSTEM_H_ */
