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
 * The Plugin Manager class manages the loading and unloading of plug-ins
 *
 */

#ifndef SMFPLUGINMANAGER_H_
#define SMFPLUGINMANAGER_H_

#include <QtSql>
#include <smfglobal.h>
#include <smfserverglobal.h>
#include "smfserver.h"

// Forward declaration
class SmfPluginManagerUtil;
class SmfTransportManagerUtil;
class QFileSystemWatcher;
class QPluginLoader;
class SmfPluginBase;
class SmfProvider;

/**
 * Structure that holds information about a plugin that is awaiting a 
 * response from Transport Manager
 */
struct SmfWaitingPluginInfoStruc
	{
	/**
	 * The server session ID for this request
	 */
	quint32 iSessionID;
	/**
	 * The plugin Id who creates this request
	 */
	QString iPluginId;
	
	/**
	 * The instance of the loaded plugin
	 */
	SmfPluginBase *iInstance;
	
	/**
	 * The type of operation requested by the plugin
	 * @see smfglobal.h
	 */
	SmfRequestTypeID iOperation;
	
	/**
	 * The input data for this request (if request need to be send again)
	 */
	QByteArray iInputData;
	
	/**
	 * The list of valid Urls accessible for this plugin
	 */
	QList<QUrl> iUrlList;

	};


/**
 * The Plugin Manager class manages the loading and unloading of plug-ins
 */
