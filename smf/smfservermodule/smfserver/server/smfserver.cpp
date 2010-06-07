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
#include "smfclientglobal.h"
#include <smfprovider.h>
#include <smfpost.h>
#include <smflocation.h>
#include <smfcontact.h>
#include <smfpicture.h>
#include <smfcomment.h>
//#ifdef NO_OTHER_MODULES
#include <QImage>
#include <QUrl>
#include <smfgroup.h>
#include <QTextStream>
#include <QFile>
//#endif
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
    delete m_SmfServerPrivate;
}

bool SmfServer::startServer()
	{
	bool success = false;
	//Initialize all the component handles
	writeLog("Not doing SmfTransportManager::getInstance");
	//m_transportManager = SmfTransportManager::getInstance();
	writeLog("Not doing m_transportManager->initializeTransport");
	//checking the network status, no need to proceed if not permitted
	//SmfTransportInitializeResult networkStatus = m_transportManager->initializeTransport();
	writeLog("Before networkStatus");
	//TODO:-check this
	if(1/*networkStatus == SmfTransportInitNoError*/)
		{
		writeLog("Before m_pluginManager construction");
		m_pluginManager = SmfPluginManager::getInstance(this);
		writeLog("After m_pluginManager construction");
		//	m_dataStoreManager = new SmfDataStoreManager();
		//Initialize private implementation
		#ifdef Q_OS_SYMBIAN
		TRAPD(err, m_SmfServerPrivate = SmfServerSymbian::NewL(CActive::EPriorityStandard,this));
		QString log("SmfServer::startServer=");
		int errorQ = err ;
		log += errorQ;
		 writeLog(log);
		if( KErrNone != err )
			{
			return success;
			}
		TInt error = m_SmfServerPrivate->Start( KSmfServerName );
		errorQ = error ;
		log.clear();
		log = QString("m_SmfServerPrivate->Start=");
		log += QString::number(error);
		 writeLog(log);
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
		//request CM server for auth expiry info, note:- we should resend the request everytime we get expiry
		//notification
		//TODO:- Do it after CM implementation
		//CMclient->requestAuthExpiryNotify();
		//connect(CMClient,SIGNAL(authExpiryNotify(NotificationType,SmfPluginID),this,SLOT(NotificationType,SmfPluginID));
		}
    return success;
	}
//Note:- Almost all the following APIs are called by private impl via the handle
/**
 * This called first once a session is created by the private impl
 * @param clientID some unique client process ID, SID for symbian platform
 * TODO:-How to get the pid for rest of the platforms?
 * @return true if client is already athorized, else false
 * 
 */
bool SmfServer::isClientAuthorized(SmfClientAuthID clientID)
	{
	//TODO:- once CM is complete do it properly
	Q_UNUSED(clientID);
	return true;
	}
/**
 * This API is called by the private impl when isClientAuthorized returns false
 * @param clientID client Id (SID for symbian platform), provided by the private impl
 * Note:- The session(and in turn the client) will be blocked untill authorization completes.
 */
void SmfServer::authorizeClient(SmfClientAuthID clientID)
	{
	//TODO:- once CM is complete, do it properly
	//CMclient->authorizeClient(clientID)
	//connect(CMClient,SIGNAL(clientAuthorizationFinished(bool,SmfClientAuthID)),this,SLOT(clientAuthorizationFinished(bool,SmfClientAuthID)));
	Q_UNUSED(clientID);	
	}
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
	//TODO:- do it properly as per CMclient implementation
	//CMclient->getAuthorizedPlugins(list,authList);
	//testing purpose only
#ifdef NO_OTHER_MODULES
	authList = list;
#endif
	}
/**
 * This API is called by the private impl to request the PM to do the actual request/parsing asynchronously
 * Note:- the sever private impl (SmfServerSymbian for symbian platform) maintains a map of session ptr and some randomely
 * generated number.For the time being we are allowing only one outstanding request per session, so no need to store
 * both session and msg ptr, but in future we may need to.
 * @param requestID Request id (corresponds to the key of the mapmaintained by SmfServerSymbian).
 * @param pluginID PluginID, provided by the session
 * @param interfaceID InterfaceID provided by session
 * @requestTypeID Request Opcode, provided by session
 */
