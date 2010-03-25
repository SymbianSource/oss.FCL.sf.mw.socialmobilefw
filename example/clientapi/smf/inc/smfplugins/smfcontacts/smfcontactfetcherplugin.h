/**
 * @file	smfcontactfetcherplugin.h
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
 * Interface specification for fetching contacts
 */

#ifndef SMFCONTACTFETCHERPLUGIN_H_
#define SMFCONTACTFETCHERPLUGIN_H_

#include <smfproviderbase.h>
#include <smfcontact.h>
#include <smfgroup.h>

/**
 * Interface specification for fetching contacts. This class provides 
 * basic functionality to allow applications to obtain list of
 * friends, followers, groups of a user in a social networking service.
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 */
class SmfContactFetcherPlugin : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfContactFetcherPlugin( QObject* aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfContactFetcherPlugin( );
	
	/**
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	virtual SmfProviderBase* getProviderInfo( ) = 0;
  
	/**
	 * Method to get the list of friends
	 * @param aRequest [out] The request data to be sent to network
	 * @param aContact to search for friend of a friend, 
	 * for self friend this parameter need not be included
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError friends( SmfPluginRequestData *aRequest,
			const SmfContact aContact = 0,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;
	
	/**
	 * Method to get the list of followers
	 * @param aRequest [out] The request data to be sent to network
	 * @param aContact to search for follower of a friend, for self 
	 * followers this parameter need not be included
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError followers( SmfPluginRequestData *aRequest,
			const SmfContact aContact = 0, 
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;
	
	/**
	 * Method to search for a contact
	 * @param aContact contact to be searched
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError search( const SmfContact &aContact,
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10) = 0;
	
	/**
	 * Method to get the list of groups
	 * @param aRequest [out] The request data to be sent to network
	 * @param aContact to search for groups of a friend, for self 
	 * group this parameter need not be included
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError groups( SmfPluginRequestData *aRequest,
			const SmfContact aContact = 0, 
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;
	
	/**
	 * Method to search for a contact in a group
	 * @param aGroup the group in which to search
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError searchInGroup( const SmfGroup &aGroup,
			SmfPluginRequestData *aRequest,
			const int aPageNum = 0, 
			const int aItemsPerPage = 10 ) = 0;

	/**
	 * Method to get the result for a network request.
	 * @param aTransportResult The result of transport operation
	 * @param aReply The QNetworkReply instance for the request
	 * @param aResult [out] An output parameter to the plugin manager.If the 
	 * return value is SmfSendRequestAgain, QVariant will be of type 
	 * SmfPluginRequestData. 
	 * If last operation was friends() or followers() or search() or 
	 * searchInGroup(), aResult will be of type QList<SmfContact>
	 * If last operation was groups(), aResult will be of type QList<SmfGroup>
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

Q_DECLARE_INTERFACE( SmfContactFetcherPlugin, "org.symbian.smf.plugin.contact.fetcher/v1.0" );

#endif /* SMFCONTACTFETCHERPLUGIN_H_ */
