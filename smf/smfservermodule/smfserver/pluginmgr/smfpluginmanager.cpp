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

#include <QDir>
#include <QLibraryInfo>
#include <QFileSystemWatcher>
#include <QPluginLoader>
#include <QList>
#include <smfpluginbase.h>
#include <smfpluginutil.h>
#include <smfprovider.h>

#include "smfpluginmanager.h"
#include "smfpluginmanagerutil.h"
#include "smftransportmanagerutil.h"

// Static data initialisation
SmfPluginManager* SmfPluginManager::m_myInstance = NULL;


/**
 * Method to get the instance of SmfPluginManager class
 * @param aServer The Smf server instance
 * @return The instance of SmfPluginManager class
 */
SmfPluginManager* SmfPluginManager::getInstance ( SmfServer *aServer)
	{
	if(NULL == m_myInstance)
		m_myInstance = new SmfPluginManager(aServer);
	return m_myInstance;
	}


/**
 * Constructor with default argument
 * @param aServer The Smf server instance
 */
SmfPluginManager::SmfPluginManager ( SmfServer *aServer )
	{
	// Save the server instance
	m_server = aServer;
	
	m_server->writeLog("Inside SmfPluginManager::SmfPluginManager()");
	
	// initialize the file watcher to monitor plugin addition/upgradation/removal
	initializeFileWatcher ( );
	
	// create a database to store the folder structure of the path "c://resource//qt//plugins"
	initializeSmfPluginDataBase ( );
	
	// create teh Plugin Manager utility class instance
	m_util = new SmfPluginManagerUtil(this);
	
	// Get handle to the Transport MAnager utility instance
	m_transMngrUtil = SmfTransportManagerUtil::getInstance();
	}


/**
 * Destructor
 */
SmfPluginManager::~SmfPluginManager ( )
	{
	m_server->writeLog("Inside SmfPluginManager::~SmfPluginManager()");
	if(m_fileWatcher)
		delete m_fileWatcher;
	
	if(m_tempStruct)
		delete m_tempStruct;
	
	if(m_waitingPluginHash.count() > 0)
		{
		foreach(SmfWaitingPluginInfoStruc *str, m_waitingPluginHash.values())
			delete str;
		}
	
	if(m_util)
		delete m_util;
	
	if(m_pluginDataBase.isOpen())
		{
		m_pluginDataBase.close();
		m_pluginDataBase.removeDatabase("SmfPluginsInfoDatabase");
		}
	
	// unload all loaded plugins
	unload(m_pluginLoaderHash.keys());
	
	if(m_myInstance)
		delete m_myInstance;
	}


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
SmfPluginManagerResult SmfPluginManager::createRequest ( const quint32& aSessionID, 
		const QString& aPluginID, 
		const SmfRequestTypeID& aOperation, 
		QByteArray& aInputData )
	{
	m_server->writeLog("Inside SmfPluginManager::createRequest()");
	
	SmfPluginManagerResult result = SmfPluginUnknownError;

	// Load the plugin
	QObject *plugin = load(aPluginID, result);
	
	// Check if plugin is loaded
	if(plugin && (SmfPluginLoaded == result))
		{
		m_server->writeLog("Plugin loaded");
		SmfPluginBase* instance = qobject_cast<SmfPluginBase *>(plugin);
		
		if(instance)
		{
		// Get the registration token of the plugin
		QString regToken = instance->getProviderInfo()->smfRegistrationId();
		QList<QUrl> urlList;
		SmfPluginRequestData reqData;

		// check if the plugin is authorised (with CSM)
		if( authorisePlugin(regToken, urlList ))
			{
			m_server->writeLog("Plugin authorised");
			
			// call the utility method to create plugin specific request
			m_util->createRequest(plugin, aOperation, aInputData, reqData, result);

			// If the request is created successfully, call the TM method to send the request
			if( SmfPluginRequestCreated == result )
				{
				m_server->writeLog("Plugin request creation successful");
				m_tempStruct = new SmfWaitingPluginInfoStruc();
				m_tempStruct->iSessionID = aSessionID;
				m_tempStruct->iPluginId = aPluginID;
				m_tempStruct->iInstance = instance;
				m_tempStruct->iOperation = aOperation;
				m_tempStruct->iInputData = aInputData;
				m_tempStruct->iUrlList = urlList;
			
				// send the request
				sendRequest ( reqData, result, urlList );
				}
			}
		
		else
			{
			// plugin not authorised, so unload
			m_server->writeLog("Plugin not authorised!!!");
			unload(instance);
			result = SmfPluginNotAuthorised;
			}
		}
		else
			{
			// plugin instance cannot be casted, so unload
			m_server->writeLog("Plugin instance cannot be casted to SmfPluginBase*!!!");
			unload(instance);
			result = SmfPluginLoadError;
			}
		}

	else
		{
		// plugin not loaded
		m_server->writeLog("Plugin not loaded!!!");
		result = SmfPluginLoadError;
		}
	
	return result;
	}


