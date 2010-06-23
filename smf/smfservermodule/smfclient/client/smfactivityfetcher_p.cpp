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
#include <QDebug>
#include "smfactivityfetcher_p.h"
#ifdef Q_OS_SYMBIAN
#include "smfclientsymbian.h"
#else
#include "SmfClientQt.h"
#endif
SmfActivityFetcherPrivate::SmfActivityFetcherPrivate(SmfActivityFetcher *activityFetcher)
:m_activityFetcher(activityFetcher)
	{
	//private impl for symbian
	#ifdef Q_OS_SYMBIAN
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

void SmfActivityFetcherPrivate::friendsActivities(const SmfContact & aFriend, int pageNum, int perPage)
	{
	  m_xtraInfoSerialized.clear();
	  m_baseProvider = m_activityFetcher->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  //SmfProvider
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  
	  QDataStream xtraInfoWrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);

	  xtraInfoWrite<<aFriend;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;
	  //serialize end
	  QString intfName(activityFetcherInterface);
	  int max = MaxSmfActivityEntrySize*perPage;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfActivityFriendsActivities,max,m_xtraInfoSerialized);
	}

void SmfActivityFetcherPrivate::filtered(QList<SmfActivityObjectType> filters, int pageNum, int perPage)
	{
	  m_xtraInfoSerialized.clear();
	  m_baseProvider = m_activityFetcher->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  //SmfProvider
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  
	  QDataStream xtraInfoWrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);

	  xtraInfoWrite<<filters;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;
	  //serialize end
	  QString intfName(activityFetcherInterface);
	  int max = MaxSmfActivityEntrySize*perPage;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfActivityFiltered,max,m_xtraInfoSerialized);
	}

void SmfActivityFetcherPrivate::selfActivities(int pageNum, int perPage)
	{
	  m_xtraInfoSerialized.clear();
	  m_baseProvider = m_activityFetcher->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  //SmfProvider
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;

	  m_xtraInfoFlag = 0;
	  write<<m_xtraInfoFlag;
	  //serialize end
	  QString intfName(activityFetcherInterface);
	  int max = MaxSmfActivityEntrySize*perPage;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfActivitySelfActivity,max);
	}
void SmfActivityFetcherPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode, SmfError error)
	{
	qDebug()<<"SmfPostProviderPrivate::resultsAvailable";
	qDebug()<<"opcode=";
	qDebug()<<QString::number(opcode);
	QDataStream reader(&result,QIODevice::ReadOnly);
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfActivityFriendsActivities:
		case SmfActivityFiltered:
		case SmfActivitySelfActivity:
			{
			if(m_entries)
				{
				delete m_entries;
				m_entries = NULL;
				}

			m_entries = new SmfActivityEntryList;
			reader>>*(m_entries);
			//not incorporating paging now
			SmfResultPage page;

			emit m_activityFetcher->resultsAvailable(m_entries,error,page);
			}
			break;
		default:
			Q_ASSERT_X(1,"SmfActivityFetcherPrivate::resultsAvailable","default case");
		}
	}



