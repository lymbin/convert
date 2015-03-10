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
	meFormat = eFormat_MP3;

	mbIsConvertStarted = false;
	mbIsNeedStop = false;

	mPipeline = 0;
	mFilesrc = 0;
	mFilesink = 0;
	mEncode = 0;
}
cConvert::~cConvert()
{
	mbIsNeedStop = true;

	while(mbIsConvertStarted)
	{
		g_usleep(200000);
	}

	if(mPipeline)
	{
		gst_element_set_state (mPipeline, GST_STATE_NULL);
		gst_object_unref (mPipeline);
		mPipeline = 0;
	}
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

void cConvert::SetFormat(std::string asFormatt)
{
	for(int i = 0; i < eFormat_Unknown; i++)
	{
		if(asFormatt == asFormat[i])
		{
			meFormat = (eFormat)i;
			return;
		}
	}

	meFormat = eFormat_Unknown;

}

std::string cConvert::CreatePipelineLine()
{
	std::stringstream asConvertLine;

	asConvertLine << "filesrc name=convert_src ! decodebin ! audioconvert ! ";

	if(meFormat == eFormat_MP3)
	{
		asConvertLine << "lamemp3enc name=convert_format";
	}
	else if(meFormat == eFormat_WAV)
	{
		asConvertLine << "wavenc name=convert_format";
	}
	else if(meFormat == eFormat_OGG)
	{
		asConvertLine << "vorbisenc name=convert_format ! oggmux";
	}

	asConvertLine << " ! filesink name=convert_sink ";

	return asConvertLine.str();
}

bool cConvert::Convert()
{
	tTracksIt aIt = mMainWin->mTracks.begin();
	std::string asConvertLine = CreatePipelineLine();
	std::stringstream asNewTuneUri;

	bool abIsConvertComplete = true;

	GstMessage *aMsg = 0;
	GstBus *aBus = 0;
	GError *aError = NULL;

	for(; aIt < mMainWin->mTracks.end(); ++aIt)
	{
		if(meFormat == eFormat_Unknown)
			continue;
		if(meFormat == eFormat_WAV && (*aIt)->mTrack->GetFormat() == eFormat_WAV)
		{
			continue;
		}

		mPipeline = gst_parse_launch (asConvertLine.c_str(), &aError);
		if (!mPipeline) {
		    g_print ("Parse error: %s\n", aError->message);
		    abIsConvertComplete = false;
		}

		mFilesrc = gst_bin_get_by_name (GST_BIN (mPipeline), "convert_src");
		g_object_set (mFilesrc, "location", (*aIt)->mTrack->GetFullUri().c_str(), NULL);

		mEncode = gst_bin_get_by_name (GST_BIN (mPipeline), "convert_format");

		asNewTuneUri << (*aIt)->mTrack->GetFolderUri() << "/" << mMainWin->FindLastNCopy((*aIt)->mTrack->GetFileName(), ".");


		if(meFormat == eFormat_MP3)
		{
			g_object_set (mEncode, "bitrate", mdQuality, NULL);
			asNewTuneUri << ".mp3";
		}
		else if(meFormat == eFormat_WAV)
		{
			asNewTuneUri << ".wav";
		}
		else if(meFormat == eFormat_OGG)
		{
			if(mdQuality*1000 > 250000)
				g_object_set (mEncode, "bitrate", 250000, NULL);
			else
				g_object_set (mEncode, "bitrate", mdQuality*1000, NULL);

			asNewTuneUri << ".ogg";
		}

		std::cout << asNewTuneUri.str() << std::endl;
		mFilesink = gst_bin_get_by_name (GST_BIN (mPipeline), "convert_sink");
		g_object_set (mFilesink, "location", asNewTuneUri.str().c_str(), NULL);


		gst_element_set_state (mPipeline, GST_STATE_PLAYING);

		aBus = gst_element_get_bus (mPipeline);

		aMsg = gst_bus_poll (aBus, (GstMessageType) (GST_MESSAGE_EOS | GST_MESSAGE_ERROR), -1);

		switch (GST_MESSAGE_TYPE (aMsg)) {
			case GST_MESSAGE_EOS: {
				g_print ("EOS\n");
				break;
			}
			case GST_MESSAGE_ERROR: {
				GError *err = NULL; /* error to show to users                 */
				gchar *dbg = NULL;  /* additional debug string for developers */

				gst_message_parse_error (aMsg, &err, &dbg);
				if (err) {
					g_printerr ("ERROR: %s\n", err->message);
					g_error_free (err);
				}
				if (dbg) {
					g_printerr ("[Debug details: %s]\n", dbg);
					g_free (dbg);
				}
			}
			default:
				g_printerr ("Unexpected message of type %d", GST_MESSAGE_TYPE (aMsg));
				break;
		}

		gst_message_unref (aMsg);
		gst_element_set_state (mPipeline, GST_STATE_NULL);
		gst_object_unref (mPipeline);
		gst_object_unref (aBus);

		mPipeline = 0;
		aBus = 0;

	}


	return abIsConvertComplete;
}