/**
 * Method called by Transport Manager when network response is available
 * @param aTransportResult The result of Transport Operation
 * @param aReply The QNetworkReply instance that requested 
 * this transaction
 * @param aResponse The network response data, may be NULL for error
 */
void SmfPluginManager::responseAvailable ( 
		const SmfTransportResult &aTransportResult, 
		QNetworkReply *aReply,
		QByteArray *aResponse )
	{
	m_server->writeLog("Inside SmfPluginManager::responseAvailable()");
		
	// For request success or For request cancellation
	if((SmfTransportOpNoError == aTransportResult) || 
			(SmfTransportOpOperationCanceledError == aTransportResult))
			{
		m_server->writeLog("no transport error/ cancellation");

			// get the details of the plugin which made this request
			SmfWaitingPluginInfoStruc* info = m_waitingPluginHash.value(aReply);
			quint32 sessionId = m_waitingPluginHash.value(aReply)->iSessionID;
			QString pluginId = m_waitingPluginHash.value(aReply)->iPluginId;
			SmfRequestTypeID operation = m_waitingPluginHash.value(aReply)->iOperation;
			QByteArray inputData = m_waitingPluginHash.value(aReply)->iInputData;
			
			QVariant result;
			SmfPluginRetType retType = SmfRequestError;
			SmfResultPage pageResult;
			
			m_server->writeLog("Before m_util->responseAvailable");
			
			// call the utility method to send response to appropriate plugins
			SmfPluginManagerResult retValue = m_util->responseAvailable( info->iInstance, operation, 
				aTransportResult, aResponse, &result, retType, pageResult );
			
			// remove the plugin from the waiting list
			delete m_waitingPluginHash.value(aReply);
			m_waitingPluginHash.remove(aReply);
		
			QByteArray arr;
			QDataStream stream(&arr, QIODevice::ReadWrite);
			if( SmfPluginResponseParsed == retValue )
				{
				m_server->writeLog("Parsing successful");
				
				// serialize the response to suitable class and pass the data to server
				serializeResult(operation, &result, stream);
			
				////TODO:- send error in enums-by manasij
				// Send the response data to the server
				m_server->resultsAvailable(sessionId, &arr, SmfNoError);
				}
		
			// Send the request again
			else if( SmfPluginSendRequestAgain == retValue )
				{
				m_server->writeLog("Send request again");
				
				// create the request again (with same paramaters)
				retValue = createRequest( sessionId, pluginId, operation, inputData );
				}
			
			// Error
			else
				{
				m_server->writeLog("Parsing failed!!");
				
				// Error in parsing, sent to server
				m_server->resultsAvailable(sessionId, &arr, SmfpluginResponseParseFailure);
				}
		
			// delete aReply later, when event loop is re-entered
			aReply->deleteLater();
			}
	
	// Any other error
	else
		{
		m_server->writeLog("Error in SmfPluginManager::responseAvailable, Transport failure code : ");
		QString err = QString::number(aTransportResult);
		m_server->writeLog(err);
		
		//Added by manasij, send all kind of errors to the server
		//TODO:- to be refined by PM owner
		quint32 sessionId = m_waitingPluginHash.value(aReply)->iSessionID;
		QByteArray arr;
		m_server->resultsAvailable(sessionId, &arr, SmftransportInitNetworkNotAvailable);
		}
	}


