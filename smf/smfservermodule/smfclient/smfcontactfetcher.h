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
 * The SmfContactFetcher class is for fetching SmfContact related info
 * 
 */

#ifndef SMFCONTACTHETCHER_H
#define SMFCONTACTHETCHER_H

#include <QObject>
#include <smfglobal.h>
#include <smfcontact.h>
#include <smflocation.h>
#include <smfgroup.h>

// Forward declaration
class SmfProvider; //base-class for service provider
class SmfContactFetcherPrivate;//implementation 

/**
 * @ingroup smf_client_group 
 * Interface to search for contacts/connections from a service provider. This class
 * provides basic functionality to allow applications to obtain list of
 * contacts or friends in a social networking service.
 * Note that to get the base provider info like service name, icon, description etc
 * use getProvider().
 * See also:
 * SmfProvider::serviceName(), SmfProvider::serviceIcon(), SmfProvider::description()
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in object.
 * Interface name:- org.symbian.smf.client.contact.fetcher
 */
class  SMFCLIENT_EXPORT SmfContactFetcher : public QObject
	{
	Q_OBJECT

public:
	/**
	 * Constructs the SmfContactFetcher.
	 * @param parent base provider info
	 * Seeing as this is a plug-in implementation, these will realistically
	 * be generated by SMF factory of some kind
	 */
	SmfContactFetcher( SmfProvider* baseProvider );
	
	/**
	 * Constructs the SmfContactFetcher.
	 * @param parent base provider info
	 * @param contact Used for searching friends of the given contact
	 * Seeing as this is a plug-in implementation, these will realistically
	 * be generated by SMF factory of some kind
	 */
	SmfContactFetcher( SmfProvider* baseProvider, SmfContact* contact );
	
	/**
	 * Destructor
	 */
	~SmfContactFetcher();

public:
	/**
	 * Get the friend listing asynchronously. The friendsListAvailable() signal 
	 * is emitted with SmfContactList once data is arrived. When the list is big,
	 * user can specify the page number and per page item data. If not supplied 
	 * by the user default values are used. 
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 * @return SmfError. SmfNoError if success, else appropriate error code
	 */
	SmfError friends ( int pageNum = SMF_FIRST_PAGE, int perPage = SMF_ITEMS_PER_PAGE );

	/**
	 * Get the list of followers asynchronously. The followersListAvailable() signal
	 * is emitted with SmfContactList once data is arrived. Please note that some
	 * service may not support followers/fans - FALSE is returned if not supported.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 * @return SmfError. SmfNoError if success, else appropriate error code
	 */
	SmfError followers ( int pageNum = SMF_FIRST_PAGE, int perPage = SMF_ITEMS_PER_PAGE );

	/**
	 * Searches for a contact The searchContactFinished() signal
	 * is emitted with SmfContactList once data is arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param contact The contact to be searched. The serach criteria must be 
	 * set as one of its fields.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 * @return SmfError. SmfNoError if success, else appropriate error code
	 */
	SmfError search ( SmfContact* contact, int pageNum = SMF_FIRST_PAGE,
					int perPage = SMF_ITEMS_PER_PAGE );

	/**
	 * Searches for a contacts (friends) who are near the user. The signal 
	 * searchNearFinished() is emitted with SmfContactList once data is arrived.
	 * Proximity defines accuracy level. When the list is big user can specify 
	 * the page number and per page item data. If not supplied by the user 
	 * default values are used.
	 * @param location The location information
	 * @param proximity The search boundary criteria
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 * @return SmfError. SmfNoError if success, else appropriate error code
	 */
	SmfError searchNear ( SmfLocation* location, 
					SmfLocationSearchBoundary proximity,
					int pageNum = SMF_FIRST_PAGE,
					int perPage = SMF_ITEMS_PER_PAGE);

	/**
	 * Get the list of groups. The groupListAvailable() signal is emitted with 
	 * SmfGroupList once data is arrived. False might be returned if this service 
	 * doesn't support any mode of grouping (very rare). When the list is big, 
	 * user can specify the page number and per page item data. If not supplied 
	 * by the user default values are used.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 * @return SmfError. SmfNoError if success, else appropriate error code
	 */
	SmfError groups ( int pageNum = SMF_FIRST_PAGE, int perPage = SMF_ITEMS_PER_PAGE );
	
	/**
	 * Searches for Smf Contacts in an Smf group. The signal searchInGroupFinished() 
	 * is emitted with SmfContactList once data is arrived. When the list is big user 
	 * can specify the page number and per page item data. If not supplied by the 
	 * user default values are used.
	 * @param group The group to be searched in
	 * @contact The contact to be searched, default (NULL) is the self contact.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 * @return SmfError. SmfNoError if success, else appropriate error code
	 */
	SmfError searchInGroup ( SmfGroup group,
						SmfContact* contact = NULL,
						int pageNum = SMF_FIRST_PAGE,
						int perPage = SMF_ITEMS_PER_PAGE );
	
	/**
	 * Request for a custom operation. The signal customDataAvailable() is emitted 
	 * when the result is available.
	 * @param operationId OperationId
	 * @param customData Custom data to be sent
	 * @return SmfError. SmfNoError if success, else appropriate error code
	 * Note:-Interpretation of operationId and customData is upto the concerned
	 * plugin and client application. service provider should provide some
	 * serializing-deserializing utilities for these custom data
	 */
	SmfError customRequest ( const int& operationId, QByteArray* customData );
	
    /**
     * Cancels a request generated due to the call to any API which results 
     * into http request. Might return error if no request is currently pending.
     * Please note that there can be only one request pending at any point of time
     * @return Appropriate SmfError value
     */
	SmfError cancelRequest ();
	
signals:

	/**
	 * This signal is emitted when a request to get friends is completed.
	 * Note if number of friends is large, then it can download the list 
	 * page by page. In that case this signal is emitted multiple times.
	 * @param list list of friends. Ownership of memory is transferred.
	 * @param error error value
	 * @param resultPage Page number info
	 * @see friends()
	 */
	void friendsListAvailable ( SmfContactList* list, 
			SmfError error, SmfResultPage resultPage );

	/**
	 * This signal is emitted when a request to get followers is completed
	 * Note if number of followers is large, then it can download the list 
	 * page by page. In that case this signal is emitted multiple times.
	 * @param list list of followers. Ownership of memory is transferred.
	 * @param error error value
	 * @param resultPage Page number info
	 * @see followers()
	 */
	void followersListAvailable ( SmfContactList* list, 
			SmfError error, SmfResultPage resultPage );
	
	/**
	 * This signal is emitted when search for a contact is finished.
	 * Note if number of contacts in the search is large, then it can download 
	 * the list page by page. In that case this signal is emitted multiple times.
	 * @param list List of filtered contacts. Ownership of memory is transferred.
	 * @param resultPage Page number info
	 * @see search()
	 */
	void searchContactFinished ( SmfContactList* list,
			SmfError error, SmfResultPage resultPage );

	/**
	 * Emitted when search for contacts who are near a geographic location, is finished.
	 * Note if number of contacts in the search is large, then it can download the list page by page
	 * In that case this signal is emitted multiple times.
	 * @param list List of filtered contacts. Ownership of memory is transferred.
	 * @param resultPage Page number info
	 * @see search()
	 */
	void searchNearFinished(SmfContactList* list,SmfError error, SmfResultPage resultPage);

	/**
	 * This signal is emitted when a request to get groups is completed
	 * Note if number of groups is large, then it can download the list 
	 * page by page. In that case this signal is emitted multiple times.
	 * @param list list of groups. Ownership of memory is transferred.
	 * @param error error value
	 * @param resultPage Page number info
	 * @see groups()
	 */
	void groupListAvailable ( SmfGroupList* list, 
			SmfError error, SmfResultPage resultPage );

	/**
	 * Emitted when search for a contact in a group is finished
	 * Note if number of contacts in the search is large, then it can download the list page by page
	 * In that case this signal is emitted multiple times.
	 * @param list list of filtered contacts. Ownership of memory is transferred.
	 * @param resultPage Page number info
	 * @see searchInGroup()
	 */
	void searchInGroupFinished(SmfContactList* list,SmfError error, SmfResultPage resultPage);
	
	/**
	 * Emitted when custom data is available
	 * @param operationId Requested operation id
	 * @param customData Custom data received, interpretation is not the responsibility 
	 * of Smf. Ownership of memory is transferred.
	 */
	void customDataAvailable ( int operationId, QByteArray* customData );
	
private:
	/**
	 * Gets the base provider info
	 */
	SmfProvider* getProvider() const;
	
// Friend Class declaration
	//so that private impl can directly call emit
	friend class SmfContactFetcherPrivate;
	
private:
	SmfProvider* m_baseProvider;
	SmfContact* m_frndContact; 				//used for searching
	SmfContactFetcherPrivate* m_private;	//private impl wrapper

	};

SMF_SERVICE_NAME(SmfContactFetcher, "org.symbian.smf.client.contact.fetcher\0.2")

#endif // SMFCONTACTHETCHER_H
