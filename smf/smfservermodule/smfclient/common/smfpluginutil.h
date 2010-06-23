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

#ifndef SMFPLUGINUTIL_H_
#define SMFPLUGINUTIL_H_

#include <QNetworkAccessManager>
#include <QMap>
#include <QVariant>
#include <parser.h>
#include <smfclientglobal.h>

enum SmfSignatureMethod
	{
	HMAC_SHA1,
	RSA_SHA1,
	PLAINTEXT
	};

enum SmfParsingMode
	{
	ParseForRequestContent, //Inline query format (foo=bar&bar=baz&baz=foo ...), suitable for POST requests.
	ParseForInlineQuery, // Same as ParseForRequestContent, but prepends the string with a question mark - 
						// suitable for GET requests (appending parameters to the request URL) 
	ParseForHeaderArguments // HTTP request header format (parameters to be put inside a request header). 
	};

/**
 * The Plugin util class provides the information required for the 
 * plugins from Smf framework
 */
class SMFCLIENT_EXPORT SmfPluginUtil
	{
public:
	/**
	 * Method to get the instance of SmfPluginUtil class
	 * @return The instance of SmfPluginUtil class
	 */
	static SmfPluginUtil* getInstance ( );
	
	/**
	 * Destructor
	 */
	~SmfPluginUtil ( );
	
	/**
    * Read JSON string from the I/O Device and converts it to a QVariant object
    * @param io Input output device
    * @param ok if a conversion error occurs, *ok is set to false; otherwise *ok is set to true.
    * @returns a QVariant object generated from the JSON string
	 */
    QVariant parse ( QIODevice* io, bool* ok = 0 );

    /**
    * This is a method provided for convenience.
    * @param jsonData data containing the JSON object representation
    * @param ok if a conversion error occurs, *ok is set to false; otherwise *ok is set to true.
    * @returns a QVariant object generated from the JSON string
    * @sa errorString
    * @sa errorLine
    */
    QVariant parse ( const QByteArray& jsonData, bool* ok = 0 );
    
    /**
    * This method returns the error message tha ocuured during last parsing
    * @returns a QString object containing the error message of the last parse operation
    */
    QString errorString ( ) const;

    /**
    * This method returns line number where the last QJson parsing error occurred
    * @returns the line number where the error occurred
    */
    int errorLine ( ) const;
	
	/**
	 * Method called by plugins to get the OAuth Keys. The PM sends the 
	 * keys only if the pluginID is that of a currently loaded plugin
	 * @param aKeys [out] The OAuth keys
	 * @param aRegToken The plugin's registration token
	 * @param aPluginID The ID of the plugin that requests for 
	 * the OAuth keys
	 */
	void getAuthKeys( QMap<QString, QString> &aKeys, 
			const QString aRegToken, 
			const QString aPluginID );
	
	/**
	 * Method called by plugins to get the Nonce value
	 * @param aNonceString [out] The Nonce string
	 */
	void getNonce( QString &aNonceString );
	
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
	QByteArray createParameterString( const QString &aRequestUrl, 
			QNetworkAccessManager::Operation aOperation, 
			const QByteArray &aToken, 
			const QByteArray &aTokenSecret, 
			const SmfSignatureMethod aSignatureMethod, 
			const QMultiMap<QByteArray, QByteArray> &aParams, 
			const SmfParsingMode aMode);
	
private:
	/**
	 * Constructor with default argument
	 */
	SmfPluginUtil ( );

	
private:
	/**
	 * The single instance of SmfPluginUtil
	 */
	static SmfPluginUtil* m_myInstance;
	
	/**
	 * The instance of QJson::Parser
	 */
	QJson::Parser *m_jsonParser;
	
	};

#endif /* SMFPLUGINUTIL_H_ */
