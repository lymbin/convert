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

class cConvert;
class cMainWin;
struct cTrackBox;

typedef std::vector<cTrackBox *> tTracks;
typedef std::vector<cTrackBox *>::iterator tTracksIt;


struct cTrackBox{
	GtkWidget *mwBox;
	cMainWin *mMainWin;
	cTrack *mTrack;
};

struct cMP3BoxWidgets
{
	GtkWidget *mwQuality;
};
struct cOGGBoxWidgets
{
	GtkWidget *mwQuality;
};
struct cWAVBoxWidgets
{

};

class cMainWin: public iObject, public cCommonStringFunctions, public cNotificationSystem{

	friend cConvert;

	GtkWidget *mwWindow;
	GtkWidget *mwMainBox;

	GtkWidget *mwFormat;
	GtkWidget *mwNewFileNameEntry;
	GtkWidget *mwSpinner;

	GtkWidget *mwTrackNameBox;	//Бокс со всеми файлами на конвертацию

	std::string msTitle;

	tTracks mTracks;

	cConvert *mConvert;

	cMP3BoxWidgets mMP3Widgets;
	cOGGBoxWidgets mOGGWidgets;
	cWAVBoxWidgets mWAVWidgets;

	bool mbIsAdditionalSettingsVisible;

public:
	cMainWin(std::string asTitle);
	virtual ~cMainWin();

	virtual void Create();
	virtual void Destroy();

	void AddNewTrack(cTrack *aTrack);

	static void OnAbout(GtkMenuItem *menuitem, cMainWin *aMainWin);
	static void OnOpenFile(GtkWidget *widget, cMainWin *aMainWin);
	static void OnConvert(GtkWidget *widget, cMainWin *aMainWin);
	static void OnShowAdditionalSettings(GtkWidget *widget, cMainWin *aMainWin);

	static void OnChangeFormat(GtkComboBox *widget, cMainWin *aMainWin);
	static void OnQualityChanged (GtkRange *aRange, cMainWin *aMainWin);
	static void OnAdjustBoundsQuality(GtkRange *range, gdouble adValue, cMainWin *aMainWin);

	static gboolean OnDeleteTrack(GtkWidget *awEventBox, GdkEventButton *aEvent, cTrackBox *aTrack);



	//The “changed” signal

};


#endif /* MAINWIN_H_ */
