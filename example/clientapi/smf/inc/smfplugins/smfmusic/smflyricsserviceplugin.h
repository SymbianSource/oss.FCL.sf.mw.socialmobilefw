/**
 * @file	smflyricsserviceplugin.h
 * @author  Nalina Hariharan, Sasken Communication Technologies Ltd - Initial contribution
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2010 Sasken Communication Technologies Ltd. 
 * All rights reserved.
 * This component and the accompanying materials are made available 
 * under the terms of the "Eclipse Public License v1.0"
 * which accompanies  this distribution, and is available 
 * at the URL "http://www.eclipse.org/legal/epl-v10.html"
 * 
 * @section DESCRIPTION
 *
 * Interface specification for music track lyrics
 */

#ifndef SMFLYRICSSERVICEPLUGIN_H_
#define SMFLYRICSSERVICEPLUGIN_H_

#include <smfpluginbase.h>
#include <smftrackinfo.h>
#include <QString>
#include <smfsubtitle.h>
#include <smflyrics.h>

/**
 * @ingroup smf_plugin_group
 * Interface specification for music track lyrics
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 */
class SmfLyricsServicePlugin : public SmfPluginBase
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfLyricsServicePlugin( QObject* aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfLyricsServicePlugin( );
	
	/**
	 * Method to get the lyrics
	 * @param aRequest [out] The request data to be sent to network
	 * @param aTrack The track whose lyrics need to be fetched
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError lyrics( SmfPluginRequestData *aRequest, 
			const SmfTrackInfo aTrack ) = 0;
	
	/**
	 * Method to get the subtitle
	 * @param aRequest [out] The request data to be sent to network
	 * @param aTrack The track whose subtitle need to be fetched
	 * @param aFilter The subtitle search filter if any
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError subtitles( SmfPluginRequestData *aRequest, 
			const SmfTrackInfo aTrack,
			const SmfSubtitleSearchFilter aFilter = SubtitleAll ) = 0;
	
	};

Q_DECLARE_INTERFACE( SmfLyricsServicePlugin, "org.symbian.smf.plugin.music.lyrics/v1.0" );

#endif /* SMFLYRICSSERVICEPLUGIN_H_ */
