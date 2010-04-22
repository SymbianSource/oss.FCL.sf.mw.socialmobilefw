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
 * Interface specification for music search services
 *
 */


#ifndef SMFMUSICSEARCHPLUGIN_H_
#define SMFMUSICSEARCHPLUGIN_H_

#include <smfpluginbase.h>
#include <QList>
#include <smfmusicfingerprint.h>
#include <smftrackinfo.h>

// Forward declaration
class SmfPluginUtil;

/**
 * @ingroup smf_plugin_group
 * Interface specification for music search services. This class
 * provides basic functionality to allow applications to search for 
 * tracks, get recommented tracks etc
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 */
class SmfMusicSearchPlugin : public SmfPluginBase
	{
public:
	/**
	 * Constructor with default argument
	 * @param aUtil The SmfPluginUtil instance. The plugins can 
	 * call the method getAuthKeys() of this class, with its pluginID to 
	 * get the OAuth keys, keys are returned only if this plugin is 
	 * authorised by Smf franework
	 */
	SmfMusicSearchPlugin( SmfPluginUtil *aUtil );
	
	/**
	 * Destructor
	 */
	~SmfMusicSearchPlugin( );
	
	/**
	 * Method to get recommended tracks
	 * @param aRequest [out] The request data to be sent to network
	 * @param aTrack The track for which similar recommendations 
	 * need to be fetched.
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError recommendations( SmfPluginRequestData &aRequest,
			const SmfTrackInfo &aTrack,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to get similar tracks
	 * @param aRequest [out] The request data to be sent to network
	 * @param aTrack The track for which similar tracks 
	 * need to be fetched.
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError tracks( SmfPluginRequestData &aRequest,
			const SmfTrackInfo &aTrack,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to get tracks having a similar finger print
	 * @param aRequest [out] The request data to be sent to network
	 * @param aSignature The finger print to be searched for need to be 
	 * fetched.
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError trackInfo( SmfPluginRequestData &aRequest,
			const SmfMusicFingerPrint &aSignature,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to search information about where to buy this song from
	 * @param aRequest [out] The request data to be sent to network
	 * @param aTrack The track for which stores need to be searched
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError stores( SmfPluginRequestData &aRequest,
			const SmfTrackInfo &aTrack,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to post the currently playing track
	 * @param aRequest [out] The request data to be sent to network
	 * @param aTrack The current playing track, that should be posted
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError postCurrentPlaying( 
			SmfPluginRequestData &aRequest,
			const SmfTrackInfo &aTrack ) = 0;
	
	};

Q_DECLARE_INTERFACE( SmfMusicSearchPlugin, "org.symbian.smf.plugin.music.search/v1.0" );

#endif /* SMFMUSICSEARCHPLUGIN_H_ */
