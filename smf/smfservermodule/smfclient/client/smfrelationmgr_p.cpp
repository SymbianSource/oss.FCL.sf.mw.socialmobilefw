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
 */

#include "smfrelationmgr_p.h"
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

SmfRelationMgrPrivate::SmfRelationMgrPrivate(SmfRelationMgr* relnMngr)
	{
	//private impl for symbian
	#ifdef Q_OS_SYMBIAN
	m_SmfClientPrivate = CSmfClientSymbian::NewL();
	#endif
	}
SmfRelationMgrPrivate::~SmfRelationMgrPrivate()
	{
	if(m_SmfClientPrivate)
		{
		delete m_SmfClientPrivate;
		m_SmfClientPrivate = NULL;
		}
	}
SmfRelationId SmfRelationMgrPrivate::create(SmfProvider *provider, SmfContact *contact)
	{
	m_dataForDSM.clear();
	QDataStream writeStream(&m_dataForDSM,QIODevice::WriteOnly);
	writeStream<<*(provider);
	writeStream<<contact;
	m_dataFromDSM.clear();
	SmfError err=SmfNoError;
	m_dataFromDSM = m_SmfClientPrivate->sendDSMSyncRequest(SmfRelationCreate,m_dataForDSM,err,maxSmfRelationIdSize);
	if(err==SmfNoError)
		{
		QDataStream readStream(&m_dataFromDSM,QIODevice::ReadOnly);
		SmfRelationId relnId;
		readStream>>relnId;
		return relnId;
		}
	else
		{
		User::Panic(_L("DSM err="),err);
		}
	}
SmfError SmfRelationMgrPrivate::associate(const SmfRelationId& relation,const SmfContact* contact,SmfProvider* provider)
	{
	m_dataForDSM.clear();
	QDataStream writeStream(&m_dataForDSM,QIODevice::WriteOnly);
	writeStream<<relation;
	writeStream<<contact;
	writeStream<<provider;
	
	m_dataFromDSM.clear();
	SmfError err = SmfNoError;
	int maxAlloc = 100;
	m_dataFromDSM = m_SmfClientPrivate->sendDSMSyncRequest(SmfRelationAssociate,m_dataForDSM,err,maxAlloc);
	if(err == SmfNoError)
		{
		QDataStream readStream(&m_dataFromDSM,QIODevice::ReadOnly);
		int errInt;
		readStream>>errInt;
		err = (SmfError)errInt;
		}
	else
		{
		User::Panic(_L("DSM err="),err);
		}
	return err;
	}
SmfError SmfRelationMgrPrivate::remove(SmfRelationId& relation,const SmfContact* contact)
	{
	m_dataForDSM.clear();
	QDataStream writeStream(&m_dataForDSM,QIODevice::WriteOnly);
	writeStream<<relation;
	writeStream<<*(contact);
	
	m_dataFromDSM.clear();
	SmfError err = SmfNoError;
	int maxAlloc = 100;
	m_dataFromDSM = m_SmfClientPrivate->sendDSMSyncRequest(SmfRelationRemove,m_dataForDSM,err,maxAlloc);
	if(err == SmfNoError)
		{
		QDataStream readStream(&m_dataFromDSM,QIODevice::ReadOnly);
		int errInt;
		readStream>>errInt;
		err = (SmfError)errInt;
		}
	else
		{
		const QString errStr = QString::number(err);
		Q_ASSERT_X(1,"SmfRelationMgrPrivate::remove","error");
		}
	return err;
	}
SmfRelationItem SmfRelationMgrPrivate::searchById(const SmfRelationId& relation)
	{
	m_dataForDSM.clear();
	QDataStream writeStream(&m_dataForDSM,QIODevice::WriteOnly);
	writeStream<<relation;
	
	m_dataFromDSM.clear();
	int maxAlloc = MaxSmfContactSize;
	SmfError err=SmfNoError;
	m_dataFromDSM = m_SmfClientPrivate->sendDSMSyncRequest(SmfRelationSearchById,m_dataForDSM,err,maxAlloc);
	if(err == SmfNoError)
		{
		QDataStream readStream(&m_dataFromDSM,QIODevice::ReadOnly);
		SmfRelationItem relnItem;
		readStream>>relnItem;
		return relnItem;
		}
	else
		{
		User::Panic(_L("DSM err="),err);
		}
	}
