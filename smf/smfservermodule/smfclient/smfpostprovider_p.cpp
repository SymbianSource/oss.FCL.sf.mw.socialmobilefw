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
 * Private implementation of SmfPostProvider
 *
 */

#include <qdebug.h>

#include "smfpostprovider.h"
#include "smfpostprovider_p.h"
#ifdef Q_OS_SYMBIAN
	#include "smfclientsymbian.h"
#else
	#include "SmfClientQt.h"
#endif


SmfPostProviderPrivate::SmfPostProviderPrivate(SmfPostProvider* postProvider)
		: m_postProvider(postProvider)
	{
#ifdef Q_OS_SYMBIAN
	//private impl for symbian
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
 * returns maximum no of chars (unicode) that service provider can post without truncation
 * negative value means no limit
 */
qint32 SmfPostProviderPrivate::maxCharsInPost()
	{ 
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	
	//SmfProvider
	write<<*m_baseProvider;
	
	QString intfName(postProviderInterface);
	//setting max size to 10
	//call private impl's send method
	QByteArray data = m_SmfClientPrivate->sendSyncRequest(intfName,	
			SmfPostGetMaxCharsInPost, 10, m_serializedDataToServer);
	
	//De-serialize it into qint32
	QDataStream reader(&data,QIODevice::ReadOnly);
	qint32 val;
	reader>>val;
	return val;
	}

/**
 * returns maximum no of items that can be returned in a single query to getPosts
 * negative value means feature not supported.
 */
qint32 SmfPostProviderPrivate::maxItems()
	{ 
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	
	//SmfProvider
	write<<*m_baseProvider;
	
	QString intfName(postProviderInterface);
	//setting max size to 10
	//call private impl's send method
	QByteArray data = m_SmfClientPrivate->sendSyncRequest(intfName,
			SmfPostGetMaxItems, 10, m_serializedDataToServer);
	
	//De-serialize it into qint32
	QDataStream reader(&data,QIODevice::ReadOnly);
	qint32 val;
	reader>>val;
	return val;
	}

/**
 * returns all the formatting of posts that this service provider supports.
 * May return 0 items to mean only QString is supported.
 */
QVector<QTextFormat> SmfPostProviderPrivate::supportedFormats ()
	{
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	
	//SmfProvider
	write<<*m_baseProvider;
	
	QString intfName(postProviderInterface);
	//ToDo :- Setting maxsize to 1000
	
	//call private impl's send method
	QByteArray data = m_SmfClientPrivate->sendSyncRequest(intfName,
			SmfPostGetSupportedFormats, 1000, m_serializedDataToServer);
	
	//De-serialize it into QVector<QTextFormat>
	QDataStream reader(&data,QIODevice::ReadOnly);
	QVector<QTextFormat> val;
	reader>>val;
	return val;
	}

/**
 * returns whether this SP supports Appearence @see SmfAppearenceInfo
 */
bool SmfPostProviderPrivate::supportsAppearence ()
	{ 
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	
	//SmfProvider
	write<<*m_baseProvider;
	
	QString intfName(postProviderInterface);
	//we are not mentioning the max size, as we'll receive only bool
	//call private impl's send method
	QByteArray data = m_SmfClientPrivate->sendSyncRequest(intfName,
			SmfPostGetAppearanceSupport, 10, m_serializedDataToServer);
	
	//De-serialize it into bool
	QDataStream reader(&data,QIODevice::ReadOnly);
	bool val;
	reader>>val;
	return val;
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
	//We need to pass Opcode and SmfProvider+SmfContact (when user is not NULL) serialized into bytearray 
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	if(user)
		{
		write<<m_argFlag;
		write<<*user;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	m_argFlag = 1;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(postProviderInterface);
	int maxAllocation = MaxSmfPostSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfContactRetrievePosts, maxAllocation);
	}
	
/**
 * Updates a post to own area, the success of the post can be checked with signal
 * updatePostFinished() signal
 * @param postData data to be posted
 * @param location location data
 */
void SmfPostProviderPrivate::post(SmfPost& postData,SmfLocation& location) 
	{
	m_serializedDataToServer.clear();
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	
	//SmfProvider
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<postData;
	write<<m_argFlag;
	write<<location;
	
	QString intfName(postProviderInterface);
	
	//we are not mentioning the max size, as we'll receive only bool
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfContactPost, 10);
	}

/**
 * Updates the last post to own area with new data, the success of the post can be checked with signal
 * updatePostFinished() signal
 * @param postData edited/new data to be posted
 * @param location location data
 */
