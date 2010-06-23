/**
* Copyright (c) 2010 Sasken Communication Technologies Ltd.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "{License}"
* which accompanies  this distribution, and is available
* at the URL "{LicenseUrl}".
*
* Initial Contributors:
* Manasij Roy, Sasken Communication Technologies Ltd - Initial contribution
*
* Contributors:
*
* Description:
* SMF Server component which handles the client requests and delegates them propoerly to the appropriate component
*
*/

#ifndef SMFSERVER_H
#define SMFSERVER_H
#include <QObject>
#include<QMap>
//SMF wide global const and macros, to be shared among SMF components as well as SMF aware apps
#include "smfglobal.h"
#include "smfprovider.h"
#include "smftransportmanager.h" // Transport Manager
#include <SmfCredMgrClientGlobal.h>
/*
* Forward declarations
* Other components of the SMF
*/
class SmfTransportManager;
class SmfPluginManager;
class SmfDataStoreManager;
class SmfSettingsAuthManager;
class SmfClient;
class SmfCredMgrClient;
//For the time being, need to change later
typedef QString SmfInterfaceID;
//For the time being, need to change later
typedef QString SmfPluginID;
//TODO:- define proper enums after consulting with CM owner
typedef int NotificationType;
//Private implementation for different platforms
#ifdef Q_OS_SYMBIAN
class SmfServerSymbian;
class SmfServerSymbianSession;
#else
class SmfServerQt;
class SmfServerQtSession;
#endif
#include "smfcontact.h"
#ifdef NO_OTHER_MODULES
#include <QTimer>
#include <QTextStream>
#endif
/**
 * SmfServer manages the client requests and delegates them to the appropriate SMF component,
 * service the clients asymchronously
 *
 */
class SmfServer : public QObject
{
  Q_OBJECT

public:

  SmfServer(QObject* parent = 0);
  
  /**
   * Does the following,-
   *First it'll check whether server is already running, if already running it'll
   *simply return
   *else
   *it'll start the server exe and initialize all other SMF components
   *returns whether server is started successfully or not  
   */
  bool startServer();
  
  ~SmfServer();

public:
  /*
   * Requests Plugin Manager to get a list of plugin IDs who implement
   * the interface interfaceID.
   * This is used got SmfClient::GetServices () where we need a list of plugins
   */
  void getPlugins(SmfInterfaceID interfaceID, QMap<SmfPluginID,SmfProvider>& pluginIDMap);
  /**
   * Same as above, except this is used for rest of the requests where we need
   * a particular plugin
   */
  SmfPluginID getPlugin(SmfInterfaceID interfaceID,SmfProvider provider);
  
  /*
   * Requests the Credential Manager to filter out non-authorized plugin IDs
   * from the list and get authorized plugins into authList. 
   */
  void getAuthorizedPlugins(QList<SmfPluginID>& list,QList<SmfPluginID>& authList);

  /**
   * Request CM API to get the list of authenticated plugins
   * @see SmfCredMgrClient::authenticatedPluginList()
   */
  QStringList getAuthenticatedPluginList(QString RegistrationToken);


  SmfTransportInitializeResult prepareTransport();
  /**
   * DEbugging purpose only
   */
  //void writeLog(QString log) const;
  /**
   * Request the Plugin manager to get the data.
   * @param requestID Corresponds to a client's session
   * @param pluginID Plugin for which the request is intended
   * @param interfaceID Interface name
   * @param dataForPlugin Data to be sent for this request
   * 
   */
  void sendToPluginManager(int requestID,SmfPluginID pluginID,SmfInterfaceID interfaceID,SmfRequestTypeID requestTypeID,QByteArray dataForPlugin = QByteArray(), int pageNo=-1,int perpage=-1);
  /**
   * Delegates the request to DSM and receives data synshronously.
   * @param qtdataForDSM Data to be passed to DSM
   * @param opcode Opcode
   * @param qtdataFromDSM Data received from DSM
   * @return Error value returned from DSM
   */
  SmfError sendToDSM(QByteArray qtdataForDSM,SmfRequestTypeID opcode,QByteArray& qtdataFromDSM);
public slots:

	/*
	 * This slot is called when Credential Manager is done with the autherizing
	 * the client for the first time. See isClientAuthorized() and authorizeClient().
	 * success specifies the success of the authorization, authID is the authentication
	 * ID in case its not same as SID of the client.
	 */
	void clientAuthorizationFinished(bool success,SmfClientAuthID authID );

	/*
	 * This slot is called as a result of trigger from Plugin manager when the 
	 * parsed data is available.
	 * @param requestID The request ID for which result is available
	 * @param parsedData Serialized data
	 */
	void resultsAvailable(int requestID,QByteArray* parsedData,SmfError error);

	/*
	 * Services the client request by sending the requested data.
	 * Note:- This will be handled by private implementation.
	 */
	void serviceClient(QByteArray* parsedData);
	
	
	/*
	 * This slot is called for every cleanup timer expiry, in this slot, we need
	 * to call SmfDataStoreManager's API to refresh data store
	 */
	void timerExpired(){};
	
	/*
	 * This method is called every time timerExpired slot is trigerred
	 * Fetches the last saved requests through Transport Manager and Plugin Manager
	 * Who will save the last request (Tranport Manager or Data Store Manager) TBD later
	 * 
	 */
	void runSavedRequest(){};
	
	/*
	 * This slot is called when the data store updates are available as a result of
	 * "runSavedRequest()".
	 * Note:- The "10.4.1.11	updateDatastore" can be merged with this 
	 */
	void dataStoreUpdateAvailable(QByteArray* respData){Q_UNUSED(respData)};
	
	/**
	 * Server calls this method when it receives message from the CM
	 * that authentication keys for the pluginID has expired
	 */
	void authenticationKeysExpired(NotificationType type,SmfPluginID id);
signals:
  //None at the moment
private:
	//private impl
#ifdef Q_OS_SYMBIAN
	SmfServerSymbian* m_SmfServerPrivate;
#else
	SmfServerQt* m_SmfServerPrivate;
#endif
	//Handles of other SMF components
	SmfTransportManager* m_transportManager;
	SmfPluginManager* m_pluginManager;
	SmfDataStoreManager* m_dataStoreManager;
	SmfSettingsAuthManager* m_settingsAuthManager;
	SmfClient* m_smfClient;
	SmfCredMgrClient* m_credentialMngr;
};
#ifdef CLIENT_SERVER_TEST
class dummyPM : public QObject
	{
		Q_OBJECT
public:
		dummyPM(SmfServer* server,QObject* parent=0);
		~dummyPM();
		SmfError createRequest ( const quint32& aSessionID, 
				const QString& aPluginID, 
				const SmfRequestTypeID& aOperation, 
				QByteArray& aInputData );
public slots:
	void responseAvailable();
private:
		QTimer* m_timer;
		SmfServer* m_server;
	};
#endif

#endif // SMFSERVER_H
