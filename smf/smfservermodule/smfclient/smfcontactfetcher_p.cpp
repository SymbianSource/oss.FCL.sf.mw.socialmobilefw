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
 * Private Qt wrapper for SmfContactFetcher
 * 
 */

#include <qdebug.h>

#include "smfcontactfetcher.h"
#include "smfcontactfetcher_p.h"
#ifdef Q_OS_SYMBIAN
	#include "smfclientsymbian.h"
#else
	#include "smfclientqt.h"
#endif

/**
 * Constructor
 * @param contactFetcher The SmfContactFetcher instance
 */
SmfContactFetcherPrivate::SmfContactFetcherPrivate ( SmfContactFetcher* contactFetcher )
		: m_contactFetcher(contactFetcher)
	{
#ifdef Q_OS_SYMBIAN
	//private impl for symbian
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
#endif
	}

/**
 * Destructor
 */
SmfContactFetcherPrivate::~SmfContactFetcherPrivate()
	{
	if(m_SmfClientPrivate)
		{
		delete m_SmfClientPrivate;
		m_SmfClientPrivate = NULL;
		}
	}

/**
 * Get the friend listing asynchronously. The friendsListAvailable() signal 
 * is emitted with SmfContactList once data is arrived. When the list is big,
 * user can specify the page number and per page item data. If not supplied 
 * by the user default values are used. 
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
 * @return true if success, else false
 */
bool SmfContactFetcherPrivate::friends(int pageNum,int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	m_argFlag = 1;
	SmfProvider* m_baseProvider = m_contactFetcher->getProvider();
	
	//serialize start
	m_dataSerialized.clear();
	QDataStream write(&m_dataSerialized,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(contactFetcherInterface);
	int maxAllocation = MaxSmfContactSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_dataSerialized, intfName, 
			SmfContactGetFriends, maxAllocation);
	
	return true;
	}

/**
 * Get the list of followers asynchronously. The followersListAvailable() signal
 * is emitted with SmfContactList once data is arrived. Please note that some
 * service may not support followers/fans - FALSE is returned if not supported.
 * When the list is big user can specify the page number and per page item data.
 * If not supplied by the user default values are used.
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
 * @return true if success, else false
 */
bool SmfContactFetcherPrivate::followers(int pageNum,int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_contactFetcher->getProvider();
	
	//serialize start
	m_dataSerialized.clear();
	QDataStream write(&m_dataSerialized,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
		
	QString intfName(contactFetcherInterface);
	int maxAllocation = MaxSmfContactSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_dataSerialized, intfName,
			SmfContactGetFollowers, maxAllocation);
	
	return true;
	}
  
/**
 * Searches for a contact The searchContactFinished() signal
 * is emitted with SmfContactList once data is arrived.
 * When the list is big user can specify the page number and per page item data.
 * If not supplied by the user default values are used.
 * @param contact The contact to be searched. The serach criteria must be 
 * set as one of its fields.
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
 */
