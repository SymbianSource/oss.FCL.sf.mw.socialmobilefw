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
#include "smfcontactfetcher_p.h"
#ifdef WRITE_LOG
#include <QFile>
#include <QTextstream>
#endif
#ifdef Q_OS_SYMBIAN
#include "SmfClientSymbian.h"
#else
#include "SmfClientQt.h"
#endif
 /**
   * Constructs the SmfContactFetcher.
   * @param parent base provider info
   * @param contact Used for searching friends of the given contact
   * Seeing as this is a plug-in implementation, these will realistically
   * be generated by SMF factory of some kind
   */
  SmfContactFetcherPrivate::SmfContactFetcherPrivate(SmfContactFetcher* contactFetcher)
  : m_contactFetcher(contactFetcher),m_contactList(0)
	  {
	  //private impl for symbian
	#ifdef Q_OS_SYMBIAN
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
	#endif
	  }
  SmfContactFetcherPrivate::~SmfContactFetcherPrivate()
	  {

	  }

  /**
   * Get the friend listing asynchronously. The friendsListAvailable() signal
   * is emitted with SmfContactList once data is arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  bool  SmfContactFetcherPrivate::friends(int pageNum,int perPage)
	  {
	  m_providerSerialized.clear();
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  
	  m_baseProvider = m_contactFetcher->getProvider();
	  //serialize start
	  QDataStream write(&m_providerSerialized,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  //serialize end
	  
	  
	  QString intfName(contactFetcherInterface);
	  
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_providerSerialized,intfName,SmfContactGetFriends);
	  }

  /**
   * Get the list of followers asynchronously. The followersListAvailable() signal
   * is emitted with SmfContactList once data is arrived. Please note that some
   * service may not support followers/fans - FALSE is returned if not supported.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  bool SmfContactFetcherPrivate::followers(int pageNum,int perPage)
	  {
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  
	  m_baseProvider = m_contactFetcher->getProvider();
	  //serialize start
	  QDataStream write(&m_providerSerialized,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  //serialize end
	  
	  
	  QString intfName(contactFetcherInterface);
	  
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_providerSerialized,intfName,SmfContactGetFollowers);
	  }
  /**
   * Searches for a contact The searchContactFinished() signal
   * is emitted with SmfContactList once data is arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  void  SmfContactFetcherPrivate::search(SmfContact* contact,int pageNum,int perPage) 
	  {
	  //We need to pass Opcode and SmfProvider+SmfContact serialized into bytearray 
	  
	  m_baseProvider = m_contactFetcher->getProvider();
	  m_contact = contact;
	  //serialize start
	  QDataStream write(&m_providerSerialized,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  //now serialize SmfContact 
	  write<<*(m_contact);
	  //serialize end
	  
	  
	  QString intfName(contactFetcherInterface);
	  
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_providerSerialized,intfName,SmfContactSearch);
	  }

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
//   bool  SmfContactFetcherPrivate::searchNear(SmfPlace* location,SmfLocationSearchBoundary proximity,int pageNum,int perPage) 
//	   {
//	   
//	   }


  /**
   * Get the list of groups. The groupListAvailable() signal
   * is emitted with SmfGroupList once data is arrived. False might be returned
   * if this service doesn't support any mode of grouping (very rare).
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
   bool SmfContactFetcherPrivate::groups(int pageNum,int perPage) 
	   {
		  //We need to pass Opcode and SmfProvider serialized into bytearray 
		  
		  m_baseProvider = m_contactFetcher->getProvider();
		  //serialize start
		  QDataStream write(&m_providerSerialized,QIODevice::WriteOnly);
		  write<<*(m_baseProvider);
		  //serialize end
		  
		  
		  QString intfName(contactFetcherInterface);
		  
		  //call private impl's send method
		  m_SmfClientPrivate->sendRequest(m_providerSerialized,intfName,SmfContactGetGroups);
	   }

  /**
   * Searches for Smf Contacts in an Smf group.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   * @param  group The group to be searcged in
   */
