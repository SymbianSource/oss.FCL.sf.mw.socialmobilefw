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

#include <QDebug>

#include "smfactivityfetcher.h"
#include "smfactivityfetcher_p.h"
#ifdef Q_OS_SYMBIAN
	#include "smfclientsymbian.h"
#else
	#include "smfclientqt.h"
#endif


SmfActivityFetcherPrivate::SmfActivityFetcherPrivate(SmfActivityFetcher *activityFetcher)
		:m_activityFetcher(activityFetcher)
	{
#ifdef Q_OS_SYMBIAN
	//private impl for symbian
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
#else
	//TODO:- Qt private impl
#endif
	}

SmfActivityFetcherPrivate::~SmfActivityFetcherPrivate()
	{
	if(m_SmfClientPrivate)
		{
		delete m_SmfClientPrivate;
		m_SmfClientPrivate = NULL;
		}
	}

void SmfActivityFetcherPrivate::selfActivities(int pageNum, int perPage)
	{
	SmfProvider* m_baseProvider = m_activityFetcher->getProvider();
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	//SmfProvider
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	//serialize end
	
	QString intfName(activityFetcherInterface);
	int max = MaxSmfActivityEntrySize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfActivitySelfActivity, max);
	}

void SmfActivityFetcherPrivate::friendsActivities(const SmfContact & aFriend, int pageNum, int perPage)
	{
	SmfProvider* m_baseProvider = m_activityFetcher->getProvider();
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	//SmfProvider
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<aFriend;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(activityFetcherInterface);
	int max = MaxSmfActivityEntrySize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfActivityFriendsActivities, max);
	}

void SmfActivityFetcherPrivate::filtered(QList<SmfActivityObjectType> filters, int pageNum, int perPage)
	{
	SmfProvider* m_baseProvider = m_activityFetcher->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	//SmfProvider
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<filters;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(activityFetcherInterface);
	int max = MaxSmfActivityEntrySize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfActivityFiltered, max);
	}

void SmfActivityFetcherPrivate::customRequest ( const int& operationId, QByteArray* customData )
	{
	SmfProvider* m_baseProvider = m_activityFetcher->getProvider();
	
	//serialize start
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
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
	
	QString intfName(activityFetcherInterface);
//ToDo:- How much size to allocate for custom data? keeping MaxSmfActivityEntrySize for now
	int maxAllocation = MaxSmfActivityEntrySize;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfActivityCustomRequest, maxAllocation);
	}

void SmfActivityFetcherPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode, SmfError error)
	{
	qDebug()<<"SmfActivityFetcherPrivate::resultsAvailable";
	qDebug()<<"opcode = "<<opcode;

	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfActivitySelfActivity:
		case SmfActivityFriendsActivities:
		case SmfActivityFiltered:
			{
			SmfActivityEntryList* m_entries = new SmfActivityEntryList;
			reader>>*m_entries;
			qDebug()<<"m_entries.count = "<<m_entries->count();
			
			//ToDo:- not incorporating paging now
			SmfResultPage page;

			emit m_activityFetcher->resultsAvailable(m_entries,error,page);
			}
			break;
			
   		case SmfActivityCustomRequest:
   			{
   			int operationId;
   			QByteArray *data = new QByteArray;
   			reader>>operationId;
   			reader>>*data;
   			
   			qDebug()<<"operationId = "<<operationId;
   			qDebug()<<"data size = "<<data->size();
   			emit m_activityFetcher->customDataAvailable(operationId, data);
   			}
   			break;
		default:
			Q_ASSERT_X(1,"Activity Private Unknown opcode","default case");
		}
	}
