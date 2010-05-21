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
 * 
 *
 */
#ifndef SMFPOSTPROVIDER_P_H_
#define SMFPOSTPROVIDER_P_H_

#include "smfprovider.h"
#include "smfclientglobal.h"
#include "smfglobal.h"
#include "smfobserver.h"

class SmfProvider;
class SmfPostProvider;

#include <QObject>
#include <QDateTime>
#include <QStringList>

#include "smfobserver.h"
#include "smfpostprovider.h"
#ifdef Q_OS_SYMBIAN
class CSmfClientSymbian;
#else
class SmfClientQt;
#endif

/**
 * Data size limits
 */
/**
 * Single Post-1 MB
 */
const int KSmfPostMaxSize = 1000000;
/**
 * Private Qt wrapper
 * Provided to accomodate diff facilities in diff platforms.
 * Uses Symbian client-server for Symbian
 * QDbus for linux
 * QLocalserver for rest of the platforms
 * Note:- only symbian client-server impl is provided at the moment
 **/
class SmfPostProviderPrivate: public smfObserver
	{
	Q_OBJECT

public:

	SmfPostProviderPrivate(SmfPostProvider* postProvider=0);
	~SmfPostProviderPrivate();
	/**
	 * From smfobserver
	 */
	  void resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error);
	/**
	 * Gets the posts asynchronously. The signal postsAvailable()with SmfPostList is emitted
	 * once the post lists are available
	 * @param user user's contact in this SP, omit for self contact
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
     * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE 
	 * @see postsAvailable()
	 */
	void posts(SmfContact* user = 0,int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE);
	
	/**
	 * Updates a post to own area, the success of the post can be checked with signal
	 * updatePostFinished() signal
	 * @param postData data to be posted
	 * @param location location data
	 */
	void post(SmfPost& postData,SmfLocation& location) ; 
	/**
	 * Updates the last post to own area with new data, the success of the post can be checked with signal
	 * updatePostFinished() signal
	 * @param postData edited/new data to be posted
	 * @param location location data
	 */
	void updatePost(SmfPost& postData); 
	
	/**
	 * Updates a post to a particular Smf contact. the success of the post can be checked with signal
	 * updatePostFinished() signal.
	 * @param postData data to be posted
	 * @param contact contact to which the post is to be directed
	 * @param location location data
	 */
	void postDirected(SmfPost& postData,SmfContact& contact,SmfLocation* location=0);
	
	/**
	 * Posts appearance info of the user.e.g. appear offline, busy, do-not-disturb
	 * @param appearence user appearance
	 * @see SmfPresenceInfo
	 * @return False on Failure/Not supported 
	 */
	bool postAppearence(SmfAppearenceInfo appearence); 
	/**
	 * Share /a contact's post to user's friends and followers (e.g. retweet in twitter, share on facebook)
	 * emits updatePostFinished() signal when done.
	 * @param postData data to be posted
	 * @param contact contact to which the post belonged
	 * @param bool whether user changed items within the post
	 */
	void sharePost(SmfPost& postData,SmfContact& contact,bool edited);

	private:
	  SmfProvider* m_baseProvider;
	  //serialized byte array of provider+other info to be sent to the server
	  //The order:- SmfProvider then params in order of their appearance in fn
	  QByteArray m_serializedDataToServer;
	#ifdef Q_OS_SYMBIAN
	  CSmfClientSymbian* m_SmfClientPrivate;
	  friend class CSmfClientSymbian;
	#else
	  SmfClientQt* m_SmfClientPrivate;
	  friend class SmfClientQt;
	#endif
	  bool m_connected;
	  SmfPostProvider* m_postProvider;
	  SmfPostList* m_postList;
	 void writeLog(QString log) const;
	};
#endif /* SMFPOSTPROVIDER_P_H_ */
