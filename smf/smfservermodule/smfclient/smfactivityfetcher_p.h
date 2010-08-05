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
 * Private Implementation of SmfActivityFetcher
 * 
 */

#ifndef SMFACTIVITYFETCHER_P_H_
#define SMFACTIVITYFETCHER_P_H_

#include <QList>
#include <smfglobal.h>
#include <smfactions.h>
#include <smfobserver.h>

class SmfActivityFetcher;
class SmfProvider;
class SmfContact;
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
	 */
	SmfActivityFetcherPrivate(SmfActivityFetcher* activityFetcher);
	
	/**
	 * Destructor
	 */
	~SmfActivityFetcherPrivate();
	
	/**
	 * Emits signal resultsAvailable() when list of activities for the user is available 
	 */
	void selfActivities(int pageNum, int perPage);

	/**
	 * Emits signal resultsAvailable() when list of activities for other contact  is available 
	 */
	void friendsActivities(const SmfContact& aFriend, int pageNum, int perPage);
	
	/**
	 * returns only those activities (self) which are from @arg filters
	 */
	void filtered(QList<SmfActivityObjectType> filters, int pageNum, int perPage);
	
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
	
public: //From smfObserver
	/**
	 * To notify availibility of asynchronous requests.
	 * @param result Requested result, before using must check error param.
	 * @param opcode Requested opcode, for which the result has arrived.
	 * @param error Error 
	 */
	void resultsAvailable(QByteArray result,SmfRequestTypeID opcode, SmfError error);
	
private:
	  SmfActivityFetcher* m_activityFetcher;
#ifdef Q_OS_SYMBIAN
	  CSmfClientSymbian* m_SmfClientPrivate;
	  friend class CSmfClientSymbian;
#else
	  SmfClientQt* m_SmfClientPrivate;
	  friend class SmfClientQt;
#endif
	  int m_argFlag;
	  //serialized byte array of provider+other info to be sent to the server
	  //The order:- SmfProvider then params in order of their appearance in fn
	  QByteArray m_serializedDataToServer;

	};

#endif /* SMFACTIVITYFETCHER_P_H_ */
