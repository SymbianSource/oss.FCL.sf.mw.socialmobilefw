/**
 * @file	smfmusicsearchplugin.h
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
 * Interface specification for music search services
 */

#ifndef SMFMUSICSEARCHPLUGIN_H_
#define SMFMUSICSEARCHPLUGIN_H_

#include <smfproviderbase.h>
#include <QList>
#include <smfmusicfingerprint.h>
#include <smftrackinfo.h>

/**
 * Interface specification for music search services. This class
 * provides basic functionality to allow applications to search for 
 * tracks, get recommented tracks etc
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 */
class SmfMusicSearchPlugin : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfMusicSearchPlugin( QObject *aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfMusicSearchPlugin( );
	
	/**
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	virtual SmfProviderBase* getProviderInfo( ) = 0;
	
	/**
	 * Method to get recommended tracks
	 * @param aTrack The track for which similar recommendations 
	 * need to be fetched.
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError recommendations( const SmfTrackInfo aTrack,
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;
	
	/**
	 * Method to get similar tracks
	 * @param aTrack The track for which similar tracks 
	 * need to be fetched.
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError tracks( const SmfTrackInfo aTrack, 
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10) = 0;
	
	/**
	 * Method to get tracks having a similar finger print
	 * @param aSignature The finger print to be searched for need to be 
	 * fetched.
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError trackInfo( const SmfMusicFingerPrint aSignature, 
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;
	
	/**
	 * Method to search information about where to buy this song from
	 * @param aTrack The track for which stores need to be searched
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError stores( const SmfTrackInfo aTrack, 
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;
	
	/**
	 * Method to post the currently playing track
	 * @param aTrack The current playing track, that should be posted
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError postCurrentPlaying( const SmfTrackInfo aTrack,
			SmfPluginRequestData *aRequest ) = 0;
	

	/**
	 * Method to get the result for a network request.
	 * @param aTransportResult The result of transport operation
	 * @param aReply The QNetworkReply instance for the request
	 * @param aResult [out] An output parameter to the plugin manager.If the 
	 * return value is SmfSendRequestAgain, QVariant will be of type 
	 * SmfPluginRequestData. 
	 * If last operation was recommendations()or tracks() or trackInfo(), 
	 * aResult will be of type QList<SmfTrackInfo>
	 * If last operation was postCurrentPlaying(), aResult will be of type bool
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

Q_DECLARE_INTERFACE( SmfMusicSearchPlugin, "org.symbian.smf.plugin.music.search/v1.0" );

#endif /* SMFMUSICSEARCHPLUGIN_H_ */
