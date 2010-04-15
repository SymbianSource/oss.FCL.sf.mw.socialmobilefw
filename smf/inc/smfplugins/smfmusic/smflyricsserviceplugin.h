/**
 * Copyright (c) 2010 Sasken Communication Technologies Ltd. 
 * All rights reserved.
 * This component and the accompanying materials are made available 
 * under the terms of the "Eclipse Public License v1.0"
 * which accompanies  this distribution, and is available 
 * at the URL "http://www.eclipse.org/legal/epl-v10.html"
 * 
 * Initial Contributors:
 * Chandradeep Gandhi, Sasken Communication Technologies Ltd - Initial contribution
 *
 * Contributors:
 * Manasij Roy, Nalina Hariharan
 * 
 * Description:
 * Interface specification for music track lyrics
 *
 */


#ifndef SMFLYRICSSERVICEPLUGIN_H_
#define SMFLYRICSSERVICEPLUGIN_H_

#include <smfpluginbase.h>
#include <smftrackinfo.h>
#include <QString>
#include <smfsubtitle.h>
#include <smflyrics.h>

// Forward declaration
class SmfPluginManagerUtil;

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
	 * @param aUtil The SmfPluginManagerUtil instance. The plugins can 
	 * call the method getAuthKeys() of this class, with its pluginID to 
	 * get the OAuth keys, keys are returned only if this plugin is 
	 * authorised by Smf franework
	 */
	SmfLyricsServicePlugin( SmfPluginManagerUtil* aUtil );
	
	/**
	 * Destructor
	 */
	~SmfLyricsServicePlugin( );
	
	/**
	 * Method to get the lyrics
	 * @param aRequest [out] The request data to be sent to network
	 * @param aTrack The track whose lyrics need to be fetched
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError lyrics( SmfPluginRequestData &aRequest, 
			const SmfTrackInfo &aTrack,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to get the subtitle
	 * @param aRequest [out] The request data to be sent to network
	 * @param aTrack The track whose subtitle need to be fetched
	 * @param aFilter The subtitle search filter if any
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError subtitles( SmfPluginRequestData &aRequest, 
			const SmfTrackInfo &aTrack,
			const SmfSubtitleSearchFilter &aFilter = SubtitleAll,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	};

Q_DECLARE_INTERFACE( SmfLyricsServicePlugin, "org.symbian.smf.plugin.music.lyrics/v1.0" );

#endif /* SMFLYRICSSERVICEPLUGIN_H_ */