/**
 * Method to cancel the service request
 * @param aPluginId The plugin whose current operation 
 * is to be cancelled
 */
bool SmfPluginManager::cancelRequest ( const QString& aPluginId )
	{
	bool retValue = false;
	m_server->writeLog("Inside SmfPluginManager::cancelRequest()");
	
	// Get the plugin for which cancel is requested
	foreach(SmfWaitingPluginInfoStruc* iPluginInfo, m_waitingPluginHash.values())
		{
		if( 0 == iPluginInfo->iPluginId.compare(aPluginId))
			{
			m_server->writeLog("Plugin to be cancelled found in the waiting list");
			
			// Notify Transport Manager
			m_transMngrUtil->cancelRequest(m_waitingPluginHash.key(iPluginInfo));
			
			// Notify the plugin that the request has been cancelled
			SmfPluginRetType retType;
			SmfResultPage pageResult;
			SmfPluginError ret = iPluginInfo->iInstance->responseAvailable( SmfTransportOpCancelled, 
					NULL, NULL, retType, pageResult );
			
			// Remove that plugin from the waiting list
			delete (m_waitingPluginHash.value(m_waitingPluginHash.key(iPluginInfo)));
			m_waitingPluginHash.remove(m_waitingPluginHash.key(iPluginInfo));
			
			if(SmfPluginErrNone == ret)
				retValue = true;
			}
		//else , cancel requested for a plugin which is not loaded, do nothing
		else
			{
			m_server->writeLog("Plugin to be cancelled not found in the waiting list!!! - do nothing");
			retValue =  false;
		}
		}
	return retValue;
	}


/**
 * Method called to initialize the file watcher watching the file  
 * system for adition/upgradation/removal of plugins
 */
void SmfPluginManager::initializeFileWatcher ( )
	{
	m_server->writeLog("Inside SmfPluginManager::initializeFileWatcher()");
	
	// Create the file watcher for the plugins in /Smf folder of the Qt plugin directory
	m_fileWatcher = new QFileSystemWatcher(this);
	
	// Get the directory having the Qt plugin stubs
	QDir dir(QLibraryInfo::location(QLibraryInfo::PluginsPath));
	
	// If Smf folder exists
	if(dir.cd("smf/plugin"))
		{
		// Add each service provider folders to the file watcher
		foreach(QString folder, dir.entryList(QDir::AllDirs))
			{
			dir.cd(folder);
			m_fileWatcher->addPath(dir.absolutePath());
			dir.cdUp();
			}
		}
	else
		m_fileWatcher->addPath(dir.absolutePath());
	}



/**
 * Method called to initialize the database holding the plugin 
 * directory sructure information. This is called only once when 
 * the Plugin Manager is instantiated.
 * This method creates and updates m_pluginIdPathHash member 
 * of this class
 */
