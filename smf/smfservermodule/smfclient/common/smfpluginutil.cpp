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
 * The Plugin util class provides the information required for the 
 * plugins from Smf framework 
 *
 */

#include <smfpluginutil.h>

// Static data initialisation
SmfPluginUtil* SmfPluginUtil::m_myInstance = NULL;

/**
 * Method to get the instance of SmfPluginUtil class
 * @return The instance of SmfPluginUtil class
 */
SmfPluginUtil* SmfPluginUtil::getInstance ( )
	{
	if(NULL == m_myInstance)
		m_myInstance = new SmfPluginUtil();
	return m_myInstance;
	}

/**
 * Constructor with default argument
 */
SmfPluginUtil::SmfPluginUtil ( )
	{
	m_jsonParser = new QJson::Parser;
	}
	
/**
 * Destructor
 */
SmfPluginUtil::~SmfPluginUtil ( )
	{
	if(m_jsonParser)
		delete m_jsonParser;
	
	if(m_myInstance)
		delete m_myInstance;
	}
	
/**
 * Method called by plugins to get the handle to QJson library 
 * @return The QJson handle
 */
QJson::Parser* SmfPluginUtil::getJsonHandle( void )
	{
	return m_jsonParser;
	}

/**
 * Method called by plugins to get the OAuth Keys. The PM sends the 
 * keys only if the pluginID is that of a currently loaded plugin
 * @param aKeys [out] The OAuth keys
 * @param aRegToken The plugin's registration token
 * @param aPluginID The ID of the plugin that requests for 
 * the OAuth keys
 */
void SmfPluginUtil::getAuthKeys( QMap<QString, QString> &aKeys, 
		const QString aRegToken, 
		const QString aPluginID )
	{
	//// Call Credential manager function to get the keys passing the 
	//// registration token and the plugin
	}

/**
 * Method called by plugins to get the Nonce value
 * @param aNonceString [out] The Nonce string
 */
void SmfPluginUtil::getNonce( QString &aNonceString )
	{
	//// Call Credential manager function to get the nonce string
	}

/**
 * Method called by plugins to generate a parameters string required to 
 * access Protected Resources using OAuth authorization.
 * @param aRequestUrl The request URL
 * @param aOperation The type of http operation
 * @param aToken The access token
 * @param aTokenSecret The token secret
 * @param aSignatureMethod The signature method to be used
 * @param aParams A map of parameters to its values
 * @param aMode The mode of creation of the request
 * @return The cretd parameter string
 */
QByteArray SmfPluginUtil::createParameterString( const QString &aRequestUrl, 
		QNetworkAccessManager::Operation aOperation, 
		const QByteArray &aToken, 
		const QByteArray &aTokenSecret, 
		const SmfSignatureMethod aSignatureMethod, 
		const QMultiMap<QByteArray, QByteArray> &aParams, 
		const SmfParsingMode aMode)
	{
	//// Call Credential manager function to create and sign the parameter string
	}

