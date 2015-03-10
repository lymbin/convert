/*
 * NotificationSystem.h
 *
 *  Created on: 10.03.2015
 *      Author: dmitry
 */

#ifndef NOTIFICATIONSYSTEM_H_
#define NOTIFICATIONSYSTEM_H_

#include "main.h"

enum eNotifType{
	eNotifTypeError = 0,
	eNotifTypeWarning,
	eNotifTypeNotification,
	eNotifTypeNone
};

class cNotificationSystem{

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
	void PrintNotification(std::string asNotification, eNotifType eNType = eNotifTypeNone) //Выводим ошибку в отдельное диалоговое окно
	{
		GtkWidget *dialogwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		GtkWidget *dialog = gtk_dialog_new_with_buttons (NotifyType(eNType).c_str(),
														  GTK_WINDOW(dialogwin),
														  GTK_DIALOG_MODAL,
														  "Закрыть",
														  GTK_RESPONSE_REJECT,
														  NULL);

		GtkWidget *awVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
		GtkWidget *awLabel;

		std::stringstream asString;
		asString << "<b>  " << asNotification << "   </b>";

		awLabel = gtk_label_new (NULL);
		gtk_label_set_markup ((GtkLabel *)awLabel, asString.str().c_str());
		gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 5);

		gtk_container_add((GtkContainer *)gtk_dialog_get_content_area ((GtkDialog *)dialog), awVbox);

		gtk_widget_show_all(gtk_dialog_get_content_area ((GtkDialog *)dialog));

		gint result = gtk_dialog_run((GtkDialog *)dialog);

		gtk_widget_show_all(dialog);

		switch(result)
		{
			default:
				break;
		}

		gtk_widget_destroy(dialog);
	}
	void PrintError(std::string asError, GtkWidget *aWidget) //Выводим ошибку как лейбл в существующий виджет (подразумевается что это не контейнер типа frame)
	{

	}

};


#endif /* NOTIFICATIONSYSTEM_H_ */