SmfRelationId SmfRelationMgrPrivate::searchByContact( SmfContact contact)
	{
	m_dataForDSM.clear();
	QDataStream writeStream(&m_dataForDSM,QIODevice::WriteOnly);
	writeStream<<contact;
	
	m_dataFromDSM.clear();
	int maxAlloc = 1000;
	SmfError err=SmfNoError;
	m_dataFromDSM = m_SmfClientPrivate->sendDSMSyncRequest(SmfRelationSearchByContact,m_dataForDSM,err,maxAlloc);
	if(err == SmfNoError)
		{
		QDataStream readStream(&m_dataFromDSM,QIODevice::ReadOnly);
		SmfRelationId relnId;
		readStream>>relnId;
		return relnId;
		}
	else
		{
		User::Panic(_L("DSM err="),err);
		}
	}
//number of contacts in a relation
uint SmfRelationMgrPrivate::count(SmfRelationId relation)
	{
	m_dataForDSM.clear();
	QDataStream writeStream(&m_dataForDSM,QIODevice::WriteOnly);
	writeStream<<relation;
	
	m_dataFromDSM.clear();
	SmfError err = SmfNoError;
	int maxAlloc = 100;
	m_dataFromDSM = m_SmfClientPrivate->sendDSMSyncRequest(SmfRelationCount,m_dataForDSM,err,maxAlloc);
	if(err == SmfNoError)
		{
		QDataStream readStream(&m_dataForDSM,QIODevice::ReadOnly);
		uint count;
		readStream>>count;
		return count;
		}
	else
		{
		User::Panic(_L("DSM err="),err);
		}
	}

SmfRelationItem SmfRelationMgrPrivate::get(SmfRelationId& relation, quint32 index)
	{
	m_dataForDSM.clear();
	QDataStream writeStream(&m_dataForDSM,QIODevice::WriteOnly);
	writeStream<<relation;
	
	m_dataFromDSM.clear();
	SmfError err = SmfNoError;
	int maxAlloc = maxSmfRelationItemSize;
	m_dataFromDSM = m_SmfClientPrivate->sendDSMSyncRequest(SmfRelationGet,m_dataForDSM,err,maxAlloc);
	if(err == SmfNoError)
		{
		QDataStream readStream(&m_dataFromDSM,QIODevice::ReadOnly);
		SmfRelationItem relationItem;
		readStream>>relationItem;
		return relationItem;
		}
	else
		{
		User::Panic(_L("DSM err="),err);
		}
	}
QList<SmfRelationItem> SmfRelationMgrPrivate::getAll(SmfRelationId& relation)
	{
	m_dataForDSM.clear();
	
	m_dataFromDSM.clear();
	SmfError err = SmfNoError;
	int maxAlloc = maxSmfRelationItemSize*maxRelationItems;
	m_dataFromDSM = m_SmfClientPrivate->sendDSMSyncRequest(SmfRelationGetAll,m_dataForDSM,err,maxAlloc);
	if(err == SmfNoError)
		{
	QDataStream readStream(&m_dataFromDSM,QIODevice::ReadOnly);
	QList<SmfRelationItem> relationItemList;
	readStream>>relationItemList;
	return relationItemList;
		}
	else
		{
		User::Panic(_L("DSM err="),err);
		}
	}
QList<SmfRelationId> SmfRelationMgrPrivate::getAllRelations()
	{
	m_dataForDSM.clear();
	
	m_dataFromDSM.clear();
	SmfError err = SmfNoError;
	int maxAlloc = 1000*maxRelationItems;
	m_dataFromDSM = m_SmfClientPrivate->sendDSMSyncRequest(SmfRelationGetAllRelations,m_dataForDSM,err,maxAlloc);
	if(err == SmfNoError)
		{
		QDataStream readStream(&m_dataFromDSM,QIODevice::ReadOnly);
		QList<SmfRelationId> relationIdList;
		readStream>>relationIdList;
		return relationIdList;
		}
	else
		{
		User::Panic(_L("DSM err="),err);
		}
	}
SmfError SmfRelationMgrPrivate::Delete(SmfRelationId& relation)
	{
	
	}
