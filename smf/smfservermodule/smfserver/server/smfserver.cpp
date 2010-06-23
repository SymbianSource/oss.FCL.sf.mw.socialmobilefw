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

#include "smfserver.h"
#include "smfpluginmanager.h"
#include "smftransportmanager.h"
#include "dsm.h"
#include "smfclientglobal.h"
#include <smfprovider.h>
#include <smfpost.h>
#include <smflocation.h>
#include <smfcontact.h>
#include <smfpicture.h>
#include <smfcomment.h>
#include <SmfCredMgrClient.h>
#include <smfrelationmgr.h>
#include <smfclientglobal.h> 
#include <QImage>
#include <QUrl>
#include <smfgroup.h>
#include <QTextStream>
#include <QFile>

#ifdef Q_OS_SYMBIAN
#include "smfserversymbian_p.h"
#else
#include "smfserverqt_p.h"
#include "smfserverqtsession.h"
#endif

SmfServer::SmfServer(QObject* parent)
    : QObject(parent)
	{
	}

SmfServer::~SmfServer()
{
	if(m_SmfServerPrivate)
		{
		delete m_SmfServerPrivate;
		m_SmfServerPrivate = NULL;
		}
    
}

bool SmfServer::startServer()
	{
	bool success = false;
	//Initialize all the component handles
	
	SmfTransportInitializeResult networkStatus = prepareTransport();
	qDebug()<<("Before m_pluginManager construction");
	m_pluginManager = SmfPluginManager::getInstance(this);
	qDebug()<<("After m_pluginManager construction");
	//	m_dataStoreManager = new SmfDataStoreManager();
	//Initialize private implementation
	#ifdef Q_OS_SYMBIAN
	TRAPD(err, m_SmfServerPrivate = SmfServerSymbian::NewL(CActive::EPriorityStandard,this));
	QString log("SmfServer::startServer=");
	int errorQ = err ;
	log += errorQ;
	 qDebug()<<(log);
	if( KErrNone != err )
		{
		return success;
		}
	TInt error = m_SmfServerPrivate->Start( KSmfServerName );
    RSemaphore semaphore;
    User::LeaveIfError( semaphore.OpenGlobal( KSmfServerSemaphoreName ) );
    // Semaphore opened ok
    semaphore.Signal();
    semaphore.Close();
	errorQ = error ;
	log.clear();
	log = QString("m_SmfServerPrivate->Start=");
	log += QString::number(error);
	 qDebug()<<(log);
	if( KErrNone == error ) 
		{
		success = true;
		}
	else 
		{
		//error
		return success;
		}
	#else
			m_SmfServerPrivate = new SmfServerQt(this);
	success = m_SmfServerPrivate->start();
	if (!success) 
		{
		return success;
		}
	#endif
	
	m_credentialMngr = new SmfCredMgrClient();
    return success;
	}
//Note:- Almost all the following APIs are called by private impl via the handle
/**
 * This API is called by the private impl when client is authorized
 * @param interfaceID Interface id, provided by the private impl (it gets it from client)
 * @param pluginIDMap Map of plugins who implement this interface and corresponding provider,
 * this is returned to the private impl
 * It calls PM to get the list. Note:- PM may return SmfProviderBase which is superset of SmfProvider.
 * TODO:- session should store this map for future ref?
 */
void SmfServer::getPlugins(SmfInterfaceID interfaceID, QMap<SmfPluginID,SmfProvider>& pluginIDMap)
	{
	pluginIDMap.clear();
	m_pluginManager->getPlugins(interfaceID,pluginIDMap);
	}

SmfPluginID SmfServer::getPlugin(SmfInterfaceID interfaceID,SmfProvider provider)
	{
	//TODO:- do it properly as per PM implementation
	//return PM->getplugin(interfaceID,provider);
	//TODO:- PM is not yet ready
	SmfPluginID id;
	m_pluginManager->getPluginId(interfaceID,provider, id);
	return id;
	//testing purpose only
	}
/**
 * This API is called by the private impl to get a list of authorized plugins from CM
 * @param list List of plugins to be filtered
 * @param authList List of authorised plugins filled by CM
 * this is returned to the private impl
 * It calls CMclient to get the list synchronously
 * TODO:- session should store this for future ref?
 */
void SmfServer::getAuthorizedPlugins(QList<SmfPluginID>& list,QList<SmfPluginID>& authList)
	{
	//TODO:-Uncomment the following once authorization flow of plugin is implemented 
/*	authList.clear();
	for(int i=0;i<list.count();i++)
		{
		bool isAuthorized = m_credentialMngr->CheckPluginAuthentication(list[i]);
		if(isAuthorized)
			{
			authList.append(list[i]);
			}
		}*/
	authList = list;
	}
SmfTransportInitializeResult SmfServer::prepareTransport()
	{
	qDebug()<<("Before SmfTransportManager::getInstance");
	m_transportManager = SmfTransportManager::getInstance();
	qDebug()<<("Before m_transportManager->initializeTransport");
	//checking the network status
	SmfTransportInitializeResult networkStatus = m_transportManager->initializeTransport();
	qDebug()<<("m_transportManager->initializeTransport return = ")<<networkStatus;
	return networkStatus;
	}
void SmfServer::sendToPluginManager(int requestID,SmfPluginID pluginID,SmfInterfaceID interfaceID,SmfRequestTypeID requestTypeID,QByteArray dataForPlugin ,int pageno,int perpage)
	{
	qDebug()<<("SmfServer::delegateToPluginManager");
	qDebug()<<(QString::number(requestID));
	qDebug()<<(pluginID);
	qDebug()<<(interfaceID);
	qDebug()<<(QString::number(requestTypeID));
	//TODO:-PM should take page info too
	m_pluginManager->createRequest(requestID,pluginID,requestTypeID,dataForPlugin);
	}
