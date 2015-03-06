/*
 * Track.cpp
 *
 *  Created on: 05.03.2015
 *      Author: dmitry
 */

#include "Track.h"

cTrack::cTrack(std::string asFullUri, std::string asFileName, std::string asFormat) : iObject (asFullUri)
{
	msFullUri = asFullUri;

	if(!asFileName.empty())
		msFileName = asFileName;

	meFormat = eFormat_Unknown;

	if(!asFormat.empty())
	{
		if((asFormat == "mp3") || (asFormat == "MP3"))
		{
			meFormat = eFormat_MP3;
		}
		else if((asFormat == "wav") || (asFormat == "WAV"))
		{
			meFormat = eFormat_WAV;
		}
		else if((asFormat == "ogg") || (asFormat == "OGG"))
		{
			meFormat = eFormat_OGG;
		}
	}


}

cTrack::~cTrack()
{

}

void cTrack::Create()
{

}
void cTrack::Destroy()
{

}

std::string cTrack::GetFullUri()
{
	return msFullUri;
}

std::string cTrack::GetFileName()
{
	return msFileName;
}
