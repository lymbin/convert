/*
 * Convert.h
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#ifndef CONVERT_H_
#define CONVERT_H_

#include "main.h"
#include "MainWin.h"
#include "Track.h"

class cMainWin;

enum eMP3Quality{
	eMP3Quality_low = 64,
	eMP3Quality_mid = 128,
	eMP3Quality_good = 192,
	eMP3Quality_best = 320,
	eMP3Quality_count = 4
};

enum eOGGQuality{
	eOGGQuality_low = 64,
	eOGGQuality_mid = 128,
	eOGGQuality_good = 192,
	eOGGQuality_best = 250,
	eOGGQuality_count = 4
};

static std::string asMP3Quality[] =
{
	"64", "128", "192", "320"
};

static std::string asOGGQuality[] =
{
	"64", "128", "192", "250"
};

class cConvert{

	int mdQuality;
	eFormat meFormat;

	cMainWin *mMainWin;

	GstElement 	*mPipeline,
				*mFilesrc,
				*mEncode,
				*mFilesink;

	bool mbIsConvertStarted;

	bool mbIsNeedStop;

public:
	cConvert(cMainWin *aMainWin);
	~cConvert();

	void SetQuality(std::string asQuality);
	void SetQuality(int adQuality);

	void SetFormat(std::string asFormat);

	std::string CreatePipelineLine();

	eFormat GetFormat();


	bool Convert();
};

#endif /* CONVERT_H_ */
