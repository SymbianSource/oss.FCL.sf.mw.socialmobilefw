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
 * 
 */

#ifndef SMFACTIVITYFETCHER_P_H_
#define SMFACTIVITYFETCHER_P_H_

#include <QObject>
#include <QList>

#include "smfactivityfetcher.h"
#include "smfglobal.h"
#include "smfprovider.h"
#include "smfcontact.h"
#include "smfevent.h"
#include "smfactions.h"
#include "smfobserver.h"

#ifdef Q_OS_SYMBIAN
class CSmfClientSymbian;	
#else
class SmfClientQt;
#endif

class SmfActivityFetcherPrivate : public smfObserver
	{
	Q_OBJECT

public:

	/**
	 * Constructs SmfActivityFetcherPrivate.
	 * 
	 */
	SmfActivityFetcherPrivate(SmfActivityFetcher* activityFetcher);

	~SmfActivityFetcherPrivate();
	/**
	 * Emits signal resultsAvailable() when list of activities for the user is available 
	 */
	void selfActivities(int pageNum,int perPage);

	/**
	 * Emits signal resultsAvailable() when list of activities for other contact  is available 
	 */
	void friendsActivities(const SmfContact& aFriend,int pageNum,int perPage);
	
	/**
	 * returns only those activities (self) which are from @arg filters
	 */
	void filtered(QList<SmfActivityObjectType> filters,int pageNum,int perPage);
	/**
	 * From smfObserver.
	 * To notify availibility of asynchronous requests.
	 * @param result Requested result, before using must check error param.
	 * @param opcode Requested opcode, for which the result has arrived.
	 * 
	 */
	void resultsAvailable(QByteArray result,SmfRequestTypeID opcode, SmfError error);
private:
	  SmfActivityFetcher* m_activityFetcher;
	  SmfActivityEntryList* m_entries;
#ifdef Q_OS_SYMBIAN
	  CSmfClientSymbian* m_SmfClientPrivate;
	  friend class CSmfClientSymbian;
#else
	  SmfClientQt* m_SmfClientPrivate;
	  friend class SmfClientQt;
#endif
	  int m_xtraInfoFlag;
	  int m_pageInfoFlag;
	  SmfProvider* m_baseProvider;
	  //serialized byte array of provider+other info to be sent to the server
	  //The order:- SmfProvider then params in order of their appearance in fn
	  QByteArray m_serializedDataToServer;
	  //serialized xtra info, order of serialization follows order of param
	  QByteArray m_xtraInfoSerialized;
	};

#endif /* SMFACTIVITYFETCHER_P_H_ */
