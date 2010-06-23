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
 * Interface specification for playlists service
 *
 */


#ifndef SMFPLAYLISTSERVICEPLUGIN_H_
#define SMFPLAYLISTSERVICEPLUGIN_H_

#include <smfpluginbase.h>
#include <smfplaylist.h>
#include <smfmusicprofile.h>
#include <smftrackinfo.h>

/**
 * @ingroup smf_plugin_group
 * Interface specification for playlists service. This class provides 
 * basic functionality to allow applications to get playlists of a user, 
 * add some track to an existing playlist, post the current playing 
 * playlists etc.
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 */
class SmfPlaylistServicePlugin : public SmfPluginBase
	{
public:
	/**
	 * Destructor
	 */
	virtual ~SmfPlaylistServicePlugin( ) {}
	
	/**
	 * Method to get the playlist
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError playlists( SmfPluginRequestData &aRequest,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to get the playlist of a particular user
	 * @param aRequest [out] The request data to be sent to network
	 * @param aUser The user whose playlists need to be fetched
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError playlistsOf( SmfPluginRequestData &aRequest,
			const SmfMusicProfile &aUser,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to add tracks to a playlist
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPlaylist The playlist where tracks should be added
	 * @param aTracks The tracks to be added to the playlist
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError addToPlaylist( SmfPluginRequestData &aRequest,
			const SmfPlaylist &aPlaylist, 
			const QList<SmfTrackInfo> &aTracks ) = 0;
	
	/**
	 * Method to post the current playing playlist
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPlaylist The current playing playlist which should be posted
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError postCurrentPlayingPlaylist(
			SmfPluginRequestData &aRequest, 
			const SmfPlaylist &aPlaylist ) = 0;
	
	/**
	 * Customised method for SmfPlaylistServicePlugin interface
	 * @param aRequest [out] The request data to be sent to network
	 * @param aOperation The operation type (should be known between 
	 * the client interface and the plugin)
	 * @param aData The data required to form the request (The type 
	 * of data should be known between client and the plugin)
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError customRequest( SmfPluginRequestData &aRequest, 
			const int &aOperation, QByteArray *aData ) = 0;
	
	};

Q_DECLARE_INTERFACE( SmfPlaylistServicePlugin, "org.symbian.smf.plugin.music.playlist/v0.2" );

#endif /* SMFPLAYLISTSERVICEPLUGIN_H_ */