class SmfPluginManager : public QObject
	{
	Q_OBJECT
	
public:
	/**
	 * Method to get the instance of SmfPluginManager class
	 * @param aServer The Smf server instance
	 * @return The instance of SmfPluginManager class
	 */
	static SmfPluginManager* getInstance ( SmfServer *aServer = NULL );
	
	/**
	 * Debugging only
	 */
	SmfServer* server ( ) { return m_server; }
	
	/**
	 * Destructor
	 */
	~SmfPluginManager ( );
	
public:
	/**
	 * Method called by Smf server to create a web query.
	 * Plugin Manager calls the appropriate web query creation method 
	 * using the aOperation and aInputData parameters. Once the web request 
	 * is ready, it calls the appropriate methods exposed by the Transport 
	 * Manager to send the network request.
	 * @param aSessionID The session ID provided by Smf Server
	 * @param aPluginID The plugin ID that need to perform this operation
	 * @param aOperation The type of operation to be performed
	 * @param aInputData The data required to create the web query
	 * @return SmfPluginManagerResult The result of the operation
	 * @see smfglobal.h
	 */
	SmfPluginManagerResult createRequest ( const quint32& aSessionID, 
			const QString& aPluginID, 
			const SmfRequestTypeID& aOperation, 
			QByteArray& aInputData );
	
	/**
	 * Method called by Transport Manager when network response 
	 * is available
	 * @param aTransportResult The result of Transport Operation
	 * @param aReply The QNetworkReply instance that requested 
	 * this transaction
	 * @param aResponse The network response data
	 */
	void responseAvailable ( const SmfTransportResult &aTransportResult, 
			QNetworkReply *aReply,
			QByteArray *aResponse );
	
	/**
	 * Method to cancel the service request
	 * @param aPluginId The plugin whose current operation 
	 * is to be cancelled
	 */
	bool cancelRequest ( const QString& aPluginId );
	
	/**
	 * Method to get the list of the SmfProvider for all the plugins that implement 
	 * the mentioned Interface 
	 * @param aInterface The interface for which list of plugins is required 
	 * @param aMap The map of pluginID and its corresponding SmfProvider
	 */
	void getPlugins ( const QString& aInterface, 
			QMap<QString,SmfProvider>& aMap );
	
	/**
	 * Method to get the pluginID for the mentioned interface and service provider 
	 * @param aInterface The interface implemented by the plugin
	 * @param aProv The plugin's service provider
	 * @param aPluginId The required pluginID
	 */
	void getPluginId ( const QString& aInterface, 
			const SmfProvider& aProv, QString& aPluginId );
	
private:
	/**
	 * Method called to initialize the file watcher watching the file  
	 * system for adition/upgradation/removal of plugins
	 */
	void initializeFileWatcher ( );
	
	/**
	 * Method called to initialize the database holding the plugin 
	 * directory sructure information. This is called only once when 
	 * the Plugin Manager is instantiated.
	 * This method creates and updates iPluginIdPathHash member 
	 * of this class
	 * @return Retuns true if success else false
	 */
	bool initializeSmfPluginDataBase ( );
	
	/**
	 * Method to load a plugin using its Plugin Id.
	 * @param aPluginId The unique ID of the plugin 
	 * @param aLoadResult [out] Output paramater indicating the result 
	 * of the loading
	 * @return The instance of the loaded plugin if loaded, else NULL
	 */
	QObject* load ( const QString &aPluginId, 
			SmfPluginManagerResult &aLoadResult);
	
	/**
	 * Method to unload a loaded plugin. Returns true if success, else 
	 * returns false.
	 * @param aPlugin The plugin instance to be unloaded
	 * @return Returns true if success, else returns false
	 */
	bool unload ( SmfPluginBase *aPlugin ); 
	
	/**
	 * Method to unload the list of loaded plugins. Returns true if all are 
	 * success, else returns false if any one fails.
	 * @param aPluginList The list of instances for all plugins that are 
	 * to be unloaded
	 * @return Returns true if all are success, else returns false if any 
	 * one fails.
	 */
	bool unload ( const QList<SmfPluginBase*> &aPluginList);
	
	/**
	 * Method that calls the Transport Manager Utility class method to 
	 * send the request created by the plugins over the network
	 * @param aReqData The request data created by the plugin
	 * @param aResult [out] The output parameter indicating the result 
	 * of this method
	 * @param aUrlList The list of accessible Urls for this plugin
	 * @see smfglobal.h
	 */
	void sendRequest ( SmfPluginRequestData &aReqData, 
			SmfPluginManagerResult &aResult,
			const QList<QUrl> &aUrlList );
	
	/**
	 * Method that checks if a plugin is authorised to make a request. 
	 * This method communicates with Credential and Settings Manager 
	 * through Smf server, giving the registration token and getting 
	 * the valid url list if available for this plugin.
	 * @param aRegToken The registration token given by the plugin
	 * @param aUrlList [out] The list of Urls that the plugin can send 
	 * request to (to be filled by CSM)
	 * @return Returns true if plugin is authorised, else returns false.
	 */
	bool authorisePlugin( const QString &aRegToken, 
			QList<QUrl> &aUrlList );
	
	/**
	 * Method to serialize the result of parsing (which is done by the 
	 * plugins) to QByteArray to be sent to Smf server.
	 * @param aOperation The type of operation to be performed
	 * @param aResult The data to be serialized
	 * @param aDataStream Stream to be written
	 */
	void serializeResult ( const SmfRequestTypeID &aOperation, 
			QVariant* aResult,
			QDataStream &aDataStream );
	
private slots:
	/**
	 * Method for the directoryChanged signal of QFileSystemWatcher.
	 * This will update the iPluginHash member and also the Plugin 
	 * Information List.
	 * @param aPath The path of the directory that has changed
	 */
	void directoryChanged ( const QString &aPath );

private:
	/**
	 * Constructor with default argument
	 * @param aServer The Smf server instance
	 */
	SmfPluginManager ( SmfServer *aServer );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfPluginManager ( const SmfPluginManager &aOther );
	
private:
	/**
	 * The single instance of SmfPluginManager
	 */
	static SmfPluginManager* m_myInstance;
	
	/**
	 * Member variable that maps the plugin instances to their 
	 * respective QPluginLoader instances (loaders may be multiple)
	 */
	QHash<SmfPluginBase*, QPluginLoader*> m_pluginLoaderHash;
	
	/**
	 * Member variable that watches the file system for 
	 * addition/upgradation/removal of plugins 
	 */
	QFileSystemWatcher *m_fileWatcher;
	
	/**
	 * Member variable that is an instance of a structure that 
	 * holds a loaded plugin information 
	 */
	SmfWaitingPluginInfoStruc *m_tempStruct;
	
	/**
	 * Member variable that maps a QNetworkReply instance to 
	 * its corresponding loaded plugin information structure  
	 */
	QHash<QNetworkReply*, SmfWaitingPluginInfoStruc*> m_waitingPluginHash;
	
	/**
	 * Member variable that maps a pluginId to its path
	 */
	QHash<QString, QString> m_pluginIdPathHash;
	
	/**
	 * Member variable that holds the instance of 
	 * SmfServer class
	 */
	SmfServer *m_server;
	
	/**
	 * Member variable that holds the instance of 
	 * SmfTransportManagerUtil class
	 */
	SmfTransportManagerUtil *m_transMngrUtil;

	/**
	 * Member variable that holds the instance of 
	 * SmfPluginManagerUtil class. SmfPluginManager is a friend of SmfPluginManagerUtil.
	 */
	SmfPluginManagerUtil *m_util;

	/**
	 * Member variable that holds QSqlDatabase instance.
	 */
	QSqlDatabase m_pluginDataBase;
	
	};

#endif /* SMFPLUGINMANAGER_H_ */
