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
	mwSpinner = 0;

	mMP3Widgets.mwQuality = 0;
	mOGGWidgets.mwQuality = 0;

	mwNewFileNameEntry = 0;
	mwTrackNameBox = 0;	//Бокс со всеми файлами на конвертацию

	mConvert = new cConvert(this);

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

	//awFilePathEntry = gtk_entry_new ();
	//gtk_box_pack_start(GTK_BOX(awOpenFileBox), awFilePathEntry, TRUE, TRUE, 10);

	awOpenFileButton = gtk_button_new_with_label ("Открыть файл");
	gtk_box_pack_start(GTK_BOX(awHBox), awOpenFileButton, FALSE, FALSE, 10);

    g_signal_connect(awOpenFileButton, "clicked", G_CALLBACK(OnOpenFile), this);

    mwTrackNameBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(awHBox), mwTrackNameBox, FALSE, FALSE, 5);


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
	g_signal_connect (mwFormat, "changed", G_CALLBACK (OnChangeFormat), this);


	gtk_box_pack_start(GTK_BOX(awHBox), mwFormat, FALSE, FALSE, 5);


	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(awVBox), awHBox, FALSE, FALSE, 10);

	//MP3
	mMP3Widgets.mwQuality = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, eMP3Quality_count-1, 1);
	gtk_scale_set_draw_value ((GtkScale *)mMP3Widgets.mwQuality, FALSE);

	for(int i = 0; i < eMP3Quality_count; i++)
	{
		std::string asQ = asMP3Quality[i] +"kbps";

		gtk_scale_add_mark ((GtkScale *)mMP3Widgets.mwQuality, i, GTK_POS_TOP, asQ.c_str());

	}
	gtk_box_pack_start(GTK_BOX(awHBox), mMP3Widgets.mwQuality, TRUE, TRUE, 30);
	gtk_range_set_show_fill_level ((GtkRange *)mMP3Widgets.mwQuality, TRUE);
	gtk_range_set_restrict_to_fill_level ((GtkRange *)mMP3Widgets.mwQuality, TRUE);
	gtk_range_set_fill_level ((GtkRange *)mMP3Widgets.mwQuality, eMP3Quality_count-1);
	gtk_range_set_value ((GtkRange *)mMP3Widgets.mwQuality, eMP3Quality_count-1);

	g_signal_connect (mMP3Widgets.mwQuality, "value-changed", G_CALLBACK (OnQualityChanged), this);
	g_signal_connect (mMP3Widgets.mwQuality, "adjust-bounds", G_CALLBACK (OnAdjustBoundsQuality), this);

	gtk_widget_show(mMP3Widgets.mwQuality);


	//OGG
	mOGGWidgets.mwQuality = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, eOGGQuality_count-1, 1);
	gtk_scale_set_draw_value ((GtkScale *)mOGGWidgets.mwQuality, FALSE);

	for(int i = 0; i < eOGGQuality_count; i++)
	{
		std::string asQ = asOGGQuality[i] +"kbps";

		gtk_scale_add_mark ((GtkScale *)mOGGWidgets.mwQuality, i, GTK_POS_TOP, asQ.c_str());

	}
	gtk_box_pack_start(GTK_BOX(awHBox), mOGGWidgets.mwQuality, TRUE, TRUE, 30);
	gtk_range_set_show_fill_level ((GtkRange *)mOGGWidgets.mwQuality, TRUE);
	gtk_range_set_restrict_to_fill_level ((GtkRange *)mOGGWidgets.mwQuality, TRUE);
	gtk_range_set_fill_level ((GtkRange *)mOGGWidgets.mwQuality, eOGGQuality_count-1);
	gtk_range_set_value ((GtkRange *)mOGGWidgets.mwQuality, eOGGQuality_count-1);

	g_signal_connect (mOGGWidgets.mwQuality, "value-changed", G_CALLBACK (OnQualityChanged), this);
	g_signal_connect (mOGGWidgets.mwQuality, "adjust-bounds", G_CALLBACK (OnAdjustBoundsQuality), this);

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

	mwSpinner = gtk_spinner_new ();
	gtk_box_pack_start(GTK_BOX(awHBox), mwSpinner, FALSE, FALSE, 5);


	g_signal_connect(awConvertButton, "clicked", G_CALLBACK(OnConvert), this);

	gtk_widget_show_all (mwWindow);

	gtk_widget_hide(mOGGWidgets.mwQuality);
	gtk_widget_hide(mwSpinner);


	mbIsCreated = true;
}
void cMainWin::Destroy()
{
	if(!mbIsCreated)
		return;

	if(mConvert)
	{
		delete mConvert;
		mConvert = 0;
	}

	tTracksIt It = mTracks.begin();
	for(; It < mTracks.end(); ++It)
	{
		delete (*It);
	}

	mbIsCreated = false;
}

