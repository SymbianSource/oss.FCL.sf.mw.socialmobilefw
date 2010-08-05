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
 * Represents activity in terms similar to standard activity stream http://activitystrea.ms/
 * 
 */

#ifndef SMFACTIVITYFETCHER_H_
#define SMFACTIVITYFETCHER_H_

#include <QList>
#include <smfglobal.h>
#include <smfactions.h>

// Forward declaration
class SmfActivityFetcherPrivate;
class SmfProvider;
class SmfContact;


/**
 * @ingroup smf_client_group
 * Basic activity service ("org.symbian.smf.client.activity.fetcher")
 */
class SMFCLIENT_EXPORT SmfActivityFetcher : public QObject
	{
	Q_OBJECT

public:
	/**
	 * Constructs SmfActivity.
	 * @param baseProvider The base provider info
	 */
	SmfActivityFetcher(SmfProvider* baseprovider = 0);

	/**
	 * Destructor
	 */
	~SmfActivityFetcher();

public slots:
	/**
	 * Emits signal resultsAvailable() when list of activities for the user is available 
	 */
	void selfActivities(int pageNum = SMF_FIRST_PAGE,int perPage = SMF_ITEMS_PER_PAGE);

	/**
	 * Emits signal resultsAvailable() when list of activities for other contact  is available 
	 */
	void friendsActivities(const SmfContact& aFriend, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * returns only those activities (self) which are from @arg filters
	 */
	void filtered(QList<SmfActivityObjectType> filters, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Request for a custom operation. The signal customDataAvailable() is emitted 
	 * when the result is available.
	 * @param operationId OperationId
	 * @param customData Custom data to be sent
	 * Note:-Interpretation of operationId and customData is upto the concerned
	 * plugin and client application. service provider should provide some
	 * serializing-deserializing utilities for these custom data
	 */
	void customRequest ( const int& operationId, QByteArray* customData );

signals:
	/**
	 * Signals availability of the result of the previous query
	 */
	void resultsAvailable(SmfActivityEntryList* entries, SmfError error, SmfResultPage resultPage);
	
	/**
	 * Signals availability of the result of the customRequest
	 */
	void customDataAvailable( const int& operationId, QByteArray* customData );

private:
	/**
	 * Gets the base provider info
	 */
	SmfProvider* getProvider() const;
	  
// Friend Class
	friend class SmfActivityFetcherPrivate;
	  
private:
	SmfActivityFetcherPrivate* m_private;
	SmfProvider* m_baseProvider;

	};

SMF_SERVICE_NAME(SmfActivityFetcher, "org.symbian.smf.client.activity.fetcher\0.2")

#endif /* SMFACTIVITYFETCHER_H_ */