SmfError SmfServer::sendToDSM(QByteArray qtdataForDSM,SmfRequestTypeID opcode,QByteArray& qtdataFromDSM)
	{
	DataStoreManager* dsm = DataStoreManager::getDataStoreManager();
	//Note:- deserialization and formation of user profile and social profile are done by server
	QDataStream readStream(&qtdataForDSM,QIODevice::ReadOnly);
	QDataStream writeStream(&qtdataFromDSM,QIODevice::ReadOnly);
	switch(opcode)
		{
		case SmfRelationCreate:
			{
			//read the incoming data
			SmfProvider provider;
			SmfContact contact;
			readStream>>provider;
			readStream>>contact;

			SmfRelationId relnId = dsm->create(&provider,&contact);
			writeStream<<relnId;
			}
			break;
		case SmfRelationAssociate:
			{
			SmfRelationId relnId;
			SmfContact contact;
			SmfProvider provider;
			readStream>>relnId;
			readStream>>contact;
			readStream>>provider;

			SmfError err = dsm->associate(relnId,&contact,&provider);
			int errInt = err;
			writeStream<<errInt;
			}
			break;
		case SmfRelationSearchById:
			{
			SmfRelationId relnId;
			readStream>>relnId;

			SmfRelationItem* relnItem = dsm->searchById(relnId);
			writeStream<<*(relnItem);
			}
			break;
		case SmfRelationCount:
			{
			SmfRelationId relationId;
			readStream>>relationId;
			int cnt = dsm->count(relationId);
			writeStream<<cnt;
			}
			break;
		case SmfRelationGet:
			{
			SmfRelationId relationId;
			quint32 index;
			readStream>>relationId;
			readStream>>index;
			SmfRelationItem* relnItem = dsm->getContact(relationId,index);
			writeStream<<relnItem;
			}
			break;
		case SmfRelationGetAll:
			{
			SmfRelationId relationId;
			readStream>>relationId;

			QList<SmfRelationItem> relnIditemList = dsm->getAll(relationId);
			writeStream<<relnIditemList;
			}
			break;
		case SmfRelationGetAllRelations:
			{
			QList<SmfRelationId> relnIdList = dsm->getAllRelations();
			writeStream<<relnIdList;
			}
			break;
		default:
			break;
		}
	return SmfNoError;
	}
/**
 * This slot is invoked when CM finishes the authorization of the client.
 * @param authID As it contains the session ptr, sever directly invokes the session's API to notify success
 */
void SmfServer::clientAuthorizationFinished(bool success,SmfClientAuthID authID )
	{
	//TODO:- implement this api in session class
	//note:- in case success is false client completes the request with SmfErrClientAuthFailed
	//TODO:- define set of smf wide error after consulting with other module owners
	authID.session->clientathorizationFinished(success);
	}
/**
 * This API is called by PM once its done with request and parsing
 * @param requestID The request id which is completed
 * @param parsedData Serialized data(as per request type) filled by PM
 * @param error Error occured
 * TODO:- should use smf wide global errors instead
 */
void SmfServer::resultsAvailable(int requestID,QByteArray* parsedData,SmfError error)
	{
	qDebug()<<("SmfServer::resultsAvailable");
	qDebug()<<("requestID=");
	qDebug()<<(QString::number(requestID));
	qDebug()<<("parsedData->size()=");
	qDebug()<<(QString::number(parsedData->size()));
	qDebug()<<("Error=");
	qDebug()<<(QString::number(error));
	//Serialize error followed by actual data
	QByteArray dataWithError;
	QDataStream writer(&dataWithError,QIODevice::WriteOnly);
	writer<<error;
	if(parsedData->size())
		{
		writer<<*(parsedData);
		}
	//find out the appropriate session and request id and service that
	m_SmfServerPrivate->findAndServiceclient(requestID,&dataWithError,error);
	}
/**
 * Seems reduntant
 */
void SmfServer::serviceClient(QByteArray* parsedData)
	{
	Q_UNUSED(parsedData)
	}
/**
 * This is called when CMclient notifies client expiry.
 * @param type notification type, set of enums for future expansion
 * @param id Plugin Id for which the authentication has expired
 */
void SmfServer::authenticationKeysExpired(NotificationType type,SmfPluginID id)
	{
	Q_UNUSED(type)
	Q_UNUSED(id)
	//resend the notify request
	//CMclient->requestAuthExpiryNotify();
	}
/*void SmfServer::writeLog(QString log) const
	{
#ifdef WRITE_LOG
	QFile file("c:\\data\\SmfServerLogs.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
	         ;
    QTextStream out(&file);
    out << log << "\n";
    file.close();
#else
    Q_UNUSED(log)
#endif
	}*/
//#ifdef CLIENT_SERVER_TEST
dummyPM::dummyPM(SmfServer* server,QObject* parent)
: m_server(server),QObject(parent)
	{
	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(responseAvailable()));
	}
SmfError dummyPM::createRequest ( const quint32& aSessionID, 
		const QString& aPluginID, 
		const SmfRequestTypeID& aOperation, 
		QByteArray& aInputData )
	{
	qDebug()<<QString::number(aSessionID);
	qDebug()<<aPluginID;
	qDebug()<<QString::number(aOperation);
	qDebug()<<QString::number(aInputData.size());
	m_timer->start(1000);
	}
void dummyPM::responseAvailable()
	{
	
	}
//#endif
