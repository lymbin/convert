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

class cMainWin;

static std::string asQuality[] =
{
	"64", "128", "192", "320"
};

static int adQualityCount = 4;

class cConvert{

	int mdQuality;
	cMainWin *mMainWin;

public:
	cConvert(cMainWin *aMainWin);
	~cConvert();

	void SetQuality(std::string asQuality);
	void SetQuality(int adQuality);
};

#endif /* CONVERT_H_ */