void SmfServer::getRequestedData(int requestID,SmfPluginID pluginID,SmfInterfaceID interfaceID, SmfRequestTypeID requestTypeID,QByteArray dataForPlugin)
	{
		//TODO:- cross check the params with PM owner
		//PM->getData(requestID,pluginID,interfaceID,requestTypeID);
		m_SmfServerPrivate->writeLog("SmfServer::getRequestedData");
		m_SmfServerPrivate->writeLog(QString::number(requestID));
		m_SmfServerPrivate->writeLog(pluginID);
		m_SmfServerPrivate->writeLog(interfaceID);
		m_SmfServerPrivate->writeLog(QString::number(requestTypeID));
		bool defaultCase = false;
		//serializedInfo contains base provider info+ other data depending on requestTypeID
		QDataStream reader(&dataForPlugin,QIODevice::ReadOnly);
		QByteArray dataTobeSent;
		switch(requestTypeID)
			{
			case SmfContactPost:
				{
				SmfProvider provider;
				SmfPost post;
				SmfLocation loc;
				reader>>provider;
				reader>>post;
				reader>>loc;
				//now form the serialized bytearray with only SmfPost and SmfPlace
				QDataStream writer(&dataTobeSent,QIODevice::WriteOnly);
				writer<<post;
				writer<<loc;
				}
				break;
			case SmfContactUpdatePost:
				{
				SmfProvider provider;
				SmfPost post;
				reader>>provider;
				reader>>post;
				//now form the serialized bytearray with only SmfPost
				QDataStream writer(&dataTobeSent,QIODevice::WriteOnly);
				writer<<post;
				}
				break;
			case SmfContactPostDirected:
				{
				SmfProvider provider;
				SmfPost post;
				SmfContact contact;
				SmfLocation place;
				reader>>provider;
				reader>>post;
				reader>>contact;
				reader>>place;
				QDataStream writer(&dataTobeSent,QIODevice::WriteOnly);
				writer<<post;
				writer<<contact;
				writer<<place;
				}
				break;
			case SmfContactSharePost:
				{
				SmfProvider provider;
				SmfPost post;
				SmfContact contact;
				bool edited;
				reader>>provider;
				reader>>post;
				reader>>contact;
				reader>>edited;
				//now form the serialized bytearray with only SmfPost
				QDataStream writer(&dataTobeSent,QIODevice::WriteOnly);
				writer<<post;
				writer<<contact;
				writer<<edited;
				}
				break;
			case SmfPictureDescription:
			case SmfPictureUpload:
				{
				SmfProvider provider;
				SmfPicture picture;
				reader>>provider;
				reader>>picture;
				//now form the serialized bytearray with only SmfPicture
				QDataStream writer(&dataTobeSent,QIODevice::WriteOnly);
				writer<<picture;
				}
				break;
			case SmfPictureMultiUpload:
				{
				SmfProvider provider;
				SmfPictureList pictureList;
				reader>>provider;
				reader>>pictureList;
				//now form the serialized bytearray with only SmfPicture
				QDataStream writer(&dataTobeSent,QIODevice::WriteOnly);
				writer<<pictureList;
				}
				break;
			case SmfPicturePostComment:
				{
				SmfProvider provider;
				SmfPicture picture;
				SmfComment comment;
				reader>>provider;
				reader>>picture;
				reader>>comment;
				//now form the serialized bytearray with only SmfPicture
				QDataStream writer(&dataTobeSent,QIODevice::WriteOnly);
				writer<<picture;
				writer<<comment;
				}
				break;
			default:
				defaultCase = true;
				break;
			}
		//When xtra info to be sent to plugin manager
		if(!defaultCase)
			{
				m_pluginManager->createRequest(requestID,pluginID,requestTypeID,dataTobeSent);
			}
		//when plugin manager needs to xtra info
		else
			{
			QByteArray arr;
			m_pluginManager->createRequest(requestID,pluginID,requestTypeID,arr);
			}
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
	authID.session->clientAuthorizationFinished(success);
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
	m_SmfServerPrivate->writeLog("SmfServer::resultsAvailable");
	m_SmfServerPrivate->writeLog("requestID=");
	m_SmfServerPrivate->writeLog(QString::number(requestID));
	m_SmfServerPrivate->writeLog("parsedData->size()=");
	m_SmfServerPrivate->writeLog(QString::number(parsedData->size()));
	m_SmfServerPrivate->writeLog("Error=");
	m_SmfServerPrivate->writeLog(QString::number(error));
	//Serialize error followed by actual data
	QByteArray dataWithError;
	QDataStream writer(&dataWithError,QIODevice::WriteOnly);
	writer<<error;
	writer<<*(parsedData);
	//find out the appropriate session and request id and service that
	m_SmfServerPrivate->findAndServiceclient(requestID,&dataWithError,error);
	}
/**
 * Seems reduntant in symbian as client is served with RMessage2::Complete()
 */
void SmfServer::serviceClient(QByteArray* parsedData)
	{
	Q_UNUSED(parsedData);
	}
/**
 * Used by PM to get a list of tokens
 * TODO:- cross check the params
 */
void SmfServer::getAuthenticationKeys(int pluginID,QStringList& keys,QStringList& urls)
	{
	//SMF-CM communication will be via CM client
	//TODO:- do it properly as per CM client impl
	//CMclient->getKeys(pluginID,keys,urls);
	Q_UNUSED(pluginID);
	Q_UNUSED(keys);
	Q_UNUSED(urls);
	}

/**
 * This is called when CMclient notifies client expiry.
 * @param type notification type, set of enums for future expansion
 * @param id Plugin Id for which the authentication has expired
 */
void SmfServer::authenticationKeysExpired(NotificationType type,SmfPluginID id)
	{
	//resend the notify request
	//CMclient->requestAuthExpiryNotify();
	Q_UNUSED(type);
	Q_UNUSED(id);
	}

void SmfServer::writeLog(QString log) const
	{
#ifdef WRITE_LOG
	QFile file("c:\\data\\SmfServerLogs.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
	         ;
    QTextStream out(&file);
    out << log << "\n";
    file.close();
#else
    Q_UNUSED(log);
#endif
	}
