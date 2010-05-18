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
/*
* Forward declarations
* Other components of the SMF
*/
class SmfTransportManager;
class SmfPluginManager;
class SmfDataStoreManager;
class SmfSettingsAuthManager;
class SmfClient;

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
  
  ~SmfServer(){}

public:
  /*
   * Request the Credential and Settings manager to check whether client has
   * been authorized previously.
   * SmfClientAuthID may be same as SID of the client which can be retrieved
   * if using Symbian Client-Server private implementation. Not supported for
   * other platforms
   */
  bool isClientAuthorized(SmfClientAuthID clientID);
  
  /*
   * In case the client is yet to be authorized, it starts the authorization
   * process by triggering Credential Manager.
   * The SLOT clientAuthorizationFinished is called once its done.
   * SmfClientAuthID may be same as SID of the client which can be retrieved
   * if using Symbian Client-Server private implementation. Not supported for
   * other platforms.
   */
  void authorizeClient(SmfClientAuthID clientID);
  
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
  
  /*
   * Prepares the transport.
   * What it'll do is not yet clear
   */
  void prepareTransport();
  /**
   * DEbugging purpose only
   */
  void writeLog(QString log) const;
  /*
   * Delegates the request of the client (to get the data from SN sites)
   * to the Plugin Manager.
   * Once the Plugin Manager gets parsed response data it calls the SLOT
   * resultsAvailable(). 
   * pluginID is the Plugin to be used.
   * requestTypeID is the function codes(opcodes) used for message passing 
   * betn clien-server.
   * Note:- Should there be an overloaded function which takes
   * list of SmfPluginID ?
   * Note:- SmfPluginManager will invoke resultsAvailable on session object once
   * it receives parsed data.
   */
#ifdef Q_OS_SYMBIAN
  /**
   * Request the Plugin manager to get the data.
   * @param requestID Corresponds to a client's session
   * @param pluginID Plugin for which the request is intended
   * @param interfaceID Interface name
   * @param dataForPlugin Data to be sent for this request
   * 
   */
  void getRequestedData(int requestID,SmfPluginID pluginID,SmfInterfaceID interfaceID, SmfRequestTypeID requestTypeID,QByteArray dataForPlugin = QByteArray());
#else
  void getRequestedData(SmfServerQtSession* session,SmfPluginID pluginID, SmfRequestTypeID requestTypeID);
#endif
  
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
	 * Request the CM to get the authentication keys for the given pluginID
	 */
	void getAuthenticationKeys(int pluginID,QStringList& keys,QStringList& urls);
	
	/**
	 * Server calls this method when it receives message from the CM
	 * that authentication keys for the pluginID has expired
	 */
	void authenticationKeysExpired(NotificationType type,SmfPluginID id);
signals:
  //None at the moment
private:
/*
 * Starts the clean-up timer for data store refresh, called from the "startServer()"
 * timeOutValue should be picked out from SmfSettingsAuthManager's API
 */
bool startCleanupTimer(int timeOutValue){Q_UNUSED(timeOutValue) return true;};


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
};


#endif // SMFSERVER_H
