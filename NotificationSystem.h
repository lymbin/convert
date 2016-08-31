/*
 * NotificationSystem.h
 *
 *  Created on: 10.03.2015
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

#ifndef NOTIFICATIONSYSTEM_H_
#define NOTIFICATIONSYSTEM_H_

#include "main.h"

// enum для типов сообщений.
enum eNotifType {
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
			return "Error";
		else if(eNType == eNotifTypeWarning)
			return "Warning";
		else if(eNType == eNotifTypeNotification)
			return "Note";
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
														  "Close",
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
		// not used yet
	}

};


#endif /* NOTIFICATIONSYSTEM_H_ */
