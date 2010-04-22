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

#include <QObject.h>
#include <QNetworkAccessManager>

/**
 * see http://qjson.sourceforge.net/usage.html for more details
 */
using namespace QJson;
class Parser;

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
class SmfPluginUtil : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Method to get the instance of SmfPluginUtil class
	 * @param aParser The JSON parser utility object instance
	 * @return The instance of SmfPluginUtil class
	 */
	static SmfPluginUtil* getInstance ( Parser *aParser );

	/**
	 * Destructor
	 */
	~SmfPluginUtil ( );

	/**
	 * Method called by plugins to get the handle to QJson library
	 * @param aHandle [out] The QJson handle
	 */
	Parser* getJsonHandle( void );

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
	 * @param aStr [out] The Nonce value
	 */
	void getNonce( QString &aStr );

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
	 * @param aParser The QJOSN parser object
	 */
	SmfPluginUtil ( Parser *aParser );


private:
	/**
	 * The single instance of SmfPluginUtil
	 */
	static SmfPluginUtil* m_myInstance;

	Parser *m_jsonParser;

	};

#endif /* SMFPLUGINUTIL_H_ */
