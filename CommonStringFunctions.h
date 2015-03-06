/*
 * CommonStringFunctions.h
 *
 *  Created on: 10.11.2014
 *      Author: dmitry
 */

#ifndef COMMONSTRINGFUNCTIONS_H_
#define COMMONSTRINGFUNCTIONS_H_

class cCommonStringFunctions
{
public:
	std::string FindNReplace(std::string &asText, std::string asFindText, std::string asReplaceText)
	{
		if(asFindText == "")
			return asText;

		unsigned int Pos = asText.find(asFindText);

		unsigned int ReplaceSize = asFindText.size();

		if(!ReplaceSize)
			ReplaceSize = 1;

		while(Pos < asText.size() && Pos != std::string::npos)
		{
			asText.replace(Pos, ReplaceSize, asReplaceText);
			Pos = asText.find(asFindText);
		}

		return asText;
	}

	std::string FindLastNCopy(std::string asText, std::string asFindText)
	{
		if(asFindText == "")
			return "";

		unsigned int Pos = asText.find_last_of(asFindText);

		std::string asCopyText;

		std::copy(asText.begin() + Pos, asText.end(), std::back_inserter(asCopyText));

		return asCopyText;
	}

	bool Find(std::string asText, std::string asFindText)
	{
		if(asFindText == "")
			return false;

		unsigned int Pos = asText.find(asFindText);

		if(Pos < asText.size() && Pos != std::string::npos)
		{
			return true;
		}

		return false;
	}


};


#endif /* COMMONSTRINGFUNCTIONS_H_ */
