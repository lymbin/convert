/*
 * Convert.cpp
 *
 *  Created on: 03.03.2015
 *      Author: dmitry
 */

#include "Convert.h"

cConvert::cConvert(cMainWin *aMainWin)
{
	mMainWin = aMainWin;
	mdQuality = 320;
}
cConvert::~cConvert()
{

}

void cConvert::SetQuality(std::string asQuality)
{
	std::stringstream asStream;
	asStream << asQuality;
	asStream >> mdQuality;
}
void cConvert::SetQuality(int adQuality)
{
	mdQuality = adQuality;
}
