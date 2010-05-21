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
 */

#ifndef SMFCLIENTPRIVATE_H_
#define SMFCLIENTPRIVATE_H_
/**
 * Private Qt wrapper
 * Provided to accomodate diff facilities in diff platforms.
 * Uses Symbian client-server for Symbian
 * QDbus for linux
 * QLocalserver for rest of the platforms
 * Note:- only symbian client-server impl is provided at the moment
 **/
#include "smfprovider.h"
#include "smfclientglobal.h"
#include "smfglobal.h"
#include "smfobserver.h"

class SmfProvider;


#include <QObject>
#include <QDateTime>
#include <QStringList>

#include "smfobserver.h"
#include "smfcontactfetcher.h"
#include "smfpostprovider.h"

#ifdef Q_OS_SYMBIAN
class CSmfClientSymbian;
#else
class SmfClientQt;
#endif


class SmfContactFetcherPrivate : public smfObserver
{
  Q_OBJECT

public:


  SmfContactFetcherPrivate(SmfContactFetcher* contactFetcher=0);
  ~SmfContactFetcherPrivate();
  //Note we need to expose the methods later through some macro
public:
  /**
   * Get the friend listing asynchronously. The friendsListAvailable() signal
   * is emitted with SmfContactList once data is arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  bool friends(int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE);

  /**
   * Get the list of followers asynchronously. The followersListAvailable() signal
   * is emitted with SmfContactList once data is arrived. Please note that some
   * service may not support followers/fans - FALSE is returned if not supported.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  bool followers(int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE); // list of contact objects

  /**
   * Searches for a contact The searchContactFinished() signal
   * is emitted with SmfContactList once data is arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  void search(SmfContact* contact,int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE) ; // list of contact objects

  /**
   * Searches for a contacts (friends) who are near the user.
   * Signal searchNearFinished() is emitted with SmfContactList once data is arrived.
   * Proximity defines accuracy level
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  //TODO:-implement 
  //bool searchNear(SmfPlace* location,SmfLocationSearchBoundary proximity,int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE) ;


  /**
   * Get the list of groups. The groupListAvailable() signal
   * is emitted with SmfGroupList once data is arrived. False might be returned
   * if this service doesn't support any mode of grouping (very rare).
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
   bool groups(int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE) ; // list of group objects

  /**
   * Searches for Smf Contacts in an Smf group.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   * @param  group The group to be searcged in
   */
   bool searchInGroup(SmfGroup group,int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE) ;
   
   //from smfobserver
   void resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error);
private:
  SmfProvider* m_baseProvider;
  //serialized byte array of provider
  QByteArray m_providerSerialized;
#ifdef Q_OS_SYMBIAN
  CSmfClientSymbian* m_SmfClientPrivate;
  friend class CSmfClientSymbian;
#else
  SmfClientQt* m_SmfClientPrivate;
  friend class SmfClientQt;
#endif
  bool m_connected;
  SmfContactFetcher* m_contactFetcher;
  SmfContactList* m_contactList;
  QList<SmfGroup> m_grpList;
  SmfContactList m_frndist;
  SmfContact* m_contact;
  SmfGroup m_grp;
  void writeLog(QString log) const;
  //debugging
//  SmfLogger* logger;
  //void writeLog(QString log) const;
};

QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfError &err );


QDataStream &operator>>( QDataStream &aDataStream, 
		SmfError &err );
#endif /* SMFCLIENTPRIVATE_H_ */
