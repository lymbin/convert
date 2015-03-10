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

enum eQuality{
	eQuality_low = 64,
	eQuality_mid = 128,
	eQuality_good = 192,
	eQuality_best = 320,
	eQuality_count = 4
};


static std::string asQuality[] =
{
	"64", "128", "192", "320"
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


	bool Convert();
};

#endif /* CONVERT_H_ */
