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
 * under the terms of the "{License}" 
 * which accompanies  this distribution, and is available 
 * at the URL "{LicenseUrl}".
 * 
 * @section DESCRIPTION
 *
 * Interface specification for music track lyrics
 */

#ifndef SMFLYRICSSERVICEPLUGIN_H_
#define SMFLYRICSSERVICEPLUGIN_H_

#include <smfproviderbase.h>
#include <smftrackinfo.h>
#include <QString>
#include <smfsubtitle.h>
#include <smflyrics.h>
#include <smfsubtitlesearchfilter.h>

/**
 * Interface specification for music track lyrics
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 */
class SmfLyricsServicePlugin : public QObject
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
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	virtual SmfProviderBase* getProviderInfo( ) = 0;
	
	/**
	 * Method to get the lyrics
	 * @param aTrack The track whose lyrics need to be fetched
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError lyrics( const SmfTrackInfo aTrack,
			SmfPluginRequestData *aRequest ) = 0;
	
	/**
	 * Method to get the subtitle
	 * @param aTrack The track whose subtitle need to be fetched
	 * @param aRequest [out] The request data to be sent to network
	 * @param aFilter The subtitle search filter if any
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError subtitles( const SmfTrackInfo aTrack,
			SmfPluginRequestData *aRequest,
			const SmfSubtitleSearchFilter aFilter = 0 ) = 0;
	
	/**
	 * Method to get the result for a network request.
	 * @param aTransportResult The result of transport operation
	 * @param aReply The QNetworkReply instance for the request
	 * @param aResult [out] An output parameter to the plugin manager.If the 
	 * return value is SmfSendRequestAgain, QVariant will be of type 
	 * SmfPluginRequestData. 
	 * If last operation was lyrics(), aResult will be of type SmfLyrics
	 * If last operation was subtitles(), aResult will be of type SmfSubtitle
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

Q_DECLARE_INTERFACE( SmfLyricsServicePlugin, "org.symbian.smf.plugin.music.lyrics/v1.0" );

#endif /* SMFLYRICSSERVICEPLUGIN_H_ */