bool SmfPluginManager::initializeSmfPluginDataBase ( )
	{
	m_server->writeLog("Inside SmfPluginManager::initializeSmfPluginDataBase()");
	
	// Find QSQLite driver and create a connection to database
	m_pluginDataBase.removeDatabase("SmfPluginsInfoDatabase");
	m_pluginDataBase = QSqlDatabase::addDatabase("QSQLITE");
	m_pluginDataBase.setDatabaseName("SmfPluginsInfoDatabase");
	
	// Open the database
	bool opened = m_pluginDataBase.open();
	if(!opened)
		return false;
	
	m_server->writeLog("Database opened");
	
	// Create a query to create the DB table for Plugin Manager (if it doesn't exists)
	QSqlQuery query;
	bool tableCreated = false;

	tableCreated = query.exec("CREATE TABLE IF NOT EXISTS pluginDetails ("
			"pluginId TEXT PRIMARY KEY, interfaceName TEXT, serviceProvider TEXT, "
			"description TEXT, serviceUrl TEXT, authAppId TEXT)");

	// Error - table not created, Plugin Manager might not work properly
	if(!tableCreated)
		{
		m_server->writeLog("Table not created, error = "+query.lastError().text());
		return false;
		}
	
	m_server->writeLog("Table created");
	
	// Get the directory having the Qt plugin stubs
	QDir dir(QLibraryInfo::location(QLibraryInfo::PluginsPath));
	
	// If Smf folder exists
	if(dir.cd("smf/plugin"))
		{
		m_server->writeLog("Smf/plugin folder exists");
		// Get each interface folders names
		foreach(QString intfName, dir.entryList(QDir::AllDirs))
			{
			dir.cd(intfName);
			m_server->writeLog("Interface name : "+dir.dirName());
			
			// Get each plugin in this folder
			foreach(QString pluginName, dir.entryList(QDir::Files))
				{
				m_server->writeLog("plugins for this Interface : "+pluginName);
				
				// load this plugin
				QPluginLoader pluginLoader(dir.absoluteFilePath(pluginName));
				QObject *instance = pluginLoader.instance();
				if (instance)
					{
					m_server->writeLog("instance found");
					SmfPluginBase* plugin = qobject_cast<SmfPluginBase *>(instance);
				    if (plugin)
				    	{
						m_server->writeLog("SmfPluginBase found");
						plugin->initialize(SmfPluginUtil::getInstance());
				    
						// get the plugin id
						QString id = plugin->getProviderInfo()->pluginId();
						
						// get the interface implemented by the plugin
						QString intfImplemented = dir.dirName();
						intfImplemented.prepend("org.symbian.smf.plugin.");
						m_server->writeLog("intfImplemented=");
						m_server->writeLog(intfImplemented);
						
						// get the service provider
						QString serProv = plugin->getProviderInfo()->serviceName();
						
						//get the description
						QString desc = plugin->getProviderInfo()->description();
						
						// get the service URL
						QString servURL = plugin->getProviderInfo()->serviceUrl().toString();
						
						// get the authentication application id
						QString str;
						QStringList list;
						QString authAppId = plugin->getProviderInfo()->authenticationApp(
								str, list, QIODevice::ReadWrite);
						
						// Update m_pluginIdPathHash
						m_pluginIdPathHash.insert(id, dir.absoluteFilePath(pluginName));
						
						QSqlQuery rowInsertQuery;
						
						// insert into database
						bool rowInserted = rowInsertQuery.exec(QString("INSERT INTO pluginDetails VALUES ('%1', "
								"'%2', '%3', '%4', '%5', '%6')").arg(id).arg(intfImplemented).arg(serProv)
								.arg(desc).arg(servURL).arg(authAppId));
						
						if(rowInserted)
							m_server->writeLog("This Plugin's information is added to database : "+id+""
									", "+intfImplemented+", "+serProv);
						else
							m_server->writeLog("plugins data not written to database, error = "+query.lastError().text());
				    	}
				    else
				    	{
						m_server->writeLog("Plugin could not be converted to SmfpluginBase* - returning");
						
						// Close the database
						m_pluginDataBase.close();
						
				    	return false;
				    	}
				    
				    pluginLoader.unload();
					}
				else
					{
					m_server->writeLog("Plugin could not be loaded - returning");
					
					// Close the database
					m_pluginDataBase.close();
										
					return false;
				}
				}
			dir.cdUp();
			}
		}
	else
		m_server->writeLog("No Smf plugins installed!!!");
	
	// Close the database
	m_pluginDataBase.close();
	
	return true;
	}



/**
 * Method to load a plugin using its Plugin Id.
 * @param aPluginId The unique ID of the plugin 
 * @param aLoadResult [out] Output paramater indicating the result 
 * of the loading
 * @return The instance of the loaded plugin if loaded, else NULL
 */
