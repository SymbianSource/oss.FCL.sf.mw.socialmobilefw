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
 * Nalina Hariharan
 *
 * Description:
 * Events related services
 *
 */

#ifndef SMFEVENTSFETCHER_H_
#define SMFEVENTSFETCHER_H_

#include <smfglobal.h>
#include <smfevent.h>
#include <smflocation.h>
#include <smfprovider.h>
#include <smfcontact.h>

class SmfEventsFetcherPrivate;

using namespace QtMobility;

/**
 * @ingroup smf_client_group
 * provides service ("org.symbian.smf.client.events.fetcher")
 */
class SMFCLIENT_EXPORT SmfEventsFetcher : public QObject
	{
	Q_OBJECT
	
public:
	/**
	 * Constructs SmfEventsFetcher with base provider info
	 * Seeing as this is a plug-in implementation, these will realistically
	 * be generated by SMF factory of some kind
	 */
	SmfEventsFetcher ( SmfProvider* baseProvider = 0 );
	
	/**
	 * Destructor
	 */
	~SmfEventsFetcher ( );
	
public:
	/**
	 * Gets list of events in a particular venue asynchronously.
	 * eventsAvailable() signal is emitted with SmfEventsList once its arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 * @return SmfNoError if success, else appropriate error value
	 */
	SmfError events ( SmfLocation venue, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Gets list of venues of a particular location asynchronously.
	 * venuesAvailable() signal is emitted with SmfLocationList once its arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 * @return SmfNoError if success, else appropriate error value 
	 */
	SmfError venues ( SmfLocation location, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	
	/**
	 * Gets a list of attendees to the event
	 * attendeesAvailable() signal is emitted with SmfContacts for the users attending this event
	 * @param event 
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 * @return SmfNoError if success, else appropriate error value
	 */
	SmfError attendees(SmfEvent event, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE); 
	
public slots:
	/**
	 * Updates events. Might not be supported by all service provider.
	 * eventsUpdated() signal can be checked for success value.
	 * @param SmfEventsList List of events to be posted
	 * @return SmfNoError if success, else appropriate error value
	 */
	SmfError postEvents ( SmfEventList events );
	
	/**
	 * Request for a custom operation. The signal customDataAvailable() is emitted 
	 * when the result is available.
	 * @param operationId OperationId
	 * @param customData Custom data to be sent
	 * @return SmfNoError if success, else appropriate error value
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
	 * Notification on arrival of event lists
	 * Note if number of list is large, then it can download the list page by page.
	 * @param list List of events
	 * @param error SmfNoError if success, else appropriate error value
	 * @param resultPage Page number info
	 */
	void eventsAvailable ( SmfEventList* list, 
			SmfError error, SmfResultPage resultPage );
	
	/**
	 * Notification on arrival of venues lists
	 * Note if number of list is large, then it can download the list page by page.
	 * @param list List of venues
	 * @param error SmfNoError if success, else appropriate error value
	 * @param resultPage Page number info
	 */
	void venuesAvailable ( SmfLocationList* list, 
			SmfError error, SmfResultPage resultPage );
	
	/**
	 * Notification on arrival of attendees lists
	 * Note if number of list is large, then it can download the list page by page.
	 * @param list List of attendees
	 * @param error SmfNoError if success, else appropriate error value
	 * @param resultPage Page number info
	 */
	void attendeesAvailable ( SmfContactList* list, 
			SmfError error, SmfResultPage resultPage );

	/**
	 * Notification of the success of request to post an event
	 * @param error SmfNoError if success, else appropriate error value
	 */
	void eventsUpdated ( SmfError success );
	
	/**
	 * Signals availability of the result of the customRequest
	 * @param operationId The ID to be shared between clients and the plugin
	 * @param customData data
	 */
	void customDataAvailable( const int& operationId, QByteArray* customData );
	
private:
	/**
	 * Gets the base provider info
	 */
	SmfProvider* getProvider() const;
	
private:
	SmfProvider* m_baseProvider;
	SmfEventsFetcherPrivate* m_private;
	friend class SmfEventsFetcherPrivate;
	
	};

SMF_SERVICE_NAME(SmfEventsFetcher, "org.symbian.smf.client.events.fetcher\0.2")


#endif /* SMFEVENTSFETCHER_H_ */
