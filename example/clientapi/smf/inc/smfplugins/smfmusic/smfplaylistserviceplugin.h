/**
 * @file	smfplaylistserviceplugin.h
 * @author  Nalina Hariharan, Sasken Communication Technologies Ltd - Initial contribution
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2010 Sasken Communication Technologies Ltd. 
 * All rights reserved.
 * This component and the accompanying materials are made available 
 * under the terms of the "{License}" 
 * which accompanies  this distribution, and is available 
 * at the URL "{LicenseUrl}".
 * 
 * @section DESCRIPTION
 *
 * Interface specification for playlists service
 */

#ifndef SMFPLAYLISTSERVICEPLUGIN_H_
#define SMFPLAYLISTSERVICEPLUGIN_H_

#include <smfproviderbase.h>
#include <smfplaylist.h>
#include <smfmusicprofile.h>
#include <smftrackinfo.h>

/**
 * Interface specification for playlists service. This class provides 
 * basic functionality to allow applications to get playlists of a user, 
 * add some track to an existing playlist, post the current playing 
 * playlists etc.
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 */
class SmfPlaylistServicePlugin : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfPlaylistServicePlugin( QObject *aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfPlaylistServicePlugin( );
	
	/**
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	virtual SmfProviderBase* getProviderInfo( ) = 0;
	
	/**
	 * Method to get the playlist
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError playlists( 
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10) = 0;
	
	/**
	 * Method to get the playlist of a particular user
	 * @param aUser The user whose playlists need to be fetched
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError playlistsOf( const SmfMusicProfile aUser,
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10) = 0;
	
	/**
	 * Method to add tracks to a playlist
	 * @param aPlaylist The playlist where tracks should be added
	 * @param aTracks The tracks to be added to the playlist
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError* addToPlaylist( const SmfPlaylist aPlaylist, 
			const QList<SmfTrackInfo> aTracks,
			SmfPluginRequestData *aRequest ) = 0;
	
	/**
	 * Method to post the current playing playlist
	 * @param aPlaylist The current playing playlist which should be posted
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError* postCurrentPlayingPlaylist( 
			const SmfPlaylist aPlaylist,
			SmfPluginRequestData *aRequest ) = 0;
	
	
	/**
	 * Method to get the result for a network request.
	 * @param aTransportResult The result of transport operation
	 * @param aReply The QNetworkReply instance for the request
	 * @param aResult An output parameter to the plugin manager.If the 
	 * return value is SmfSendRequestAgain, QVariant will be of type 
	 * SmfPluginRequestData. 
	 * If last operation was playlists() or playlistsOf(), aResult will be of 
	 * type QList<SmfPlaylist>
	 * If last operation was addToPlaylist() or postCurrentPlayingPlaylist(), 
	 * aResult will be of type bool
	 * @return SmfPluginRetType
	 */
	/**
	 * Method to get the result for a network request.
	 * @param aTransportResult The result of transport operation
	 * @param aReply The QNetworkReply instance for the request
	 * @param aResult [out] An output parameter to the plugin manager.If the 
	 * return value is SmfSendRequestAgain, QVariant will be of type 
	 * SmfPluginRequestData. 
	 * If last operation was playlists() or playlistsOf(), aResult will be of 
	 * type QList<SmfPlaylist>
	 * If last operation was addToPlaylist() or postCurrentPlayingPlaylist(), 
	 * aResult will be of type bool
	 * @param aRetType [out] SmfPluginRetType
	 * @param aIsLastPage [out] true if this the last page, else false
	 * @return SmfPluginError 
	 */
	virtual SmfPluginError responseAvailable( 
			const SmfTransportResult aTransportResult, 
			QNetworkReply *aReply, 
			QVariant* aResult, 
			SmfPluginRetType aRetType,
			bool aIsLastPage) = 0;
	
	};

Q_DECLARE_INTERFACE( SmfPlaylistServicePlugin, "org.symbian.smf.plugin.music.playlist/v1.0" );

#endif /* SMFPLAYLISTSERVICEPLUGIN_H_ */