QObject* SmfPluginManager::load ( const QString &aPluginId,
		SmfPluginManagerResult &aLoadResult)
	{
	m_server->writeLog("Inside SmfPluginManager::load()");
	
	QPluginLoader *pluginLoader;
	
	// Find the plugin Path
	QString pluginPath = m_pluginIdPathHash.value(aPluginId);
	
	if(!pluginPath.isEmpty())
		{
		// create the plugin loader and load the plugin
		pluginLoader = new QPluginLoader(pluginPath);
		SmfPluginBase *plugin = qobject_cast<SmfPluginBase *>(pluginLoader->instance());
		
		// If the plugin is loaded
		if( pluginLoader->isLoaded() && plugin )
			{
			// Initialize the plugin
			plugin->initialize(SmfPluginUtil::getInstance());
			
			// update the plugin loader and the loaded plugin lists
			m_pluginLoaderHash.insertMulti(plugin, pluginLoader);
			aLoadResult = SmfPluginLoaded;
			m_server->writeLog("Plugin loaded");
			}
		else
			{
			m_server->writeLog("Plugin not loaded");
		
			// Plugin could not be loaded, error
			aLoadResult = SmfPluginNotLoaded;
		}
		}
	else
		{
		m_server->writeLog("Plugin not found");
		
		// plugin not found in hash
		aLoadResult = SmfPluginNotFound;
		}
	
	return pluginLoader->instance();
	}


/**
 * Method to unload a loaded plugin. Returns true if success, else 
 * returns false.
 * @param aPlugin The plugin instance to be unloaded
 * @return Returns true if success, else returns false
 */
bool SmfPluginManager::unload ( SmfPluginBase *aPlugin )
	{
	m_server->writeLog("Inside SmfPluginManager::unload()");
	
	// Get all the loaders for this plugin
	QList<QPluginLoader*> loaderList = m_pluginLoaderHash.values(aPlugin);
	bool unloaded = false;

	foreach(QPluginLoader *loader, loaderList)
		{
		// for each loader unload the plugin
		unloaded = loader->unload();
		
		// delete the instance of the loader
		delete loader;
		loader = NULL;
		}
	
	// Remove the plugin and its associated loaders from the Hash
	m_pluginLoaderHash.remove(aPlugin);
	return unloaded;
	}


/**
 * Method to unload the list of loaded plugins. Returns true if all are 
 * success, else returns false if any one fails.
 * @param aPluginList The list of instances for all plugins that are 
 * to be unloaded
 * @return Returns true if all are success, else returns false if any 
 * one fails.
 */
bool SmfPluginManager::unload ( const QList<SmfPluginBase *> &aPluginList)
	{
	m_server->writeLog("Inside SmfPluginManager::unload() - overloaded fn");
	//unload all the required plugins
	bool unloaded = true;
	
	foreach(SmfPluginBase *plugin, aPluginList)
		{
		// unload individual plugins in the list
		bool ret = unload(plugin);
		
		// indicate error if any one of the plugin failed to unload
		if(!ret)
			unloaded = ret;
		}
	return unloaded;
	}


/**
 * Method that calls the Transport Manager Utility class method to 
 * send the request created by the plugins over the network
 * @param aReqData The request data created by the plugin
 * @param aResult [out] The output parameter indicating the result 
 * of this method
 * @param aUrlList The list of accessible Urls for this plugin
 * @see smfglobal.h
 */
