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
 * Description:
 * Represents activity in terms similar to standard activity stream http://activitystrea.ms/
 */

#ifndef SMFACTIVITYFETCHER_H_
#define SMFACTIVITYFETCHER_H_

#include <QObject>
#include <QList>
#include <qmobilityglobal.h>

#include "smfglobal.h"
#include "smfprovider.h"
#include "smfcontact.h"
#include "smfevent.h"
#include "smfactions.h"

typedef QList<SmfActivityEntry> SmfActivityEntryList;
class SmfActivityFetcherPrivate;
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

	~SmfActivityFetcher();

public slots:
	/**
	 * Emits signal resultsAvailable() when list of activities for the user is available 
	 */
	void selfActivities(int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE);

	/**
	 * Emits signal resultsAvailable() when list of activities for other contact  is available 
	 */
	void friendsActivities(const SmfContact& aFriend,int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE);
	
	/**
	 * returns only those activities (self) which are from @arg filters
	 */
	void filtered(QList<SmfActivityObjectType> filters,int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE);

	  /**
	   * Gets the base provider info
	   */
	  SmfProvider* getProvider()  ;
	
signals:

	/**
	 * Signals availability of the result of the previous query
	 */
	void resultsAvailable(SmfActivityEntryList* entries, SmfError error, SmfResultPage resultPage);
private:
	SmfActivityFetcherPrivate* m_private;
	SmfProvider* m_baseProvider;
	friend class SmfActivityFetcherPrivate;
	};

SMF_SERVICE_NAME(SmfActivityFetcher, "org.symbian.smf.client.activity.fetcher\0.2")

#endif /* SMFACTIVITYFETCHER_H_ */
