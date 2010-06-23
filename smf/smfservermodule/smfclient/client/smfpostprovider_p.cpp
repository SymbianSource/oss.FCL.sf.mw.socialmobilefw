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
 *
 */

#include "smfpostprovider_p.h"
//logging headers
#ifdef WRITE_LOG
#include <QTextStream>
#include <QFile>
#endif
#ifdef Q_OS_SYMBIAN
#include "smfclientsymbian.h"
#else
#include "SmfClientQt.h"
#endif
SmfPostProviderPrivate::SmfPostProviderPrivate(SmfPostProvider* postProvider)
: m_postProvider(postProvider),m_postList(NULL)
	{
	//private impl for symbian
	#ifdef Q_OS_SYMBIAN
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
	#endif
	}
SmfPostProviderPrivate::~SmfPostProviderPrivate()
	{
	if(m_SmfClientPrivate)
		{
		delete m_SmfClientPrivate;
		m_SmfClientPrivate = NULL;
		}
	}
/**
 * Gets the posts asynchronously. The signal postsAvailable()with SmfPostList is emitted
 * once the post lists are available
 * @param user user's contact in this SP, omit for self contact
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE 
 * @see postsAvailable()
 */
void SmfPostProviderPrivate::posts(SmfContact* user ,int pageNum,int perPage)
{
	  //We need to pass Opcode and SmfProvider+SmfContact
	  // (when user is not NULL) serialized into bytearray 
	  
	  m_baseProvider = m_postProvider->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  //TODO:- use different opcode incase post retreival is for other user
	  //serialize SmfContact if post retrieval is for other user

	  if(user)
		  {
		  //m_xtraInfoFlag = 1;
		  //write<<m_xtraInfoFlag;
		  //now serialize SmfContact 
		  m_xtraInfoSerialized.clear();
		  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
		  Xtrawrite<<*(user);
		  m_xtraInfoFlag = m_xtraInfoSerialized.size();
		  write<<m_xtraInfoFlag;
		  }
	  else
		  {
		  m_xtraInfoFlag = 0;
		  write<<m_xtraInfoFlag;
		  }
	  //serialize end
	  intfName.clear();
	  intfName.append(postProviderInterface);
	  int maxAllocation = MaxSmfPostSize*perPage;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfContactRetrievePosts,maxAllocation,m_xtraInfoSerialized);

}
	
/**
 * Updates a post to own area, the success of the post can be checked with signal
 * updatePostFinished() signal
 * @param postData data to be posted
 * @param location location data
 */
void SmfPostProviderPrivate::post(SmfPost& postData,SmfLocation& location) 
	{
	  m_xtraInfoSerialized.clear();
	  m_baseProvider = m_postProvider->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  //SmfProvider
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;
	  //m_xtraInfoFlag = 1;
	  //write<<m_xtraInfoFlag;
	  
	  QDataStream xtraInfoWrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //SmfPost
	  xtraInfoWrite<<postData;
	  //SmfPlace
	  xtraInfoWrite<<location;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;
	  //serialize end
	  QString intfName(postProviderInterface);
	  //we are not mentioning the max size, as we'll receive only bool
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfContactPost,10,m_xtraInfoSerialized);
	}
/**
 * Updates the last post to own area with new data, the success of the post can be checked with signal
 * updatePostFinished() signal
 * @param postData edited/new data to be posted
 * @param location location data
 */
void SmfPostProviderPrivate::updatePost(SmfPost& postData)
	{
	  m_baseProvider = m_postProvider->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  //SmfProvider
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;
	  //m_xtraInfoFlag = 1;
	  //write<<m_xtraInfoFlag;
	  
	  QDataStream xtraInfoWrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //SmfPost
	  xtraInfoWrite<<postData;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;
	  //serialize end
	  QString intfName(postProviderInterface);
	  //we are not mentioning the max size, as we'll receive only bool
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfContactUpdatePost,10,m_xtraInfoSerialized);
	}

/**
 * Updates a post to a particular Smf contact. the success of the post can be checked with signal
 * updatePostFinished() signal.
 * @param postData data to be posted
 * @param contact contact to which the post is to be directed
 * @param location location data
 */
