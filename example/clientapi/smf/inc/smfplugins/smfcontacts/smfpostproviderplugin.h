/**
 * @file	smfpostproviderplugin.h
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
 * Interface specification for getting/posting updates to a social site
 */

#ifndef SMFPOSTPROVIDERPLUGIN_H_
#define SMFPOSTPROVIDERPLUGIN_H_

#include <smfproviderbase.h>
#include <smfcontact.h>
#include <smfpost.h>
#include <smfstatusdata.h>


/**
 * SmfPresenceInfo.
 * Indicates the presence information of user like Online, Offline, Busy, 
 * Do no disturb, Appear Offline etc.
 */
enum SmfPresenceInfo
	{
	  ENotSupported,
	  EOnline,
	  EOffline,
	  EBusy,
	  EDoNotDisturb,
	  EAppearOffline,
	  EOther
	};

/**
 * Interface to get/update the posts to a service provider. This class
 * provides basic functionality to allow applications to obtain list of
 * posts, updates posts or change the presence information to a social 
 * networking service.
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 */
class SmfPostProviderPlugin : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfPostProviderPlugin( QObject* aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfPostProviderPlugin( );
	
	/**
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	virtual SmfProviderBase* getProviderInfo( ) = 0;
	
	/**
	 * Method to get the latest posts
	 * @param aStatusData
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError getPosts( const SmfStatusData aStatusData,
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;

	/**
	 * Method to update a post
	 * @param aPostData The post data to be posted
	 * @param aLocation The location
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError updatePost( const SmfPost aPostData, 
			const QtMobility::QContactGeolocation aLocation,
			SmfPluginRequestData *aRequest ) = 0;
	
	/**
	 * Method to update a post to a particular contact
	 * @param aPostData The post data to be posted
	 * @param aContact The contact where the data has to be posted
	 * @param aLocation The location
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError updatePostDirected( const SmfPost aPostData, 
			const SmfContact aContact, 
			const QtMobility::QContactGeolocation aLocation,
			SmfPluginRequestData *aRequest ) = 0;
	/**
	 * Method to update the presence information of the user
	 * @param aAppearence The appearence information
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError postAppearence( const SmfPresenceInfo aAppearence,
			SmfPluginRequestData *aRequest ) = 0;
	
	/**
	 * Method to get the result for a network request.
	 * @param aTransportResult The result of transport operation
	 * @param aReply The QNetworkReply instance for the request
	 * @param aResult [out] An output parameter to the plugin manager.If the 
	 * return value is SmfSendRequestAgain, QVariant will be of type 
	 * SmfPluginRequestData. 
	 * If last operation was getPosts(), aResult will be of type QList<SmfPost>
	 * If last operation was updatePost() or updatePostDirected() or 
	 * postAppearence, aResult will be of type bool
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

Q_DECLARE_INTERFACE( SmfPostProviderPlugin, "org.symbian.smf.plugin.contact.posts/v1.0" );

#endif /* SMFPOSTPROVIDERPLUGIN_H_ */
