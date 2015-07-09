/*
 * MainWin.cpp
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#include "MainWin.h"

// Конструктор cMainWin. Обнуляем и инициализируем поля класса.
// Класс - наследник iObject'a.
cMainWin::cMainWin(std::string asTitle) : iObject("MainWindow")
{
	// Обнуляем указатели на виджеты.
	mwWindow = 0;	// Виджет окна.
	mwMainBox = 0;	// Главный бокс программы.

	mwFormat = 0;	// Комбо бокс для выбора формата конверации.
	mwSpinner = 0;	// Спиннер для визуального отображения процесса конвертации - кушает много ресурсов.

	mMP3Widgets.mwQuality = 0;	// Слайдер настройки качества MP3 кодера.
	mOGGWidgets.mwQuality = 0;	// Слайдер настройки качества OGG кодера.

	mwNewFileNameEntry = 0;		// Поле с новым именем файла - не используется.
	mwTrackNameBox = 0;			// Бокс со всеми файлами на конвертацию.

	// Задаём новый экземпляр класса cConvert.
	mConvert = new cConvert(this);

	mTracks.clear();	// Очищаем список всех записей.

	mbIsAdditionalSettingsVisible = false;	// Не показываем страницу с доп. настройками для аудио файлов.

	msTitle = asTitle;	// Заголовок окна.
}

// Деструктор cMainWin. Сначала удаляем все объекты, что были созданы в методе Create, затем - объекты, которые были созданы в конструкторе.
cMainWin::~cMainWin()
{
	// Функция уничтожения всех объектов, что были созданы в методе Create.
	Destroy();

	// Удаляем экземпляр класса cConvert из памяти.
	if(mConvert)
	{
		delete mConvert;
		mConvert = 0;
	}
}

// Основная функция построения класса, в которой происходит заполнение окна виджетами.
void cMainWin::Create()
{
	// Создаём объект, если ранее он был не создан или был уничтожен.
	if(mbIsCreated)
		return;


	// Общие виджеты. Будут использованы несколько раз.
	GtkWidget *awHBox;		// Горизонтальный бокс.
	GtkWidget *awVBox;		// Вертикальный бокс.
	GtkWidget *awLabel;		// Заголовок.
	GtkWidget *awFrame;		// Фрейм для виджетов.

	GtkWidget *awMenuBar;		// Виджет для основного меню.
	GtkWidget *awSubmenu;		// Подменю.
	GtkWidget *awMenuitem;		// Пункт основного меню.
	GtkWidget *awSubMenuitem;	// Пункт подменю.

	GtkWidget *awOpenFileButton;	// Кнопка открытия файла.
	GtkWidget *awAdditionButton;	// Кнопка доп. настроек.
	GtkWidget *awConvertButton;		// Кнопка конвертации.


	// Настраиваем виджет главного окна.
	mwWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size ((GtkWindow *)mwWindow, 800, 400);	// Задаём размер.
	gtk_window_move((GtkWindow *)mwWindow, 50, 50);					// Перемещаем окно.
	gtk_window_set_title((GtkWindow *)mwWindow, msTitle.c_str());	// Задаём имя.

	// Устанавливаем обработчик события уничтожения окна.
	g_signal_connect(
			G_OBJECT(mwWindow), "delete_event",
			G_CALLBACK(gtk_main_quit), this);



	// Создаём новый вертикальный бокс и забиваем его в основное окно.
	mwMainBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add((GtkContainer *)mwWindow, mwMainBox);



	// Создаём новый виджет основного меню.
	awMenuBar = gtk_menu_bar_new ();
    gtk_box_pack_start (GTK_BOX (mwMainBox), awMenuBar, FALSE, TRUE, 0);	// Забиваем меню в главный бокс.


    // Создаём новый пункт меню Справка и заполняем его подменю.
    awMenuitem = gtk_menu_item_new_with_label ("Справка");
    awSubmenu = gtk_menu_new();


    // Добавляем в подменю новый пункт.
    awSubMenuitem = gtk_menu_item_new_with_label ("О программе");
    gtk_menu_shell_append (GTK_MENU_SHELL (awSubmenu), awSubMenuitem);	// Не забываем связать этот пункт с подменю.

    // Устанавливаем обработчик нажатия на пункт подменю.
    g_signal_connect(awSubMenuitem, "activate",
                                    G_CALLBACK(OnAbout), this);

    gtk_menu_item_set_submenu (GTK_MENU_ITEM (awMenuitem), awSubmenu);	// Связываем подменю с главным.
    gtk_menu_shell_append (GTK_MENU_SHELL (awMenuBar), awMenuitem);		// И добавляем пунтк меню в главное меню.


    // Добавляем в главный бокс разделитель виджетов(простая длинная линия).
    gtk_box_pack_start (GTK_BOX (mwMainBox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, TRUE, 5);


    // Задаём вертикальный бокс для первого пункта процесса конвертации и забиваем его в главный бокс.
	awVBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(mwMainBox), awVBox, FALSE, FALSE, 1);

	// Задаём горизонтальный бокс для первого пункта процесса конвертации и забиваем его в предыдущий вертикальный бокс.
	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(awVBox), awHBox, FALSE, FALSE, 1);

	// Задаём виджет для отображения текста и добавляем в горизонтальный бокс.
	awLabel = gtk_label_new ("1.");
	//gtk_widget_set_halign (awLabel, GTK_ALIGN_START); // Выравнивание. Не нужно, если использовать горизонтальный бокс.
	gtk_box_pack_start(GTK_BOX(awHBox), awLabel, FALSE, FALSE, 5);

	// Создаём кнопку Открыть файл и добавляем в горизонтальный бокс.
	awOpenFileButton = gtk_button_new_with_label ("Открыть файл");
	gtk_box_pack_start(GTK_BOX(awHBox), awOpenFileButton, FALSE, FALSE, 10);

	// Устанавливаем обработчик нажатия на кнопку Открыть файл.
    g_signal_connect(awOpenFileButton, "clicked",
    									G_CALLBACK(OnOpenFile), this);


    // Создаём бокс для списка всех записей и добавляем в горизонтальный бокс.
    mwTrackNameBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(awHBox), mwTrackNameBox, FALSE, FALSE, 5);


    // Переходим ко второму пункту меню
    // Задаём вертикальный бокс для второго пункта процесса конвертации и забиваем его в главный бокс.
	awVBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(mwMainBox), awVBox, FALSE, FALSE, 10);

	// Задаём горизонтальный бокс для второго пункта процесса конвертации и забиваем его в предыдущий вертикальный бокс.
	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(awVBox), awHBox, FALSE, FALSE, 1);

	// Задаём виджет для отображения текста и добавляем в горизонтальный бокс.
	awLabel = gtk_label_new ("2. ");
	gtk_box_pack_start(GTK_BOX(awHBox), awLabel, FALSE, FALSE, 5);


	// Создаём комбо бокс для выбора формата конвертации.
	mwFormat = gtk_combo_box_text_new ();
	for(int i = eFormat_MP3; i < eFormat_Unknown; i++)
	{
		// Для каждого пункта из enum добавляем пункт в комбо боксе со своим текстом из asFormat.
		gtk_combo_box_text_insert_text ((GtkComboBoxText *)mwFormat, -1, asFormat[i].c_str());
	}
	gtk_combo_box_set_active((GtkComboBox *)mwFormat, 0);	// Устанавливаем активный элемент на первый в списке.

	// Задаём обработчик события смены формата.
	g_signal_connect (mwFormat, "changed",
								G_CALLBACK (OnChangeFormat), this);

	gtk_box_pack_start(GTK_BOX(awHBox), mwFormat, FALSE, FALSE, 5);		// Добавляем комбо бокс в горизонтальный бокс.


	// Теперь настроим виджеты, которые будут соответствовать формату конвертации из комбо бокса.
	// Виджеты, соответствующие формату, будут показываться, только при выборе этого формата в комбо боксе.
	// Задаём горизонтальный бокс для настроек под определённый формат и добавляем его в вертикальный бокс.
	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(awVBox), awHBox, FALSE, FALSE, 10);

	// Настроим MP3 виджеты.
	// Задаём слайдер качества с определённым количеством меток.
	mMP3Widgets.mwQuality = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, eMP3Quality_count-1, 1);
	gtk_scale_set_draw_value ((GtkScale *)mMP3Widgets.mwQuality, FALSE);	// Не будем рисовать текущее значение.

	// Добавим метки на шкалу слайдера для визуализации.
	for(int i = 0; i < eMP3Quality_count; i++)
	{
		std::string asQ = asMP3Quality[i] +"kbps";	// Значения мток берём из asMP3Quality массива.
		gtk_scale_add_mark ((GtkScale *)mMP3Widgets.mwQuality, i, GTK_POS_TOP, asQ.c_str());
	}

	gtk_box_pack_start(GTK_BOX(awHBox), mMP3Widgets.mwQuality, TRUE, TRUE, 30);			// Добавляем слайдер в горизонтальный бокс.
	gtk_range_set_show_fill_level ((GtkRange *)mMP3Widgets.mwQuality, TRUE);			// Заполняем слайдер другим цветом.
	gtk_range_set_restrict_to_fill_level ((GtkRange *)mMP3Widgets.mwQuality, TRUE);		// Устанавливаем указатель слайдера до уровня заполнения слайдера.
	gtk_range_set_fill_level ((GtkRange *)mMP3Widgets.mwQuality, eMP3Quality_count-1);	// Полностью заполняем слайдер цветом.
	gtk_range_set_value ((GtkRange *)mMP3Widgets.mwQuality, eMP3Quality_count-1);		// Ставим значение слайдера на последнее из массива значений.

	// Указатель теперь связан с уровнем заполнения, но указатель всё ещё можно двигать, не меняя уровень заполнения.
	// Для этого необходимо ввести 2 обработчика событий - изменение значение слайдера и изменение границ указателя, для перемещения уровня заполнения.
	g_signal_connect (mMP3Widgets.mwQuality, "value-changed", G_CALLBACK (OnQualityChanged), this);
	g_signal_connect (mMP3Widgets.mwQuality, "adjust-bounds", G_CALLBACK (OnAdjustBoundsQuality), this);

	gtk_widget_show(mMP3Widgets.mwQuality);	// Показываем виджеты страницы MP3 конвертера.


	// Настраиваем OGG виджеты.
	// Задаём слайдер качества с определённым количеством меток.
	mOGGWidgets.mwQuality = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, eOGGQuality_count-1, 1);
	gtk_scale_set_draw_value ((GtkScale *)mOGGWidgets.mwQuality, FALSE);

	// Добавим метки на шкалу слайдера для визуализации.
	for(int i = 0; i < eOGGQuality_count; i++)
	{
		std::string asQ = asOGGQuality[i] +"kbps";	// Значения мток берём из asOGGQuality массива.
		gtk_scale_add_mark ((GtkScale *)mOGGWidgets.mwQuality, i, GTK_POS_TOP, asQ.c_str());
	}

	gtk_box_pack_start(GTK_BOX(awHBox), mOGGWidgets.mwQuality, TRUE, TRUE, 30);			// Добавляем слайдер в горизонтальный бокс.
	gtk_range_set_show_fill_level ((GtkRange *)mOGGWidgets.mwQuality, TRUE);			// Заполняем слайдер другим цветом.
	gtk_range_set_restrict_to_fill_level ((GtkRange *)mOGGWidgets.mwQuality, TRUE);		// Устанавливаем указатель слайдера до уровня заполнения слайдера.
	gtk_range_set_fill_level ((GtkRange *)mOGGWidgets.mwQuality, eOGGQuality_count-1);	// Полностью заполняем слайдер цветом.
	gtk_range_set_value ((GtkRange *)mOGGWidgets.mwQuality, eOGGQuality_count-1);		// Ставим значение слайдера на последнее из массива значений.

	// Снова вводим 2 обработчика событий, один на изменение значения слайдера и друго для изменения границ указателя, для перемещения уровня заполнения.
	g_signal_connect (mOGGWidgets.mwQuality, "value-changed", G_CALLBACK (OnQualityChanged), this);
	g_signal_connect (mOGGWidgets.mwQuality, "adjust-bounds", G_CALLBACK (OnAdjustBoundsQuality), this);


	// Добавим новую кнопку Дополнительно для отображения дополнительных параметров и добавим его в горизонтальный бокс.
	awAdditionButton = gtk_button_new_with_label ("Дополнительно");
	gtk_box_pack_end(GTK_BOX(awHBox), awAdditionButton, FALSE, FALSE, 30);

	// Введём обработчик нажатия на кнопку Дополнительно.
    g_signal_connect(awAdditionButton, "clicked",
    									G_CALLBACK(OnShowAdditionalSettings), this);


    // Переходим к третьему пункту меню.
    // Задаём вертикальный бокс для третьего пункта процесса конвертации и забиваем его в главный бокс.
	awVBox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(mwMainBox), awVBox, FALSE, FALSE, 1);

	// Задаём горизонтальный бокс для третьего пункта процесса конвертации и забиваем его в предыдущий вертикальный бокс.
	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(awVBox), awHBox, FALSE, FALSE, 10);

	// Задаём виджет для отображения текста и добавляем в горизонтальный бокс.
	awLabel = gtk_label_new ("3.");
	gtk_box_pack_start(GTK_BOX(awHBox), awLabel, FALSE, FALSE, 5);


	// Добавим кнопку Конвертировать - для начала процесса конвертации ваших аудио файлов. Добавим кнопку в горизонтальный бокс.
	awConvertButton = gtk_button_new_with_label ("Конвертировать");
	gtk_box_pack_start(GTK_BOX(awHBox), awConvertButton, FALSE, FALSE, 10);

	// Создадим спиннер для визуального отображения процесса конвертации.
	// Это просто необходимо, т.к. программа как бы подвисает, из-за того что мы конвертируем в одном потоке с программой.
	// Серъёзно нагружает процессор =(
	mwSpinner = gtk_spinner_new ();
	gtk_box_pack_start(GTK_BOX(awHBox), mwSpinner, FALSE, FALSE, 5);


	// Обработчик события нажатие на кнопку Конвертация.
	g_signal_connect(awConvertButton, "clicked",
										G_CALLBACK(OnConvert), this);


	gtk_widget_show_all (mwWindow);	// Покажем все виджеты в окне.

	gtk_widget_hide(mOGGWidgets.mwQuality);	// Скроем виджеты со страницы OGG. По умолчанию отображаем конвертацию в MP3.
	gtk_widget_hide(mwSpinner);				// Скроем спиннер, т.к. мы хотим его показать только в момент конвертации файлов.


	mbIsCreated = true;	// Объект создан.
}

// Уничтожаем всё что создано в Create. Зачищаем выделенную память, обнуляем поля и т.п.
void cMainWin::Destroy()
{
	// Уничтожаем только созданный объект.
	if(!mbIsCreated)
		return;

	// Удаляем все треки, т.к. они создаются в методах класса.
	tTracksIt It = mTracks.begin();
	for(; It < mTracks.end(); ++It)
	{
		delete (*It);
	}

	mbIsCreated = false;	// Объект уничтожен.
}

// Метод добавления трека в список треков.
void cMainWin::AddNewTrack(cTrack *aTrack)
{
	// Добавляем трек только если был создан бокс c треками для их визуального отображения в окне программы.
	if(!mwTrackNameBox)
		return;

	// Общие виджеты. Будут использованы несколько раз.
	GtkWidget *awHBox;		// Горизонтальный бокс.
	GtkWidget *awLabel;		// Заголовок.
	GtkWidget *awImage;		// Виджет с картинкой.
	GtkWidget *awEventBox;	// Коробка событий, для прикрепления обработчика сигналов на любой виджет. В данном случае на картинку.

	// Задаём новый горизонтальный бокс и добавим его в бокс с треками. Тот, напомню, является вертикальным боксом.
	awHBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start (GTK_BOX (mwTrackNameBox), awHBox, FALSE, FALSE, 0);

	// Добавим загаловок с именем конвертируемого файла.
	awLabel = gtk_label_new (aTrack->GetFileName().c_str());
	gtk_box_pack_start (GTK_BOX (awHBox), awLabel, FALSE, FALSE, 0);

	// Берём картинку из дефолтных GTK картинок.
	awImage = gtk_image_new_from_stock (GTK_STOCK_CLOSE, GTK_ICON_SIZE_SMALL_TOOLBAR);

	// Создаём новый эвент бокс.
	awEventBox = gtk_event_box_new ();

	// Свяжем коробку событий и виджет картинки, а также добавим коробку событий в горизонтальный бокс.
	gtk_container_add (GTK_CONTAINER (awEventBox), awImage);
	gtk_box_pack_start (GTK_BOX (awHBox), awEventBox, FALSE, FALSE, 2);

	// Создадим новый экземпляр класс cTrackBox, содержащий указатель на трек и текущий горизонтальный бокс, в котором отображается трек.
	cTrackBox *aTrackBox = new cTrackBox();

	aTrackBox->mMainWin = this;	// Добавим указатель на основное окно,
	aTrackBox->mTrack = aTrack;	// указатель на трек,
	aTrackBox->mwBox = awHBox;	// и текущий горизонтальный бокс.

	// Добавим к коробке событий событие нажатия на виджет. Что-то наподобие кнопки.
	g_signal_connect (G_OBJECT (awEventBox), "button_press_event", G_CALLBACK (cMainWin::OnDeleteTrack), aTrackBox);


	mTracks.push_back(aTrackBox);	// Трек добавим в массив.

	gtk_widget_show_all(awHBox);	// Покажем все виджеты горизонтального бокса.
}

// Обработчик нажатия на кнопку О программе в главном меню.
void cMainWin::OnAbout(GtkMenuItem *menuitem, cMainWin *aMainWin)
{
	// Создаём новое диалоговое окно с заголовком О программе, полем для текста и кнопкой Закрыть, которая вызовет сигнал GTK_RESPONSE_REJECT при нажатии.
	GtkWidget *dialogwin = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *dialog = gtk_dialog_new_with_buttons ("О Программе",
													  GTK_WINDOW(dialogwin),
													  GTK_DIALOG_MODAL,
													  "Закрыть",
													  GTK_RESPONSE_REJECT,
													  NULL);

	// Создадим вертикальный бокс для текста и прочих виджетов.
	GtkWidget *awVbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);


	GtkWidget *awLabel;
	awLabel = gtk_label_new (NULL);														// Создаём новый заголовок для текста.
	gtk_label_set_markup ((GtkLabel *)awLabel, "<big><b>Audio Converter</b></big>");	// Устанавливаем разметку текста.
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 3);						// Пакуем текст в вертикальный бокс.

	// Другая версия разметки текста в виджете заголовка. Тэг <big> из GTK.
	std::stringstream asString;
	asString << "<b>Версия " << SYS_VERSION << "</b>";

	awLabel = gtk_label_new (NULL);										// Создаём новый заголовок для текста.
	gtk_label_set_markup ((GtkLabel *)awLabel, asString.str().c_str()); // Устанавливаем разметку текста.
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 3);		// Пакуем текст в вертикальный бокс.

	// Создаём новый заголовок для текста и пакуем его в вертикальный бокс.
	awLabel = gtk_label_new ("Developed by Dmitry Kilchanov ");
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 3);

	// Создаём новый заголовок для текста и пакуем его в вертикальный бокс.
	awLabel = gtk_label_new ("Copyright 2015 Mind Walkers ");
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 3);


	// Теперь создадим многострочный текст и забъём его в вертикальный бокс.
	awLabel = gtk_label_new ("1. Выберите один или несколько файлов, которые вы хотите конвертировать. \n"
							"2. Настройте параметры исходящего файла - выберите формат, "
							"укажите качество(доступно не для всех форматов).\n"
							"Если требуется, укажите дополнительные параметры в пункте \"Дополнительно\"(пока не доступно).\n"
							"3. Нажмите кнопку \"Конвертировать\" и немного подождите.\n"
							"Вы только что успешно конвертировали ваши файлы в другой формат, поздравляем!");
	gtk_box_pack_start(GTK_BOX(awVbox), awLabel, FALSE, FALSE, 5);

	// Обязательно добавим наш вертикальный бокс в контейнер диалогового окна.
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

// Обработчик нажатия кнопки Открыть файл.
void cMainWin::OnOpenFile(GtkWidget *widget, cMainWin *aMainWin)
{
	GtkWidget *dialog;	// Объявим новый виджет диалога выбора файла.

	// Теперь создадим его, как отдельное окно с заголовком Выберите файл и двумя кнопками Отмена и Открыть.
	// Нажатие на кнопку Отмена вызовет сигнал GTK_RESPONSE_CANCEL.
	// Нажатие на кнопку Открыть вызовет сигнал GTK_RESPONSE_ACCEPT.
	dialog = gtk_file_chooser_dialog_new ("Выберите файл",
											GTK_WINDOW(aMainWin->mwWindow),
											GTK_FILE_CHOOSER_ACTION_OPEN,
											("_Cancel"), GTK_RESPONSE_CANCEL,
											("_Open"), GTK_RESPONSE_ACCEPT,
											NULL);

	GtkFileFilter *filter = gtk_file_filter_new ();			// Создадим новый фильтр для расширений файлов.
	gtk_file_filter_add_pattern (filter, "*.mp3");			// Добавим в фильтр шаблон файлов с расширением .mp3,
	gtk_file_filter_add_pattern (filter, "*.MP3");			// 												.MP3,
	gtk_file_filter_add_pattern (filter, "*.wav");			// 												.wav,
	gtk_file_filter_add_pattern (filter, "*.WAV");			// 												.WAV,
	gtk_file_filter_add_pattern (filter, "*.ogg");			// 												.ogg,
	gtk_file_filter_add_pattern (filter, "*.OGG");			// 												.OGG
	gtk_file_filter_set_name (filter, ".mp3, .wav, .ogg");	// и установим имя этого фильтра.

	gtk_file_chooser_add_filter (GTK_FILE_CHOOSER(dialog),  filter);	// Добавим фильтр в диалог выбора файла.
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog),  filter);		// Установим фильтр для диалога.

	//Нельзя выбирать несколько записей одновременно - приведёт к непредсказемой работе последующих методов.
	gtk_file_chooser_set_select_multiple (GTK_FILE_CHOOSER(dialog), FALSE);

	// Запустим поток диалогового окна. Тут же вызовется функция отображения всех виджетов окна.
	// Поток будет выполняться до получения сигнала ответа или уничтожения диалогового окна.
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		// Извлекаем нужные URI из полученного.
		unsigned int slash;
		std::string source = (const char *)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));	// Получаем абсолютный путь до файла.

		slash = source.rfind("/");		// Находим последний слэш.
		if(slash==std::string::npos)	// Если слэш - последний символ в источнике, то нужно оповестить об ошибке, т.к. файл не выбран.
		{
			g_print("Ошибка при добавлении файла\n");
			gtk_widget_destroy (dialog);
			aMainWin->PrintNotification("Ошибка при добавлении файла", eNotifTypeError);		// Выводим диалоговое окно с ошибкой.
			return;
		}

		//Извелкаем URI для базы данных
		std::string asFullPatch = source;

		// Используя особенную функцию отсечения текста до последнего символа и функцию замены одного символа на другой получим короткий путь до файла.
		// Именно его и будем использовать в базе данных и при выводе в окне конвертера.
		std::string asShortPatch = aMainWin->FindLastNCopyAfter(asFullPatch, "/");
		aMainWin->FindNReplace(asShortPatch, "/", "");

		// Точно также выделим тип файла.
		std::string asTypeString = aMainWin->FindLastNCopyAfter(asFullPatch, ".");
		aMainWin->FindNReplace(asTypeString, ".", "");


		// Теперь выделим имя папки с файлом с помощью другой функции, которая копирует текст сначала и по последнее упоминание символа.
		std::string asFolderName = aMainWin->FindLastNCopy(asFullPatch, "/");


		// Отобразим в консоли все полученные пути для тестов.
		std::cout << "Короткий путь до файла: " << asShortPatch << std::endl;
		std::cout << "Тип файла: " << asTypeString << std::endl;
		std::cout << "Имя папки с файлом: "  << asFolderName << std::endl;

		// Теперь создадим новый трек с полученными параметрами и добавим его в базу данных.
		cTrack *aTrack = new cTrack(asFullPatch, asShortPatch, asTypeString, asFolderName);
		aMainWin->AddNewTrack(aTrack);
	}

	gtk_widget_destroy (dialog);	// После использования окна, можем его уничтожить.

}

// Обработчик нажатия кнопки Конвертировать.
void cMainWin::OnConvert(GtkWidget *widget, cMainWin *aMainWin)
{
	// Проверим есть ли вообще что конвертировать.
	if(aMainWin->mTracks.empty())
	{
		aMainWin->PrintNotification("Не выбрано ни одного трека для конвертации.", eNotifTypeError);
		return;
	}

	// Проверим создан ли экземпляр конвертера в окне.
	if(!aMainWin->mConvert)
	{
		aMainWin->PrintNotification("Ошибка конвертера. Перезапустите приложение.", eNotifTypeError);
		return;
	}

	// Получим из комбо бокса формат файла в который будем конвертировать все аудио файлы.
	aMainWin->mConvert->SetFormat(gtk_combo_box_text_get_active_text ((GtkComboBoxText *)aMainWin->mwFormat));

	// Для разных форматов необходимо получить параметры конвертации.
	// Например, получим качество файлов на выходе.
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

	// Теперь-то покажем спиннер и запустим его.
	gtk_widget_show(aMainWin->mwSpinner);
	gtk_spinner_start (GTK_SPINNER (aMainWin->mwSpinner));

	// Основная функция конвертации файлов.
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

	gtk_widget_hide(aMainWin->mwSpinner);	// Спрячем спиннер до следующей конвертации.
}

// Обработчик нажатия кнопки Дополнительно.
void cMainWin::OnShowAdditionalSettings(GtkWidget *widget, cMainWin *aMainWin)
{
	// Пока ничего важного не показывает.
	std::cout << "Настройки" << std::endl;
}

// Обработчик изменения границ слайдера. Даёт возможность регулировать границы до изменения позиции указателя слайдера.
void cMainWin::OnAdjustBoundsQuality(GtkRange *aRange, gdouble adValue, cMainWin *aMainWin)
{
	// Устанавливаем уровень заполнения слайдера на значение adValue плюс 0.1, для более точной регулировки далее.
	int adIntValue = (int)(adValue+0.1);
	gtk_range_set_fill_level (aRange, adIntValue);
}

// Обработчик смены формата конвертации.
void cMainWin::OnChangeFormat(GtkComboBox *widget, cMainWin *aMainWin)
{
	// Забираем текстовое значение поля из комбо бокса.
	std::string asFormat = gtk_combo_box_text_get_active_text ((GtkComboBoxText *)aMainWin->mwFormat);

	// И теперь для каждого типа показываем их страницы с параметрами и скрываем все остальные страницы с параметрами.
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

// Обработчик изменения значения слайдера. Тут-то мы и меняем позицию слайдера.
void cMainWin::OnQualityChanged(GtkRange *aRange, cMainWin *aMainWin)
{
	// Получаем значение слайдера плюс 0.1, для более точной регулировки указателя.
	int adValue = (int)(gtk_range_get_value (aRange)+0.1);

	// Смещаем значение слайдера на 0.1.
	if(adValue != gtk_range_get_value (aRange))
	{
		gtk_range_set_value (aRange, adValue);
	}

	std::cout << gtk_range_get_value (aRange) << std::endl;
}

// Обработчик удаления трека из списка в окне.
gboolean cMainWin::OnDeleteTrack(GtkWidget *awEventBox, GdkEventButton *aEvent, cTrackBox *aTrack)
{
	// Если трек существует и заданы его параметры, тогда удалим трек.
	if(aTrack && aTrack->mTrack)
	{
		// Ищем этот трек в листе треков.
		tTracksIt It = aTrack->mMainWin->mTracks.begin();
		for(;It != aTrack->mMainWin->mTracks.end(); ++It)
		{
			// Сравниваем по полным именам файла.
			// Если файлов два и более, то удаляем первый из массива, т.к. проверки на дубликаты нет.
			// Всё равно при конвертации учитывается только то что внутри списка.
			if(aTrack->mTrack->GetFullUri() == (*It)->mTrack->GetFullUri())
			{
				gtk_widget_destroy(aTrack->mwBox);		// Уничтожаем бокс с виджетами данного трека.
				delete (*It);							// Очистим память.
				aTrack->mMainWin->mTracks.erase(It);	// Удалим трек из списка треков.
				break;
			}
		}
	}
	return TRUE;
}