void SmfPluginManager::sendRequest ( SmfPluginRequestData &aReqData, 
		SmfPluginManagerResult &aResult,
		const QList<QUrl> &aUrlList )
	{
	m_server->writeLog("Inside SmfPluginManager::sendRequest()");
	
	QNetworkReply* reply;
	bool sopCompliant = false;
	
	// Check the type of Http operation to be performed
	switch(aReqData.iHttpOperationType)
		{
		// Http HEAD
		case QNetworkAccessManager::HeadOperation:
			reply = m_transMngrUtil->head(aReqData.iNetworkRequest, aUrlList, sopCompliant);
			break;
		
		// Http GET
		case QNetworkAccessManager::GetOperation:
			reply = m_transMngrUtil->get(aReqData.iNetworkRequest, aUrlList, sopCompliant);
			break;
			
		// Http PUT	
		case QNetworkAccessManager::PutOperation:
			reply = m_transMngrUtil->put(aReqData.iNetworkRequest, aReqData.iPostData->buffer(), aUrlList, sopCompliant);
			break;
			
		// Http POST
		case QNetworkAccessManager::PostOperation:
			reply = m_transMngrUtil->post(aReqData.iNetworkRequest, aReqData.iPostData->buffer(), aUrlList, sopCompliant);
			break;
			
		// Http DELETE
		case QNetworkAccessManager::DeleteOperation:
			reply = m_transMngrUtil->deleteResource(aReqData.iNetworkRequest, aUrlList, sopCompliant);
			break;
			
		default:
			aResult = SmfPluginUnknownService;
			return;
		}
	
	if( sopCompliant )
		{
		if( reply )
			{
			// SOP compliant, sending successful
			m_waitingPluginHash.insert(reply, m_tempStruct);
			m_tempStruct = NULL;
			aResult = SmfPluginNoError;
			
			m_server->writeLog("No error, request sent");
			
			}
		// reply is NULL, sending failed
		else
			{
			m_server->writeLog("QNEtrworkReply returned error - not sent");
			aResult = SmfPluginRequestSendingFailed;
		}
		}
	
	// SOP violation
	else
		{
		m_server->writeLog("SOP checking failed");
		aResult = SmfPluginSOPCheckFailed;
	}
	}


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
bool SmfPluginManager::authorisePlugin( const QString &aRegToken, 
		QList<QUrl> &aUrlList )
	{
	Q_UNUSED(aRegToken)
	m_server->writeLog("Inside SmfPluginManager::authorisePlugin()");
	
#ifdef CSM_INTEGRATED
// Get the valid URL list from CSM, giving the reg token
	if(m_server->authorisePlugin(aRegToken, aUrlList))
		return true;
	else
		return false;
	
#else
// CSM STUBBING - start
	QUrl url1 ("http://www.example.com");
	QUrl url2 ("http://api.facebook.com");
	QUrl url3 ("http://api.flickr.com");

	aUrlList.append(url1);
	aUrlList.append(url2);
	aUrlList.append(url3);

	return true;
// CSM STUBBING - end
#endif

	}


/**
 * Method to serialize the result of parsing (which is done by the 
 * plugins) to QByteArray to be sent to Smf server.
 * @param aOperation The type of operation to be performed
 * @param aResult The data to be serialized
 * @param aDataStream Stream to be written
 */
void SmfPluginManager::serializeResult ( 
		const SmfRequestTypeID &aOperation, 
		QVariant* aResult,
		QDataStream &aDataStream )
	{
	m_server->writeLog("Inside SmfPluginManager::serializeResult()");
	
	// Call the utlity class method to serialize the result
	m_util->serializeResult(aOperation, aResult, aDataStream);
	}


/**
 * Method for the directoryChanged signal of QFileSystemWatcher.
 * This will update the iPluginHash member and also the Plugin 
 * Information List.
 * @param aPath The path of the directory that has changed
 */