void cMainWin::AddNewTrack(cTrack *aTrack)
{
	if(!mwTrackNameBox)
		return;

	GtkWidget *awHBox;
	GtkWidget *awLabel;
	GtkWidget *awImage;
	GtkWidget *awEventBox;

	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start (GTK_BOX (mwTrackNameBox), awHBox, FALSE, FALSE, 0);

	awLabel = gtk_label_new (aTrack->GetFileName().c_str());
	gtk_box_pack_start (GTK_BOX (awHBox), awLabel, FALSE, FALSE, 0);

	awImage = gtk_image_new_from_stock (GTK_STOCK_CLOSE, GTK_ICON_SIZE_SMALL_TOOLBAR);
	awEventBox = gtk_event_box_new ();

	gtk_container_add (GTK_CONTAINER (awEventBox), awImage);
	gtk_box_pack_start (GTK_BOX (awHBox), awEventBox, FALSE, FALSE, 2);

	cTrackBox *aTrackBox = new cTrackBox();

	aTrackBox->mMainWin = this;
	aTrackBox->mTrack = aTrack;
	aTrackBox->mwBox = awHBox;

	g_signal_connect (G_OBJECT (awEventBox), "button_press_event", G_CALLBACK (cMainWin::OnDeleteTrack), aTrackBox);


	mTracks.push_back(aTrackBox);

	gtk_widget_show_all(awHBox);
}

void cMainWin::OnAbout(GtkMenuItem *menuitem, cMainWin *aMainWin)
{
	GtkWidget *dialogwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *dialog = gtk_dialog_new_with_buttons ("О Программе",
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

	awLabel = gtk_label_new ("Copyright 2015 Mind Walkers Studio");
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 3);

	awLabel = gtk_label_new ("1. Выберите один или несколько файлов, которые вы хотите конвертировать. \n"
							"2. Настройте параметры исходящего файла - выберите формат, "
							"укажите качество(доступно не для всех форматов).\n"
							"Если требуется, укажите дополнительные параметры в пункте \"Дополнительно\"(пока не доступно).\n"
							"3. Нажмите кнопку \"Конвертировать\" и немного подождите.\n"
							"Вы только что успешно конвертировали ваши файлы в другой формат, поздравляем!");
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 5);

	gtk_container_add((GtkContainer *)gtk_dialog_get_content_area ((GtkDialog *)dialog), awVbox);

	//gtk_container_add((GtkContainer *)gtk_dialog_get_content_area ((GtkDialog *)dialog), gtk_label_new ("Покинуть Videoconsole?"));

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
void cMainWin::OnOpenFile(GtkWidget *widget, cMainWin *aMainWin)
{
	GtkWidget *dialog;

	dialog = gtk_file_chooser_dialog_new ("Выберите файл",
											GTK_WINDOW(aMainWin->mwWindow),
											GTK_FILE_CHOOSER_ACTION_OPEN,
											("_Cancel"), GTK_RESPONSE_CANCEL,
											("_Open"), GTK_RESPONSE_ACCEPT,
											NULL);

	GtkFileFilter *filter = gtk_file_filter_new ();
	gtk_file_filter_add_pattern (filter, "*.mp3");
	gtk_file_filter_add_pattern (filter, "*.MP3");
	gtk_file_filter_add_pattern (filter, "*.wav");
	gtk_file_filter_add_pattern (filter, "*.WAV");
	gtk_file_filter_add_pattern (filter, "*.ogg");
	gtk_file_filter_add_pattern (filter, "*.OGG");
	gtk_file_filter_set_name (filter, ".mp3, .wav, .ogg");
	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(dialog),  filter);
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog),  filter);

	//Нельзя выбирать несколько записей одновременно - приведёт к непредсказемой работе последующих методов.
	gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER(dialog), FALSE);



	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		//Извлекаем нужные URI из полученного
		unsigned int slash;
		std::string source = (const char *)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));

		slash = source.rfind("/");
		if(slash==std::string::npos)
		{
			g_print("Ошибка при добавлении файла\n");
			gtk_widget_destroy (dialog);
			aMainWin->PrintNotification("Ошибка при добавлении файла", eNotifTypeError);
			return;
		}

		//Извелкаем URI для базы данных
		std::string asFullPatch = source;

		std::string asShortPatch = aMainWin->FindLastNCopyAfter(asFullPatch, "/");
		aMainWin->FindNReplace(asShortPatch, "/", "");

		std::string asTypeString = aMainWin->FindLastNCopyAfter(asFullPatch, ".");
		aMainWin->FindNReplace(asTypeString, ".", "");

		std::string asFolderName = aMainWin->FindLastNCopy(asFullPatch, "/");


		std::cout << asShortPatch << std::endl;
		std::cout << asTypeString << std::endl;
		std::cout << asFolderName << std::endl;


		cTrack *aTrack = new cTrack(asFullPatch, asShortPatch, asTypeString, asFolderName);
		aMainWin->AddNewTrack(aTrack);
	}

	gtk_widget_destroy (dialog);

}
void cMainWin::OnConvert(GtkWidget *widget, cMainWin *aMainWin)
{
	if(aMainWin->mTracks.empty())
	{
		aMainWin->PrintNotification("Не выбрано ни одного трека для конвертации.", eNotifTypeError);
		return;
	}

	if(!aMainWin->mConvert)
	{
		aMainWin->PrintNotification("Ошибка конвертера. Перезапустите приложение.", eNotifTypeError);
		return;
	}

	aMainWin->mConvert->SetFormat(gtk_combo_box_text_get_active_text ((GtkComboBoxText *)aMainWin->mwFormat));

	if(aMainWin->mConvert->GetFormat() == eFormat_MP3)
	{
		int adQualityMark = gtk_range_get_value ((GtkRange *)aMainWin->mMP3Widgets.mwQuality);
		aMainWin->mConvert->SetQuality(asMP3Quality[adQualityMark]);
	}
	else if(aMainWin->mConvert->GetFormat() == eFormat_OGG)
	{
		int adQualityMark = gtk_range_get_value ((GtkRange *)aMainWin->mOGGWidgets.mwQuality);
		aMainWin->mConvert->SetQuality(asOGGQuality[adQualityMark]);
	}
	else if(aMainWin->mConvert->GetFormat() == eFormat_WAV)
	{

	}

	gtk_widget_show(aMainWin->mwSpinner);
	gtk_spinner_start (GTK_SPINNER (aMainWin->mwSpinner));

	if(!aMainWin->mConvert->Convert())
	{
		gtk_spinner_stop (GTK_SPINNER (aMainWin->mwSpinner));
		aMainWin->PrintNotification("Что-то пошло не так при конвертации.", eNotifTypeError);
	}
	else
	{
		gtk_spinner_stop (GTK_SPINNER (aMainWin->mwSpinner));
		aMainWin->PrintNotification("Конвертация прошла успешно!", eNotifTypeNotification);
	}

	gtk_widget_hide(aMainWin->mwSpinner);
}

