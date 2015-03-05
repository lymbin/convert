/*
 * MainWin.cpp
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#include "MainWin.h"

cMainWin::cMainWin(std::string asTitle) : iObject("MainWindow")
{
	mwWindow = 0;
	mwMainBox = 0;

	mwFormat = 0;
	mwQuality = 0;
	mwNewFileNameEntry = 0;
	mwTrackNameBox = 0;	//Бокс со всеми файлами на конвертацию

	mTracks.clear();

	mbIsAdditionalSettingsVisible = false;

	msTitle = asTitle;
}
cMainWin::~cMainWin()
{
	Destroy();
}

void cMainWin::Create()
{
	if(mbIsCreated)
		return;

	// Общие виджеты. Будут использованы несколько раз
	GtkWidget *awHBox;
	GtkWidget *awVBox;
	GtkWidget *awLabel;
	GtkWidget *awFrame;

	GtkWidget *awMenuBar;
	GtkWidget *awSubmenu;
	GtkWidget *awMenuitem;
	GtkWidget *awSubMenuitem;

	GtkWidget *awOpenFileButton;
	GtkWidget *awAdditionButton;
	GtkWidget *awConvertButton;


	mwWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size ((GtkWindow *)mwWindow, 800, 400);
	gtk_window_move((GtkWindow *)mwWindow, 50, 50);
	gtk_window_set_title((GtkWindow *)mwWindow, msTitle.c_str());

	g_signal_connect(
			G_OBJECT(mwWindow), "delete_event",
			G_CALLBACK(gtk_main_quit), this);

	mwMainBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add((GtkContainer *)mwWindow, mwMainBox);



	awMenuBar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (mwMainBox), awMenuBar, FALSE, TRUE, 0);

    awMenuitem = gtk_menu_item_new_with_label ("Справка");
    awSubmenu = gtk_menu_new();
    awSubMenuitem = gtk_menu_item_new_with_label ("О программе");
    gtk_menu_shell_append (GTK_MENU_SHELL (awSubmenu), awSubMenuitem);

    g_signal_connect(awSubMenuitem, "activate",
                                    G_CALLBACK(OnAbout), this);

    gtk_menu_item_set_submenu (GTK_MENU_ITEM (awMenuitem), awSubmenu);
    gtk_menu_shell_append (GTK_MENU_SHELL (awMenuBar), awMenuitem);


    gtk_box_pack_start (GTK_BOX (mwMainBox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, TRUE, 5);

	awVBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(mwMainBox), awVBox, FALSE, FALSE, 1);

	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(awVBox), awHBox, FALSE, FALSE, 1);

	awLabel = gtk_label_new ("1.");
	//gtk_widget_set_halign (awLabel, GTK_ALIGN_START); // Не нужно, если использовать горизонтальный бокс
	gtk_box_pack_start(GTK_BOX(awHBox), awLabel, FALSE, FALSE, 5);

	//awOpenFileBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	//gtk_box_pack_start(GTK_BOX(awOpenBox), awOpenFileBox, FALSE, FALSE, 5);

	//awFilePathEntry = gtk_entry_new ();
	//gtk_box_pack_start(GTK_BOX(awOpenFileBox), awFilePathEntry, TRUE, TRUE, 10);

	awOpenFileButton = gtk_button_new_with_label ("Открыть файл");
	gtk_box_pack_start(GTK_BOX(awHBox), awOpenFileButton, FALSE, FALSE, 10);

    g_signal_connect(awOpenFileButton, "clicked", G_CALLBACK(OnOpenFile), this);

	awVBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(mwMainBox), awVBox, FALSE, FALSE, 10);

	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(awVBox), awHBox, FALSE, FALSE, 1);

	awLabel = gtk_label_new ("2. ");
	gtk_box_pack_start(GTK_BOX(awHBox), awLabel, FALSE, FALSE, 5);



	mwFormat = gtk_combo_box_text_new ();
	for(int i = eFormat_MP3; i < eFormat_Unknown; i++)
	{
		gtk_combo_box_text_insert_text ((GtkComboBoxText *)mwFormat, -1, asFormat[i].c_str());
	}
	gtk_combo_box_set_active((GtkComboBox *)mwFormat, 0);
	gtk_box_pack_start(GTK_BOX(awHBox), mwFormat, FALSE, FALSE, 5);


	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(awVBox), awHBox, FALSE, FALSE, 10);


	mwQuality = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, adQualityCount-1, 1);
	gtk_scale_set_draw_value ((GtkScale *)mwQuality, FALSE);

	for(int i = 0; i < adQualityCount; i++)
	{
		std::string asQ = asQuality[i] +"kbps";

		gtk_scale_add_mark ((GtkScale *)mwQuality, i, GTK_POS_TOP, asQ.c_str());

	}
	gtk_box_pack_start(GTK_BOX(awHBox), mwQuality, TRUE, TRUE, 30);


	awAdditionButton = gtk_button_new_with_label ("Дополнительно");
	gtk_box_pack_end(GTK_BOX(awHBox), awAdditionButton, FALSE, FALSE, 30);

    g_signal_connect(awAdditionButton, "clicked", G_CALLBACK(OnShowAdditionalSettings), this);

	//awLabel = gtk_label_new ("Имя нового файла: ");
	//gtk_box_pack_start(GTK_BOX(awHBox), awLabel, FALSE, FALSE, 15);

	//awNewFileNameEntry = gtk_entry_new ();
	//gtk_box_pack_start(GTK_BOX(awHBox), awNewFileNameEntry, FALSE, FALSE, 5);


	awVBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(mwMainBox), awVBox, FALSE, FALSE, 1);

	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(awVBox), awHBox, FALSE, FALSE, 10);

	awLabel = gtk_label_new ("3.");
	//gtk_widget_set_halign (awLabel, GTK_ALIGN_START); // Не нужно, если использовать горизонтальный бокс
	gtk_box_pack_start(GTK_BOX(awHBox), awLabel, FALSE, FALSE, 5);

	awConvertButton = gtk_button_new_with_label ("Конвертировать");
	gtk_box_pack_start(GTK_BOX(awHBox), awConvertButton, FALSE, FALSE, 10);

	g_signal_connect(awConvertButton, "clicked", G_CALLBACK(OnConvert), this);

	gtk_widget_show_all (mwWindow);

	mbIsCreated = true;
}
void cMainWin::Destroy()
{
	if(!mbIsCreated)
		return;

	std::vector<cTrack *>::iterator aIt = mTracks.begin();
	for(; aIt < mTracks.end(); ++aIt)
	{
		delete (*aIt);
	}

	mbIsCreated = false;
}

void cMainWin::OnAbout(GtkMenuItem *menuitem, cMainWin *aMainWin)
{
	GtkWidget *dialogwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *dialog = gtk_dialog_new_with_buttons (NULL,
													  GTK_WINDOW(dialogwin),
													  GTK_DIALOG_MODAL,
													  "Закрыть",
													  GTK_RESPONSE_REJECT,
													  NULL);

	GtkWidget *awVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	GtkWidget *awLabel;

	awLabel = gtk_label_new (NULL);
	gtk_label_set_markup ((GtkLabel *)awLabel, "<big><b>Audio Converter</b></big>");
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 3);

	std::stringstream asString;
	asString << "<b>Версия " << SYS_VERSION << "</b>";

	awLabel = gtk_label_new (NULL);
	gtk_label_set_markup ((GtkLabel *)awLabel, asString.str().c_str());
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 3);

	awLabel = gtk_label_new ("Copyright 2015 Dmitry Kilchanov & Mind Walkers Studio");
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 3);

	gtk_container_add((GtkContainer *)gtk_dialog_get_content_area ((GtkDialog *)dialog), awVbox);

	//gtk_container_add((GtkContainer *)gtk_dialog_get_content_area ((GtkDialog *)dialog), gtk_label_new ("Покинуть Videoconsole?"));

	gtk_widget_show_all(gtk_dialog_get_content_area ((GtkDialog *)dialog));

	gint result = gtk_dialog_run((GtkDialog *)dialog);

	gtk_widget_show_all(dialog);

	switch(result)
	{
		case GTK_RESPONSE_ACCEPT:
			aMainWin->Destroy();
			gtk_main_quit();
			break;
		default:
			break;
	}

	gtk_widget_destroy(dialog);

	std::cout << "Справка" << std::endl;
}
void cMainWin::OnOpenFile(GtkWidget *widget, cMainWin *aMainWin)
{
	std::cout << "Открыть файл" << std::endl;
}
void cMainWin::OnConvert(GtkWidget *widget, cMainWin *aMainWin)
{
	std::cout << "Конвертация" << std::endl;
}
void cMainWin::OnDeleteTrack(GtkWidget *widget, cMainWin *aMainWin)
{
	std::cout << "Удаление" << std::endl;
}

void cMainWin::OnShowAdditionalSettings(GtkWidget *widget, cMainWin *aMainWin)
{
	std::cout << "Настройки" << std::endl;
}