void SmfPluginManager::directoryChanged ( const QString &aPath )
	{
	m_server->writeLog("Inside SmfPluginManager::directoryChanged()");
	
	// Create a QDir instance with the given path
	QDir dir(aPath);
	QString pluginId;
	QString oldpluginId;
	QString interfaceName;
	QString serviceProv;
	QString authAppId;

	// Get all the files in the directory at a specified path(sorted)
	QStringList newPlugins = dir.entryList(QDir::Files, QDir::Name);
	QStringList::const_iterator newListIterator = newPlugins.constBegin();
	
	// Get all plugins who were in this path, before this directory was changed
	QStringList availablePlugins = m_pluginIdPathHash.keys(aPath);
	availablePlugins.sort();
	QStringList::const_iterator oldListIterator = availablePlugins.constBegin();
	
	// Open the database
	bool opened = m_pluginDataBase.open();
	if(!opened)
		;//return;
	
	// If plugin is changed
	if( newPlugins.count() == availablePlugins.count() )
		{
		// Check for equality
		while( newListIterator != newPlugins.constEnd() )
			{
			if( *newListIterator == *oldListIterator )
				{
				newListIterator++;
				oldListIterator++;
				}
			else
				break;
			}
		// replace *oldListIterator with *newListIterator
		QHash<QString, QString>::iterator i = m_pluginIdPathHash.find(*oldListIterator);
		m_pluginIdPathHash.insert(*newListIterator, i.value());
		m_pluginIdPathHash.remove(*oldListIterator);
		
		// Also update database with *newListIterator
	    QSqlQuery updateQuery;
	    
	    bool updated = updateQuery.exec(QString("UPDATE pluginDetails SET pluginId = '%1' "
	    		"WHERE pluginId = '%2'").arg(*newListIterator).arg(*oldListIterator));
	    if (!updated)
	    	m_server->writeLog("Database table not updated, error = "+updateQuery.lastError().text());
		
		
		// Get the new and old plugin Ids
		pluginId = *newListIterator;
		oldpluginId = *oldListIterator;
		
		// Load the plugin and get its service provider name
		SmfPluginManagerResult result;
		SmfPluginBase* instance = qobject_cast<SmfPluginBase *>(load(pluginId, result));
		
		if(instance && (SmfPluginLoaded == result))
			{
			instance->initialize(SmfPluginUtil::getInstance());
			serviceProv = instance->getProviderInfo()->serviceName();
			interfaceName = dir.dirName();
			}
		
		unload(instance);
		// Inform server that plugin has been changed
#ifdef CSM_INTEGRATED
		//Remove after Server Integration
		m_server->pluginChanged(oldPluginId, newPluginId, interfaceName, serviceProv);
#endif
		}
		
	// If plugin is added
	else if(newPlugins.count() > availablePlugins.count())
		{
		// Check for equality
		while( oldListIterator != availablePlugins.constEnd() )
			{
			if( *newListIterator == *oldListIterator )
				{
				newListIterator++;
				oldListIterator++;
				}
			else
				break;
			}
		// replace *oldListIterator with *newListIterator
		m_pluginIdPathHash.insert(*newListIterator, aPath);
		
		// Get the plugin Id
		pluginId = *newListIterator;
		
		// Load the plugin and get its service provider name
		SmfPluginManagerResult result;
		SmfPluginBase* instance = qobject_cast<SmfPluginBase *>(load(pluginId, result));
		
		if(instance && (SmfPluginLoaded == result))
			{
			instance->initialize(SmfPluginUtil::getInstance());
			serviceProv = instance->getProviderInfo()->serviceName();
			interfaceName = dir.dirName();
			interfaceName.prepend("org.symbian.smf.plugin.");
			QString prgm;
			QStringList list;
			authAppId = instance->getProviderInfo()->authenticationApp(prgm, list, QIODevice::ReadWrite);
			}
		
		unload(instance);
		
		// Also add to the database the value newListIterator and aPath
		QSqlQuery insertRowQuery;
		bool rowInserted = insertRowQuery.exec(QString("INSERT INTO pluginDetails VALUES "
				"('%1', '%2', '%3', '%4')").arg(pluginId).arg(interfaceName).arg(serviceProv).arg(authAppId));

		 // Error
		if (!rowInserted)
			m_server->writeLog("Database table not inserted, error = "+insertRowQuery.lastError().text());
		
		// Inform server that plugin has been added
#ifdef CSM_INTEGRATED
		//Remove after Server Integration
		m_server->pluginAdded(pluginId, interfaceName, serviceProv);
#endif
		}
	
	// If plugin is removed
	else //for newPlugins.count() < availablePlugins.count()
		{
		// Check for equality
		while( newListIterator != newPlugins.constEnd() )
			{
			if( *newListIterator == *oldListIterator )
				{
				newListIterator++;
				oldListIterator++;
				}
			else
				break;
			}
		// remove *oldListIterator
		m_pluginIdPathHash.remove(*oldListIterator);
		
		// Also remove oldListIterator from the database
		QSqlQuery deleteRowQuery;
		bool rowDeleted = deleteRowQuery.exec(QString("DELETE FROM pluginDetails WHERE pluginId = '%1'")
				.arg(*oldListIterator));
		 
		// Error
		if (!rowDeleted)
			m_server->writeLog("Database table row not deleted, error = "+deleteRowQuery.lastError().text());
		
		// Get the plugin Id
		pluginId = *oldListIterator;
		
		// Load the plugin and get its service provider name
		SmfPluginManagerResult result;
		SmfPluginBase* instance = qobject_cast<SmfPluginBase *>(load(pluginId, result));
		
		if(instance && (SmfPluginLoaded == result))
			{
			instance->initialize(SmfPluginUtil::getInstance());
			serviceProv = instance->getProviderInfo()->serviceName();
			interfaceName = dir.dirName();
			}
		
		unload(instance);
		// Inform server that plugin has removed
#ifdef CSM_INTEGRATED
		//Remove after Server Integration
		m_server->pluginRemoved(pluginId, interfaceName, serviceProv);
#endif
		}
	// Close the database
	m_pluginDataBase.close();
	}