bool  SmfContactFetcherPrivate::searchInGroup(SmfGroup group,int pageNum,int perPage) 
   {
	  //We need to pass Opcode and SmfProvider+SmfGroup serialized into bytearray 
	  
	  m_baseProvider = m_contactFetcher->getProvider();
	  m_grp = group;
	  //serialize start
	  QDataStream write(&m_providerSerialized,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  //now serialize SmfGroup 
	  write<<m_grp;
	  //serialize end
	  
	  
	  QString intfName(contactFetcherInterface);
	  
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_providerSerialized,intfName,SmfContactSearch);
   }


void SmfContactFetcherPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error)
   	{
	writeLog("SmfContactFetcherPrivate::resultsAvailable");
	
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//TODO:- order of serialization Error value followed by data
	
   	QDataStream reader(&result,QIODevice::ReadOnly);
   	
   	//Now de-serialize it based on opcode
   	switch(opcode)
   		{
   		case SmfContactGetFriendsComplete:
   			{

   			if(m_contactList)
   				{
				delete m_contactList;
				m_contactList = NULL;
   				}
   			m_contactList = new SmfContactList;
   			//TODO
//   			SmfError error;
//   			reader>>error;
//   			writeLog("Error=");
//   			writeLog(QString::number(error));
//   			SmfError err = (SmfError)error;
   			m_frndist.clear();
   			reader>>m_frndist;
   			writeLog("m_frndist.count=");
   			writeLog(QString::number(m_frndist.count()));
   			//not incorporating paging now
   			SmfResultPage page;

   			emit m_contactFetcher->friendsListAvailable(&m_frndist,SmfNoError,page);
   			
   			}
   			break;
   		case SmfContactGetFollowersComplete:
   			{
   			if(m_contactList)
   				{
				delete m_contactList;
				m_contactList = NULL;
   				}
   			m_contactList = new SmfContactList;
   			quint32 error;
   			reader>>error;
   			SmfError err = (SmfError)error;
   			reader>>*(m_contactList);
   			//not incorporating paging now
   			SmfResultPage page;

   			emit m_contactFetcher->followersListAvailable(m_contactList,err,page);
			
   			}
   			break;
   		case SmfContactGetGroupsComplete:
   			{
   			writeLog("Before m_grpList.clear=");
   			m_grpList.clear();
   			writeLog("Before reader>>m_grpList=");
   			reader>>m_grpList ;
   			//not incorporating paging now
   			SmfResultPage page;
   			writeLog("m_grpList.count=");
   			writeLog(QString::number(m_grpList.count()));
   			emit m_contactFetcher->groupListAvailable(&m_grpList,SmfNoError,page);
   			
   			}
   			break;
   		case SmfContactSearchComplete:
   			{
   			if(m_contactList)
   				{
				delete m_contactList;
				m_contactList = NULL;
   				}
   			m_contactList = new SmfContactList;
   			quint32 error;
   			reader>>error;
   			SmfError err = (SmfError)error;
   			reader>>*(m_contactList);
   			//not incorporating paging now
   			SmfResultPage page;
   			//searchContactFinished
   			emit m_contactFetcher->searchContactFinished(m_contactList,err,page);
   			}
   			break;
   		default:
   			writeLog("Before m_grpList.clear=");
   			m_grpList.clear();
   			writeLog("Before reader>>m_grpList=");
   			reader>>m_grpList ;
   			//not incorporating paging now
   			SmfResultPage page;
   			writeLog("m_grpList.count=");
   			writeLog(QString::number(m_grpList.count()));
   			emit m_contactFetcher->groupListAvailable(&m_grpList,SmfNoError,page);
   		}
   	
   	}
void SmfContactFetcherPrivate::writeLog(QString log) const
	{
	QFile file("c:\\data\\SmfClientLogs.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
	         return;
    QTextStream out(&file);
    out << log << "\n";
    file.close();

	}
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfError &err )
	{
	quint32 errInt = (quint32)err;
	aDataStream<<errInt;
	return aDataStream;
	}


QDataStream &operator>>( QDataStream &aDataStream, 
		SmfError &err )
	{
	quint32 errInt;
	aDataStream>>errInt;
	err = (SmfError)errInt;
	return aDataStream;
	}