void SmfPostProviderPrivate::updatePost(SmfPost& postData)
	{
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	//SmfProvider
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<postData;
	
	QString intfName(postProviderInterface);
	//we are not mentioning the max size, as we'll receive only bool
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfContactUpdatePost, 10);
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
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	
	//SmfProvider
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<postData;
	write<<m_argFlag;
	write<<contact;
	if(location)
		{
		write<<m_argFlag;
		write<<*location;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	
	QString intfName(postProviderInterface);
	//we are not mentioning the max size, as we'll receive only bool
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfContactPostDirected, 10);
	}

/**
 * Method to post a comment on a post.
 * @param aTarget Post on which comment has to be posted
 * @param aComment comment to be posted
 * @param aLocation location data
 */
void SmfPostProviderPrivate::commentOnAPost( SmfPost &aTarget, SmfPost &aComment, 
		SmfLocation *aLocation)
	{
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	
	//SmfProvider
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<aTarget;
	write<<m_argFlag;
	write<<aComment;
	
	if(aLocation)
		{
		write<<m_argFlag;
		write<<*aLocation;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	
	QString intfName(postProviderInterface);
	//we are not mentioning the max size, as we'll receive only bool
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfContactCommentOnAPost, 10);
	}

/**
 * Posts appearance info of the user.e.g. appear offline, busy, do-not-disturb
 * @param appearence user appearance
 * @see SmfPresenceInfo
 * @return False on Failure/Not supported 
 */
bool SmfPostProviderPrivate::postAppearence(SmfAppearenceInfo appearence,
		const QString &status)
	{
	//TODO:-How to return "supported" value? should it be synchronous?
	//Currently doing it asynchronously with the assumption of always supported
	//TODO:- implement some signal completion API
	m_serializedDataToServer.clear();
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<appearence;
	write<<m_argFlag;
	write<<status;
		
	QString intfName(postProviderInterface);
	//we are not mentioning the max size, as we'll receive only bool
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfContactPostAppearence, 10);
	
	return true;
	}

/**
 * Share a contact's post to user's friends and followers (e.g. retweet in twitter, share on facebook)
 * emits updatePostFinished() signal when done.
 * @param postData data to be posted
 * @param contact contact to which the post belonged
 * @param bool whether user changed items within the post
 */
void SmfPostProviderPrivate::sharePost(SmfPost& postData,SmfContact& contact,bool edited)
	{
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<postData;
	write<<m_argFlag;
	write<<contact;
	write<<m_argFlag;
	write<<edited;
	
	QString intfName(postProviderInterface);
	//we are not mentioning the max size, as we'll receive only bool
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfContactSharePost, 10);
	}

/**
 * Request for a custom operation. The signal customDataAvailable() is emitted 
 * when the result is available.
 * @param operationId OperationId
 * @param customData Custom data to be sent
 * Note:-Interpretation of operationId and customData is upto the concerned
 * plugin and client application. service provider should provide some
 * serializing-deserializing utilities for these custom data
 */
void SmfPostProviderPrivate::customRequest ( const int& operationId, QByteArray* customData )
	{
	SmfProvider* m_baseProvider = m_postProvider->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<operationId;
	if(customData)
		{
		write<<m_argFlag;
		write<<*customData;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	
	QString intfName(postProviderInterface);
	//ToDo:- How much size to allocate for custo data? keeping MaxSmfPostSize for now
	int maxAllocation = MaxSmfPostSize;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfContactPostCustomRequest, maxAllocation);
	}

/**
* From smfobserver
*/
void SmfPostProviderPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error)
   {
	qDebug()<<"Inside SmfPostProviderPrivate::resultsAvailable()";
	qDebug()<<"opcode = "<<opcode;

	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfContactRetrievePosts:
			{
			SmfPostList* m_postList = new SmfPostList;
			reader>>*(m_postList);
			
			qDebug()<<"postList.count="<<m_postList->count();
			
			//not incorporating paging now
			SmfResultPage page;

			emit m_postProvider->postsAvailable(m_postList,error,page);
			}
			break;
			
		case SmfContactPost:	
		case SmfContactUpdatePost:
		case SmfContactPostDirected:
		case SmfContactCommentOnAPost:
		case SmfContactPostAppearence:
		case SmfContactSharePost:
			{
			int errInt;
			reader>>errInt;
			qDebug()<<"success = "<<errInt;
			SmfError success = (SmfError)errInt;
			emit m_postProvider->postFinished(success);
			}
			break;
			
		case SmfContactPostCustomRequest:
   			{
   			int operationId;
   			QByteArray *data = new QByteArray;
   			reader>>operationId;
   			reader>>*data;
   			qDebug()<<"operationId = "<<operationId;
   			qDebug()<<"data size = "<<data->size();
   			emit m_postProvider->customDataAvailable(operationId, data);
   			}
   			break;
			
		default:
			Q_ASSERT_X(1,"PostProvider Private Unknown opcode","default case");
		}
   }