/**
 * Method to get the list of the SmfProvider for all the plugins that implement 
 * the mentioned Interface 
 * @param aInterface The interface for which list of plugins is required 
 * @param aMap The map of pluginID and its corresponding SmfProvider
 */
void SmfPluginManager::getPlugins(const QString& aInterface, QMap<QString,SmfProvider>& aMap)
	{
	m_server->writeLog("Inside SmfPluginManager::getPlugins()");
	
	// Open the database
	bool opened = m_pluginDataBase.open();
	if(!opened)
		{
		m_server->writeLog("Data base not opened, exiting getplugins()");
		return;
		}
	
	m_server->writeLog("Data base opened");
	
	// Query the database for all pluginIDs that implement the given interface
	QSqlQuery query(QString("SELECT pluginId, interfaceName, serviceProvider, description, "
			"serviceUrl FROM pluginDetails where interfaceName = '%1'").arg(aInterface));
	
	if (query.next())
		{
		m_server->writeLog("Query is success");

		SmfProvider prov;
		
		// get the pluginId
		QString pluginId = query.value(0).toString();
		
		// get the service type / interface name
		QStringList servicetypes;
		servicetypes.insert(0, query.value(1).toString());
		prov.serviceTypes(servicetypes);
		
		// Get the serv provider
		QString servName = query.value(2).toString();
		prov.serviceName(servName);
		
		// Get the description
		QString desc = query.value(3).toString();
		prov.description(desc);
		
		// Get the service URL
		QUrl url(query.value(4).toString());
	prov.serviceUrl(url);

		aMap.insert(pluginId, prov);
		}
	else
		m_server->writeLog("Data base query->next() returned false, error = "+query.lastError().text());
	
	// Close the database
	m_pluginDataBase.close();
	}


/**
 * Method to get the pluginID for the mentioned interface and service provider 
 * @param aInterface The interface implemented by the plugin
 * @param aProv The plugin's service provider
 * @param aPluginId The required pluginID
 */
void SmfPluginManager::getPluginId(const QString& aInterface, const SmfProvider& aProv, QString& aPluginId)
	{
	m_server->writeLog("SmfPluginManager::getPluginId");
	
	// Open the database
	bool opened = m_pluginDataBase.open();
	if(!opened)
		{
		m_server->writeLog("Data base not opened, exiting");
		return;
		}
	
	m_server->writeLog("Data base opened");

	// Query the database for a pluginID with given interface name and service provider
	QSqlQuery query(QString("SELECT pluginId FROM pluginDetails where interfaceName = '%1' AND "
			"serviceProvider = '%2'").arg(aInterface).arg(aProv.serviceName()));
	
	if (query.next())
		{
		m_server->writeLog("Query is success");
		
		// get the pluginId
		aPluginId = query.value(0).toString();
		}
	else
		m_server->writeLog("Data base query->next() returned false, error = "+query.lastError().text());
	
	m_server->writeLog("returned pluginID = "+aPluginId);
	
	// Close the database
	m_pluginDataBase.close();
	}
