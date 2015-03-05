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

class cMainWin: public iObject{

	GtkWidget *mwWindow;
	GtkWidget *mwMainBox;

	GtkWidget *mwFormat;
	GtkWidget *mwQuality;
	GtkWidget *mwNewFileNameEntry;

	GtkWidget *mwTrackNameBox;	//Бокс со всеми файлами на конвертацию

	std::string msTitle;

	int mdCurrentTrack;

	std::vector<cTrack *> mTracks;

	bool mbIsAdditionalSettingsVisible;

public:
	cMainWin(std::string asTitle);
	~cMainWin();

	void Create();
	void Destroy();

	static void OnAbout(GtkMenuItem *menuitem, cMainWin *aMainWin);
	static void OnOpenFile(GtkWidget *widget, cMainWin *aMainWin);
	static void OnConvert(GtkWidget *widget, cMainWin *aMainWin);
	static void OnDeleteTrack(GtkWidget *widget, cMainWin *aMainWin);
	static void OnShowAdditionalSettings(GtkWidget *widget, cMainWin *aMainWin);
};


#endif /* MAINWIN_H_ */