void cMainWin::OnShowAdditionalSettings(GtkWidget *widget, cMainWin *aMainWin)
{
	std::cout << "Настройки" << std::endl;
}

void cMainWin::OnAdjustBoundsQuality(GtkRange *aRange, gdouble adValue, cMainWin *aMainWin)
{
	int adIntValue = (int)(adValue+0.1);
	gtk_range_set_fill_level (aRange, adIntValue);
}

void cMainWin::OnChangeFormat(GtkComboBox *widget, cMainWin *aMainWin)
{
	std::string asFormat = gtk_combo_box_text_get_active_text ((GtkComboBoxText *)aMainWin->mwFormat);

	if(asFormat == "MP3")
	{
		gtk_widget_show(aMainWin->mMP3Widgets.mwQuality);
		gtk_widget_hide(aMainWin->mOGGWidgets.mwQuality);

	}
	else if(asFormat == "OGG")
	{
		gtk_widget_show(aMainWin->mOGGWidgets.mwQuality);
		gtk_widget_hide(aMainWin->mMP3Widgets.mwQuality);
	}
	else if(asFormat == "WAV")
	{
		gtk_widget_hide(aMainWin->mOGGWidgets.mwQuality);

		gtk_widget_hide(aMainWin->mMP3Widgets.mwQuality);
	}
}

void cMainWin::OnQualityChanged(GtkRange *aRange, cMainWin *aMainWin)
{
	int adValue = (int)(gtk_range_get_value (aRange)+0.1);

	if(adValue != gtk_range_get_value (aRange))
	{
		gtk_range_set_value (aRange, adValue);
	}

	std::cout << gtk_range_get_value (aRange) << std::endl;
}
gboolean cMainWin::OnDeleteTrack(GtkWidget *awEventBox, GdkEventButton *aEvent, cTrackBox *aTrack)
{
	if(aTrack && aTrack->mTrack)
	{
		tTracksIt It = aTrack->mMainWin->mTracks.begin();

		for(;It != aTrack->mMainWin->mTracks.end(); ++It)
		{
			if(aTrack->mTrack->GetFullUri() == (*It)->mTrack->GetFullUri())
			{
				gtk_widget_destroy(aTrack->mwBox);
				delete (*It);
				aTrack->mMainWin->mTracks.erase(It);
				break;
			}
		}
	}
	return TRUE;
}