void  SmfContactFetcherPrivate::search(SmfContact* contact,int pageNum,int perPage) 
	{
	//We need to pass Opcode and SmfProvider+SmfContact serialized into bytearray 
	SmfProvider* m_baseProvider = m_contactFetcher->getProvider();
	m_dataSerialized.clear();
	//serialize start
	QDataStream write(&m_dataSerialized,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	if(contact)
		{
		m_argFlag = 1;
		write<<m_argFlag;
		write<<*contact;
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
	write<<pageNum;
	
	QString intfName(contactFetcherInterface);
	int maxAllocation = MaxSmfContactSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_dataSerialized, intfName,
			SmfContactSearch, maxAllocation);
	}

/**
 * Searches for a contacts (friends) who are near the user. The signal 
 * searchNearFinished() is emitted with SmfContactList once data is arrived.
 * Proximity defines accuracy level. When the list is big user can specify 
 * the page number and per page item data. If not supplied by the user 
 * default values are used.
 * @param location The location information
 * @param proximity The search boundary criteria
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
 */
bool  SmfContactFetcherPrivate::searchNear(SmfLocation* location,
		SmfLocationSearchBoundary proximity,
		int pageNum,int perPage) 
	{
	SmfProvider* m_baseProvider = m_contactFetcher->getProvider();
	m_dataSerialized.clear();
	
	//serialize start
	QDataStream write(&m_dataSerialized,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	if(location)
		{
		m_argFlag = 1;
		write<<m_argFlag;
		write<<*location;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	write<<m_argFlag;
	write<<proximity;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(contactFetcherInterface);
	int maxAllocation = MaxSmfContactSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_dataSerialized, intfName,
			SmfContactSearchNear, maxAllocation);
	
	return true;
	}

/**
 * Get the list of groups. The groupListAvailable() signal is emitted with 
 * SmfGroupList once data is arrived. False might be returned if this service 
 * doesn't support any mode of grouping (very rare). When the list is big, 
 * user can specify the page number and per page item data. If not supplied 
 * by the user default values are used.
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
 */
bool SmfContactFetcherPrivate::groups(int pageNum,int perPage) 
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_contactFetcher->getProvider();
	m_dataSerialized.clear();
	
	//serialize start
	QDataStream write(&m_dataSerialized,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
		
	QString intfName(contactFetcherInterface);
	int maxAllocation = MaxSmfGroupSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_dataSerialized, intfName,
			SmfContactGetGroups, maxAllocation);
	
	return true;
	}

/**
 * Searches for Smf Contacts in an Smf group. The signal searchInGroupFinished() 
 * is emitted with SmfContactList once data is arrived. When the list is big user 
 * can specify the page number and per page item data. If not supplied by the 
 * user default values are used.
 * @param group The group to be searched in
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
 * @return true if success, else false
 */
bool  SmfContactFetcherPrivate::searchInGroup(SmfGroup group,int pageNum,int perPage) 
	{
	//We need to pass Opcode and SmfProvider+SmfGroup serialized into bytearray 
	SmfProvider* m_baseProvider = m_contactFetcher->getProvider();
	
	//serialize start
	m_dataSerialized.clear();
	QDataStream write(&m_dataSerialized,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<group;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(contactFetcherInterface);
	int maxAllocation = MaxSmfContactSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_dataSerialized, intfName,
			SmfContactSearchInGroup, maxAllocation);
	
	return true;
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
void SmfContactFetcherPrivate::customRequest ( const int& operationId, QByteArray* customData )
	{
	//We need to pass Opcode and SmfProvider+SmfGroup serialized into bytearray 
	SmfProvider* m_baseProvider = m_contactFetcher->getProvider();
	
	//serialize start
	m_dataSerialized.clear();
	QDataStream write(&m_dataSerialized,QIODevice::WriteOnly);
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
	
	QString intfName(contactFetcherInterface);
//ToDo:- How much size to allocate for custom data? keeping MaxSmfContactSize for now
	int maxAllocation = MaxSmfContactSize;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_dataSerialized, intfName,
			SmfContactCustomRequest, maxAllocation);
	}

/**
 * To notify availibility of asynchronous requests.
 * @param result Requested result, before using must check error param.
 * @param opcode Requested opcode, for which the result has arrived.
 * @param error Error
 */
void SmfContactFetcherPrivate::resultsAvailable(QByteArray result,
		SmfRequestTypeID opcode,SmfError error)
   	{
	qDebug()<<"SmfContactFetcherPrivate::resultsAvailable";
	
   	QDataStream reader(&result,QIODevice::ReadOnly);
   	
   	//Now de-serialize it based on opcode
   	switch(opcode)
   		{
   		case SmfContactGetFriends:
   			{
   			SmfContactList* m_contactList = new SmfContactList;
   			reader>>*m_contactList;
   			qDebug()<<"m_contactList.count = "<<m_contactList->count();

   			//TODO:-After consulting with PM owner decide page serialization
   			SmfResultPage page;

   			emit m_contactFetcher->friendsListAvailable(m_contactList,error,page);
   			}
   			break;
   			
   		case SmfContactGetFollowers:
   			{
   			SmfContactList* m_contactList = new SmfContactList;
   			reader>>*(m_contactList);
   			qDebug()<<"m_contactList.count = "<<m_contactList->count();
   			
   			//TODO:-After consulting with PM owner decide page serialization
   			SmfResultPage page;

   			emit m_contactFetcher->followersListAvailable(m_contactList,error,page);
			}
   			break;
   			
   		case SmfContactSearch:
   			{
   			SmfContactList* m_contactList = new SmfContactList;
   			reader>>*(m_contactList);
   			qDebug()<<"m_contactList.count = "<<m_contactList->count();
   			
   			/** @TODO:-After consulting with PM owner decide page serialization */
   			SmfResultPage page;
   			
  			emit m_contactFetcher->searchContactFinished(m_contactList,error,page);
   			}
   			break;
   			
   		case SmfContactSearchNear:
   			{
   			SmfContactList* m_contactList = new SmfContactList;
   			reader>>*(m_contactList);
   			qDebug()<<"m_contactList.count = "<<m_contactList->count();
   			
   			/** @TODO:-After consulting with PM owner decide page serialization*/
   			SmfResultPage page;
   			
   			emit m_contactFetcher->searchNearFinished(m_contactList,error,page);
   			}
   			break;
   			
   		case SmfContactGetGroups:
   			{
   			qDebug()<<"Before reader>>m_grpList";
   			SmfGroupList *m_grpList = new SmfGroupList;
   			reader>>*(m_grpList);
   			
   			/** @TODO:-After consulting with PM owner decide page serialization */
   			SmfResultPage page;
   			
   			qDebug()<<"m_grpList.count = "<<m_grpList->count();
   			emit m_contactFetcher->groupListAvailable(m_grpList,error,page);
   			}
   			break;
   			
   		case SmfContactSearchInGroup:
   			{
   			SmfContactList* m_contactList = new SmfContactList;
   			reader>>*(m_contactList);
   			
   			//TODO:-After consulting with PM owner decide page serialization
   			SmfResultPage page;
   			
   			qDebug()<<"m_contactList.count = "<<m_contactList->count();
   			
   			emit m_contactFetcher->searchInGroupFinished(m_contactList,error,page);
   			}
   			break;
   			
   		case SmfContactCustomRequest:
   			{
   			int operationId;
   			QByteArray *data = new QByteArray;
   			reader>>operationId;
   			reader>>*data;
   			
   			qDebug()<<"operationId = "<<operationId;
   			qDebug()<<"data size = "<<data->size();
   			emit m_contactFetcher->customDataAvailable(operationId, data);
   			}
   			break;
   			
   		default:
   			qDebug()<<"default - Contact Private Unknown opcode";
   			User::Panic(_L("Contact Private Unknown opcode = "),opcode);
   		}
   	}

QDataStream &operator<<( QDataStream &aDataStream, const SmfError &err )
	{
	quint32 errInt = (quint32)err;
	aDataStream<<errInt;
	return aDataStream;
	}


QDataStream &operator>>( QDataStream &aDataStream, SmfError &err )
	{
	quint32 errInt;
	aDataStream>>errInt;
	err = (SmfError)errInt;
	return aDataStream;
	}