void SmfPostProviderPrivate::postDirected(SmfPost& postData,SmfContact& contact,SmfLocation* location)
	{

	  m_baseProvider = m_postProvider->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  //SmfProvider
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;
	  //m_xtraInfoFlag = 1;
	  //write<<m_xtraInfoFlag;
	  
	  QDataStream xtraInfoWrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //SmfPost
	  xtraInfoWrite<<postData;
	  //SmfContact
	  xtraInfoWrite<<contact;
	  //SmfPlace
	  xtraInfoWrite<<location;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;
	  //serialize end
	  
	  QString intfName(postProviderInterface);
	  //we are not mentioning the max size, as we'll receive only bool
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfContactPostDirected,10,m_xtraInfoSerialized);
	}

/**
 * Posts appearance info of the user.e.g. appear offline, busy, do-not-disturb
 * @param appearence user appearance
 * @see SmfPresenceInfo
 * @return False on Failure/Not supported 
 */
//TODO:-How to return "supported" value? should it be synchronous?
//Currently doing it asynchronously with the assumption of always supported
//TODO:- implement some signal completion API
bool SmfPostProviderPrivate::postAppearence(SmfAppearenceInfo appearence)
	{
	  m_baseProvider = m_postProvider->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;
	  //m_xtraInfoFlag = 1;
	  //write<<m_xtraInfoFlag;
	  QDataStream xtraInfoWrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  xtraInfoWrite<<appearence;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;
	  QString intfName(postProviderInterface);
	  //we are not mentioning the max size, as we'll receive only bool
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfContactPostAppearence,10,m_xtraInfoSerialized);
	}
/**
 * Share /a contact's post to user's friends and followers (e.g. retweet in twitter, share on facebook)
 * emits updatePostFinished() signal when done.
 * @param postData data to be posted
 * @param contact contact to which the post belonged
 * @param bool whether user changed items within the post
 */
void SmfPostProviderPrivate::sharePost(SmfPost& postData,SmfContact& contact,bool edited)
	{
	  m_baseProvider = m_postProvider->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;
	  //m_xtraInfoFlag = 1;
	  //write<<m_xtraInfoFlag;
	  
	  QDataStream xtraInfoWrite(&m_serializedDataToServer,QIODevice::WriteOnly);
	  xtraInfoWrite<<postData;
	  xtraInfoWrite<<contact;
	  xtraInfoWrite<<edited;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;
	  QString intfName(postProviderInterface);
	  //we are not mentioning the max size, as we'll receive only bool
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfContactSharePost,10,m_serializedDataToServer);
	}
/**
* From smfobserver
*/
void SmfPostProviderPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error)
   {
	writeLog("SmfPostProviderPrivate::resultsAvailable");
	writeLog("opcode=");
	writeLog(QString::number(opcode));
	QDataStream reader(&result,QIODevice::ReadOnly);
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfContactRetrievePosts:
			{
				if(m_postList)
					{
					delete m_postList;
					m_postList = NULL;
					}

				m_postList = new SmfPostList;
				reader>>*(m_postList);
				writeLog("postList.count=");
				writeLog(QString::number(m_postList->count()));
				foreach(SmfPost post,*m_postList)
					{
					writeLog(post.description());
					}
				//not incorporating paging now
				SmfResultPage page;

				emit m_postProvider->postsAvailable(m_postList,error,page);
			
			}
			break;
		case SmfContactPost:
			{
			int errInt;
			reader>>errInt;
			writeLog("success=");
			SmfError success = (SmfError)errInt; 
			emit m_postProvider->postFinished(success);
			
			}
			break;
		case SmfContactUpdatePost:
		case SmfContactPostDirected:
		case SmfContactSharePost:
			{
			int errInt;
			reader>>errInt;
			writeLog("success=");
			SmfError success = (SmfError)errInt; 
			emit m_postProvider->postFinished(success);
			}
		default:
			Q_ASSERT_X(1,"SmfPostProviderPrivate::resultsAvailable","default case");
		}
   }
void SmfPostProviderPrivate::writeLog(QString log) const
	{
#ifdef WRITE_LOG
	QFile file("c:\\data\\SmfClientLogs.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
	         return;
    QTextStream out(&file);
    out << log << "\n";
    file.close();
#endif
	}
