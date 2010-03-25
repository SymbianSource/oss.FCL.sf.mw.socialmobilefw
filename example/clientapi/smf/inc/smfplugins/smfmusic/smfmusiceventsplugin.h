/**
 * @file	smfmusiceventsplugin.h
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
 * Interface specification for music events
 * 
 * Note: This class has dependencies on QtMobility project
 */

#ifndef SMFMUSICEVENTSPLUGIN_H_
#define SMFMUSICEVENTSPLUGIN_H_

#include <smfproviderbase.h>
#include <qtcontacts.h>
#include <smfevent.h>
#include <smfvenue.h>

using namespace QtMobility;

/**
 * Interface specification for music events
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 * 
 * Note: This class has dependencies on QtMobility project
 */
class SmfMusicEventsPlugin : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfMusicEventsPlugin( QObject* aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfMusicEventsPlugin( );
	
	/**
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	virtual SmfProviderBase* getProviderInfo( ) = 0;
	
	/**
	 * Method to get the events based on location
	 * @param aLocation Location of the event
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError events( const QtMobility::QContactGeolocation aLocation,
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;
	
	/**
	 * Method to get the venues based on location
	 * @param aLocation Location of the venue
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError venues( const QtMobility::QContactGeolocation aLocation,
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;
	
	/**
	 * Method to get the events based on venues
	 * @param aVenue Venue of the event
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError events( const SmfVenue aVenue,
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;
	
	/**
	 * Method to post events
	 * of posting the events is available
	 * @param aEventList The list of events to be posted
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError postEvents( const QList<SmfEvent> aEventList,
			SmfPluginRequestData *aRequest ) = 0;
	
	
	/**
	 * Method to get the result for a network request.
	 * @param aTransportResult The result of transport operation
	 * @param aReply The QNetworkReply instance for the request
	 * @param aResult [out] An output parameter to the plugin manager.If the 
	 * return value is SmfSendRequestAgain, QVariant will be of type 
	 * SmfPluginRequestData. 
	 * If last operation was events(), aResult will be of type QList<SmfEvent>
	 * If last operation was venues(), aResult will be of type QList<SmfVenue>
	 * If last operation was postEvents(), aResult will be of type bool
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

Q_DECLARE_INTERFACE( SmfMusicEventsPlugin, "org.symbian.smf.plugin.music.events/v1.0" );

#endif /* SMFMUSICEVENTSPLUGIN_H_ */
