/*
 * Track.h
 *
 *  Created on: 05.03.2015
 *      Author: dmitry
 */

#ifndef TRACK_H_
#define TRACK_H_

#include "main.h"

enum eFormat
{
	eFormat_MP3 = 0,
	eFormat_WAV,
	eFormat_OGG,
	eFormat_Unknown,
};

static std::string asFormat[] =
{
	"MP3", "WAV", "OGG"
};

static int adFormatCount = 3;

static std::string asQuality[] =
{
	"64", "128", "192", "320"
};

static int adQualityCount = 4;

class cTrack : public iObject {

	eFormat meFormat;
	std::string msUri;

public:
	cTrack() : iObject("track") {meFormat = eFormat_Unknown;}
	~cTrack(){Destroy();}

	void Create(){}
	void Destroy(){}
};


#endif /* TRACK_H_ */
