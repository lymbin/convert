/*
 * Track.h
 *
 *  Created on: 05.03.2015
 *      Author: dmitry
 */

#ifndef TRACK_H_
#define TRACK_H_

#include "main.h"
#include "Object.h"

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

class cTrack: public iObject{

	eFormat meFormat;
	std::string msFullUri;
	std::string msFileName;

public:
	cTrack(std::string asFullUri, std::string asFileName = "", std::string asFormat = "");

	~cTrack();

	virtual void Create();
	virtual void Destroy();

	std::string GetFullUri();
	std::string GetFileName();
};


#endif /* TRACK_H_ */
