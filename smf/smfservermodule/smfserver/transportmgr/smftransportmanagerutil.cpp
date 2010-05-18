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
 * The Transport Manager Utility class provides the http/https transaction 
 * methods for the smf framework
 *
 */

// Include files
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <zlib.h>
// For proxy settings on emulator only - REMOVE for device
#include <QNetworkProxy>

#include "smftransportmanagerutil.h"
#include "smfpluginmanager.h"

// Static Global constants
static const char kSmfUserAgent[] = "SmfFrameWork";
static const char kSmfCacheControl[] = "no-cache";
static const char kSmfAcceptEncoding[] = "gzip";

// Static data initialisation
SmfTransportManagerUtil* SmfTransportManagerUtil::m_myInstance = NULL;

/**
 * Method to get the instance of SmfTransportManagerUtil class
 * @return The instance of SmfTransportManager class
 */
SmfTransportManagerUtil* SmfTransportManagerUtil::getInstance ( )
	{
	if(NULL == m_myInstance)
		m_myInstance = new SmfTransportManagerUtil( );
	return m_myInstance;
	}

/**
 * Constructor with default argument
 * @param aParent The parent object
 */
SmfTransportManagerUtil::SmfTransportManagerUtil ( )
		: m_networkAccessManager ( this )
	{
	// Storage of settings (Only data usage as of now)
	m_settings = new QSettings("Sasken", "SmfTransportManager");

	// Create the settings if no data is stored
	if( !m_settings->contains("Sent Data") )
		m_settings->setValue("Sent Data", 0);
	if( !m_settings->contains("Received Data") )
		m_settings->setValue("Received Data", 0);
	}


/**
 * Destructor
 */
SmfTransportManagerUtil::~SmfTransportManagerUtil ( )
	{
	m_activeNetwReplyList.clear();
	
	if(m_settings)
		delete m_settings;
	
	if(m_myInstance)
		delete m_myInstance;
	}


/**
 * Method that does a http GET request. Returns the unique QNetworkReply
 * instance for this transaction to the plugin manager to trace the 
 * proper response.
 * @param aRequest The request formed by the plugin
 * @param aUrlList The list of accessible Urls for this plugin
 * @param aSOPCompliant [out] Output parameter indicating Same Origin 
 * Policy Compliance. Contains true if the request complies to the policy,
 * else false. If it is false, the network request will not performed
 * @return The unique QNetworkReply instance for this transaction
 */
QNetworkReply* SmfTransportManagerUtil::get ( 
		QNetworkRequest &aRequest,
		const QList<QUrl> &aUrlList,
		bool &aSOPCompliant )
	{
	bool found = false;
	QString hostName = aRequest.url().host();
	
	// Same Origin Policy checking
	// Check if the host name of the url in the request created by the 
	// plugin is among the authorised url host name list
	foreach(QUrl url, aUrlList)
		{
		if(0 == hostName.compare(url.host()))
			{
			found = true;
			break;
			}
		}
	
	if(found)
		{
		aSOPCompliant = true;
		
		// Set header to accept gzip encoded data
		aRequest.setRawHeader("Accept-encoding", kSmfAcceptEncoding);
		
		// Set the cache control
		aRequest.setRawHeader("Cache-Control", kSmfCacheControl);

		// Put the same user agent for all requests sent by Smf
		aRequest.setRawHeader("User-Agent", kSmfUserAgent);
		
		QNetworkReply* reply = m_networkAccessManager.get(aRequest);
	
		connect(&m_networkAccessManager, SIGNAL(finished(QNetworkReply *)), 
				this, SLOT(networkReplyFinished(QNetworkReply *)));
		
		connect(&m_networkAccessManager, SIGNAL(error(QNetworkReply::NetworkError)), 
				this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
		
		m_activeNetwReplyList.append(reply);
		
		return reply;
		}
	else
		{
		// Error :: SAME ORIGIN POLICY violation!!!
		aSOPCompliant = false;
		return NULL;
		}
	}


/**
 * Method that does a http POST request. Returns the unique QNetworkReply
 * instance for this transaction to the plugin manager to trace the 
 * proper response.
 * @param aRequest The request formed by the plugin
 * @param aPostData The data to be posted via http POST request
 * @param aUrlList The list of accessible Urls for this plugin
 * @param aSOPCompliant [out] Output parameter indicating Same Origin 
 * Policy Compliance. Contains true if the request complies to the policy,
 * else false. If it is false, the network request will not performed
 * @return The unique QNetworkReply instance for this transaction
 */
QNetworkReply* SmfTransportManagerUtil::post ( 
		QNetworkRequest &aRequest, 
		const QByteArray& aPostData,
		const QList<QUrl> &aUrlList,
		bool &aSOPCompliant )
	{
	bool found = false;
	QString hostName = aRequest.url().host();
	
	// Same Origin Policy checking
	// Check if the host name of the url in the request created by the 
	// plugin is among the authorised url host name list
	foreach(QUrl url, aUrlList)
		{
		if(0 == hostName.compare(url.host()))
			{
			found = true;
			break;
			}
		}
	
	if(found)
		{
		aSOPCompliant = true;
		
		// Set the cache control
		aRequest.setRawHeader("Cache-Control", kSmfCacheControl);
		
		// Put the same user agent for all requests sent by Smf
		aRequest.setRawHeader("User-Agent", kSmfUserAgent);
		
		QNetworkReply* reply = m_networkAccessManager.post(aRequest, aPostData);
	
		connect(&m_networkAccessManager, SIGNAL(finished(QNetworkReply *)), 
				this, SLOT(networkReplyFinished(QNetworkReply *)));
		
		connect(&m_networkAccessManager, SIGNAL(error(QNetworkReply::NetworkError)), 
				this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
		
		m_activeNetwReplyList.append(reply);
		
		return reply;
		}
	else
		{
		// Error :: SAME ORIGIN POLICY violation!!!
		aSOPCompliant = false;
		return NULL;
		}
	}


/**
 * Method that does a http HEAD request. Returns the unique QNetworkReply
 * instance for this transaction to the plugin manager to trace the 
 * proper response.
 * @param aRequest The request formed by the plugin
 * @param aUrlList The list of accessible Urls for this plugin
 * @param aSOPCompliant [out] Output parameter indicating Same Origin 
 * Policy Compliance. Contains true if the request complies to the policy,
 * else false. If it is false, the network request will not performed
 * @return The unique QNetworkReply instance for this transaction
 */
QNetworkReply* SmfTransportManagerUtil::head ( 
		QNetworkRequest& aRequest,
		const QList<QUrl> &aUrlList,
		bool &aSOPCompliant )
	{
	bool found = false;
	QString hostName = aRequest.url().host();

	// Same Origin Policy checking
	// Check if the host name of the url in the request created by the 
	// plugin is among the authorised url host name list
	foreach(QUrl url, aUrlList)
		{
		if(0 == hostName.compare(url.host()))
			{
			found = true;
			break;
			}
		}
	
	if(found)
		{
		aSOPCompliant = true;
		// Set the cache control
		aRequest.setRawHeader("Cache-Control", kSmfCacheControl);
		
		// Put the same user agent for all requests sent by Smf
		aRequest.setRawHeader("User-Agent", kSmfUserAgent);
		
		QNetworkReply* reply = m_networkAccessManager.head(aRequest);
	
		connect(&m_networkAccessManager, SIGNAL(finished(QNetworkReply *)), 
				this, SLOT(networkReplyFinished(QNetworkReply *)));
		
		connect(&m_networkAccessManager, SIGNAL(error(QNetworkReply::NetworkError)), 
				this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
		
		m_activeNetwReplyList.append(reply);
		
		return reply;
		}
	else
		{
		// Error :: SAME ORIGIN POLICY violation!!!
		aSOPCompliant = false;
		return NULL;
		}
	}


/**
 * Method that does a http PUT request. Returns the unique QNetworkReply
 * instance for this transaction to the plugin manager to trace the 
 * proper response.
 * @param aRequest The request formed by the plugin
 * @param aPostData The data to be posted via http PUT request
 * @param aUrlList The list of accessible Urls for this plugin
 * @param aSOPCompliant [out] Output parameter indicating Same Origin 
 * Policy Compliance. Contains true if the request complies to the policy,
 * else false. If it is false, the network request will not performed
 * @return The unique QNetworkReply instance for this transaction
 */
QNetworkReply* SmfTransportManagerUtil::put ( 
		QNetworkRequest &aRequest,
		const QByteArray& aPostData,
		const QList<QUrl> &aUrlList,
		bool &aSOPCompliant )
	{
	bool found = false;
	QString hostName = aRequest.url().host();

	// Same Origin Policy checking
	// Check if the host name of the url in the request created by the 
	// plugin is among the authorised url host name list
	foreach(QUrl url, aUrlList)
		{
		if(0 == hostName.compare(url.host()))
			{
			found = true;
			break;
			}
		}
	
	if(found)
		{
		aSOPCompliant = true;
		// Set the cache control
		aRequest.setRawHeader("Cache-Control", kSmfCacheControl);
		
		// Put the same user agent for all requests sent by Smf
		aRequest.setRawHeader("User-Agent", kSmfUserAgent);
		
		QNetworkReply* reply = m_networkAccessManager.put(aRequest, aPostData);
	
		connect(&m_networkAccessManager, SIGNAL(finished(QNetworkReply *)), 
				this, SLOT(networkReplyFinished(QNetworkReply *)));
		
		connect(&m_networkAccessManager, SIGNAL(error(QNetworkReply::NetworkError)), 
				this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
		
		m_activeNetwReplyList.append(reply);
		
		return reply;
		}
	else
		{
		// Error :: SAME ORIGIN POLICY violation!!!
		aSOPCompliant = false;
		return NULL;
		}
	}


/**
 * Method that does a http DELETE request. Returns the unique QNetworkReply
 * instance for this transaction to the plugin manager to trace the 
 * proper response.
 * @param aRequest The request formed by the plugin
 * @param aUrlList The list of accessible Urls for this plugin
 * @param aSOPCompliant [out] Output parameter indicating Same Origin 
 * Policy Compliance. Contains true if the request complies to the policy,
 * else false. If it is false, the network request will not performed
 * @return The unique QNetworkReply instance for this transaction
 */
QNetworkReply* SmfTransportManagerUtil::deleteResource ( 
		QNetworkRequest &aRequest,
		const QList<QUrl> &aUrlList,
		bool &aSOPCompliant )
	{
	bool found = false;
	QString hostName = aRequest.url().host();

	// Same Origin Policy checking
	// Check if the host name of the url in the request created by the 
	// plugin is among the authorised url host name list
	foreach(QUrl url, aUrlList)
		{
		if(0 == hostName.compare(url.host()))
			{
			found = true;
			break;
			}
		}
	
	if(found)
		{
		aSOPCompliant = true;
		// Set the cache control
		aRequest.setRawHeader("Cache-Control", kSmfCacheControl);
		
		// Put the same user agent for all requests sent by Smf
		aRequest.setRawHeader("User-Agent", kSmfUserAgent);
		
		QNetworkReply* reply = m_networkAccessManager.deleteResource(aRequest);
	
		connect(&m_networkAccessManager, SIGNAL(finished(QNetworkReply *)), 
				this, SLOT(networkReplyFinished(QNetworkReply *)));
		
		connect(&m_networkAccessManager, SIGNAL(error(QNetworkReply::NetworkError)), 
				this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
		
		m_activeNetwReplyList.append(reply);
		
		return reply;
		}
	else
		{
		// Error :: SAME ORIGIN POLICY violation!!!
		aSOPCompliant = false;
		return NULL;
		}
	}


/**
 * Method that cancels the service request by cancelling the current 
 * http transaction.
 * @param aCancelReply The QNetworkReply instance whose transaction 
 * has to be cancelled
 * @return Returns true for success, else false 
 */
bool SmfTransportManagerUtil::cancelRequest ( 
		QNetworkReply *aCancelReply )
	{
	// Checks if this transaction is running
	if( aCancelReply->isRunning() )
		{
		// Aborts the running transaction
		aCancelReply->abort();
		}
	// Close the connection
	aCancelReply->close();
	
	int index = m_activeNetwReplyList.indexOf(aCancelReply);
	if( index >= 0 )
		m_activeNetwReplyList.removeAt(index);
	
	return true;
	}



/*
 * Method that is called whenever a new network configuration is added to the system.
 * @param aResult SmfTransportResult
 */
void SmfTransportManagerUtil::configurationAdded ( const SmfTransportResult &aResult )
	{
	// Inform PM and plugins that a new IAP ia added, and hence resend the request
	SmfPluginManager::getInstance()->responseAvailable ( aResult, NULL, NULL );
	}

/*
 * Method that is called when the state of the network configuration changes.
 * @param aResult SmfTransportResult
 */
void SmfTransportManagerUtil::configurationChanged ( const SmfTransportResult &aResult )
	{
	// Inform PM and plugins that the IAP is changed, and hence resend the request
	SmfPluginManager::getInstance()->responseAvailable ( aResult, NULL, NULL );
	}

/*
 * Method that is called when a configuration is about to be removed from the system.
 * The removed configuration is invalid but retains name and identifier.
 * @param aResult SmfTransportResult
 */
void SmfTransportManagerUtil::configurationRemoved ( const SmfTransportResult &aResult )
	{
	// Inform PM and plugins that the IAP is to be deleted, and 
	// hence resend the request
	SmfPluginManager::getInstance()->responseAvailable ( aResult, NULL, NULL );

	}


/**
 * Method to indicate the http transaction has finished.
 * @param aNetworkReply The QNetworkReply instance for which the http 
 * transaction was made
 */
void SmfTransportManagerUtil::networkReplyFinished ( QNetworkReply *aNetworkReply )
	{
	// remove the QNetworkReply instance from the list of outstanding transactions
	int index = m_activeNetwReplyList.indexOf(aNetworkReply);
	if( index >= 0 )
		m_activeNetwReplyList.removeAt(index);
	
	// indicate the result of transaction to the plugin manager
	SmfTransportResult trResult;
	convertErrorType(aNetworkReply->error(), trResult);
	
	// Store the data received
	bool converted = false;
	quint64 data = m_settings->value("Received Data").toULongLong(&converted);
	if(converted)
		{
		data += aNetworkReply->readBufferSize();
		m_settings->setValue("Received Data", data);
		}

	int error = 0;
	QByteArray *arr = NULL;
	
	// Read the response from the device
	QByteArray response = aNetworkReply->readAll();
	
	// Check if the http response header has the raw header "Content-Encoding:gzip"
	// If so, inflate the gzip deflated data
	QByteArray headerKey("Content-Encoding");
	if(aNetworkReply->hasRawHeader(headerKey))
		{
		SmfPluginManager::getInstance()->server()->writeLog("Response is gzip encoded!!!");
		if( "gzip" == QString(aNetworkReply->rawHeader(headerKey)))
			arr = inflateResponse(response, error);
		else
			SmfPluginManager::getInstance()->server()->writeLog("Unsupported encoding format!!! - error");
		}
	else
		SmfPluginManager::getInstance()->server()->writeLog("Response is not gzip encoded");
	

	SmfPluginManager::getInstance()->responseAvailable ( trResult, aNetworkReply, arr );
	}


/**
 * Method to deflate a gzipped network response. Once this method is called, 
 * QNetworkReply internal buffer for holding network response is emptied.
 * @param aResponse The QByteArray instance holding the gzip encoded data
 * @param aError Argument indicating error
 * @return a QByteArray* containing the deflated data. If deflating fails, 
 * the encoded data itself without deflation is returned.
 */
QByteArray* SmfTransportManagerUtil::inflateResponse ( QByteArray &aResponse, int& aError )
	{
	Q_UNUSED(aError)
	// Read the response from the device
	SmfPluginManager::getInstance()->server()->writeLog("Encoded response content = "+QString(aResponse.toHex()));
	SmfPluginManager::getInstance()->server()->writeLog("Encoded response content size = "+QString::number(aResponse.size(), 10));
	
	// Get the uncompressed size of the response (last 4 bytes of the compressed data as per GZIP format spec)
	QByteArray sizeStr;
	for(int count = 1 ; count <= 4 ; count++)
		sizeStr.append(aResponse[aResponse.size()-count]);
	SmfPluginManager::getInstance()->server()->writeLog("Size string as a string = "+QString(sizeStr.toHex()));
	bool ok = false;
	int uncomSize = sizeStr.toHex().toInt(&ok, 16);
	SmfPluginManager::getInstance()->server()->writeLog("Size of uncompressed data = "+QString::number(uncomSize, 10));

	// Initialize the data required for zlib method call
	z_stream stream;
	unsigned char *out = new unsigned char[uncomSize];
	if(out)
		SmfPluginManager::getInstance()->server()->writeLog("Memory allocated for output buffer");
	else
		{
		//Error
		SmfPluginManager::getInstance()->server()->writeLog("Memory not allocated for output buffer!!!");
		return NULL;
		}
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.avail_in = 0;
	stream.next_in = Z_NULL;
	
	int ret = inflateInit2(&stream, 16+MAX_WBITS);
	SmfPluginManager::getInstance()->server()->writeLog("return value of inflateInit2() = "+QString::number(ret, 10));
	if(Z_OK != ret)
		{
		SmfPluginManager::getInstance()->server()->writeLog("Error in inflateInit2, returning...");
		delete[] out;
		return NULL;
		}

	// Initialize the data required for inflate() method call
	stream.avail_in = aResponse.size();
	stream.next_in = (unsigned char *)aResponse.data();
	stream.avail_out = uncomSize;
	stream.next_out = out;
	
	ret = inflate(&stream, Z_NO_FLUSH);
	SmfPluginManager::getInstance()->server()->writeLog("return value of inflate() = "+QString::number(ret, 10));
	
	switch (ret) 
		{
		// fall through
		case Z_NEED_DICT:
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			{
			(void)inflateEnd(&stream);
			delete[] out;
			return NULL;
			}
		}

	SmfPluginManager::getInstance()->server()->writeLog("total bytes read so far = "+QString::number(stream.total_in, 10));
	SmfPluginManager::getInstance()->server()->writeLog("total bytes output so far = "+QString::number(stream.total_out, 10));

	// Write the inflated data to a QByteArray
	QByteArray *uncompressedData = new QByteArray((char *)out);
	delete[] out;
	
	// If there is some unwanted data at the end of uncompressed data, chop them
	int chopLength = uncompressedData->size() - uncomSize;
	SmfPluginManager::getInstance()->server()->writeLog("uncompressedData before chopping = "+QString(uncompressedData->toHex()));
	SmfPluginManager::getInstance()->server()->writeLog("old size of uncompressed data = "+QString::number(uncompressedData->size(), 10));
	uncompressedData->chop(chopLength);
	SmfPluginManager::getInstance()->server()->writeLog("new size of uncompressed data = "+QString::number(uncompressedData->size(), 10));
	SmfPluginManager::getInstance()->server()->writeLog("uncompressedData after chopping = "+QString(uncompressedData->toHex()));

	// Write the uncompressed data to a file
	QFile file1("c:\\data\\uncompressedflickrresponse.txt");
	if (!file1.open(QIODevice::WriteOnly))
		SmfPluginManager::getInstance()->server()->writeLog("File to write the uncompressed response could not be opened");
	file1.write(uncompressedData->data());
	file1.close();
	SmfPluginManager::getInstance()->server()->writeLog("uncompressed data written to c:\\data\\uncompressedflickrresponse.txt");
	
	return uncompressedData;
	}

/**
 * Method called when the QNetworkReply detects an error in processing.
 * @param aError The QNetworkReply error code 
 */
void SmfTransportManagerUtil::networkReplyError ( QNetworkReply::NetworkError aError )
	{
	// indicate the error to the plugin manager
	SmfTransportResult trResult;
	convertErrorType(aError, trResult);
	
	SmfPluginManager::getInstance()->responseAvailable ( trResult, NULL, NULL );
	}


/**
 * Method to convert QNetworkReply Error to the type SmfTransportResult 
 * QNetworkRequest received before executing the web query.
 * @param aError The QNetworkReply Error
 * @param aResult [out] The SmfTransportResult error
 */
void SmfTransportManagerUtil::convertErrorType( const QNetworkReply::NetworkError &aError, 
		SmfTransportResult &aResult )
	{
	switch(aError)
		{
		case QNetworkReply::NoError:
			aResult = SmfTransportOpNoError;
			break;
			
		case QNetworkReply::ConnectionRefusedError:
			aResult = SmfTransportOpConnectionRefusedError;
			break;
			
		case QNetworkReply::RemoteHostClosedError:
			aResult = SmfTransportOpRemoteHostClosedError;
			break;
			
		case QNetworkReply::HostNotFoundError:
			aResult = SmfTransportOpHostNotFoundError;
			break;
			
		case QNetworkReply::TimeoutError:
			aResult = SmfTransportOpTimeoutError;
			break;
			
		case QNetworkReply::OperationCanceledError:
			aResult = SmfTransportOpOperationCanceledError;
			break;
			
		case QNetworkReply::SslHandshakeFailedError:
			aResult = SmfTransportOpSslHandshakeFailedError;
			break;
			
		case QNetworkReply::ProxyConnectionRefusedError:
			aResult = SmfTransportOpProxyConnectionRefusedError;
			break;
			
		case QNetworkReply::ProxyConnectionClosedError:
			aResult = SmfTransportOpProxyConnectionClosedError;
			break;
			
		case QNetworkReply::ProxyNotFoundError:
			aResult = SmfTransportOpProxyNotFoundError;
			break;
			
		case QNetworkReply::ProxyTimeoutError:
			aResult = SmfTransportOpProxyTimeoutError;
			break;
			
		case QNetworkReply::ProxyAuthenticationRequiredError:
			aResult = SmfTransportOpProxyAuthenticationRequiredError;
			break;
			
		case QNetworkReply::ContentAccessDenied:
			aResult = SmfTransportOpContentAccessDenied;
			break;
			
		case QNetworkReply::ContentOperationNotPermittedError:
			aResult = SmfTransportOpContentOperationNotPermittedError;
			break;
			
		case QNetworkReply::ContentNotFoundError:
			aResult = SmfTransportOpContentNotFoundError;
			break;
			
		case QNetworkReply::AuthenticationRequiredError:
			aResult = SmfTransportOpAuthenticationRequiredError;
			break;
			
		case QNetworkReply::ContentReSendError:
			aResult = SmfTransportOpContentReSendError;
			break;
			
		case QNetworkReply::ProtocolUnknownError:
			aResult = SmfTransportOpProtocolUnknownError;
			break;

		case QNetworkReply::ProtocolInvalidOperationError:
			aResult = SmfTransportOpProtocolInvalidOperationError;
			break;
			
		case QNetworkReply::UnknownNetworkError:
			aResult = SmfTransportOpUnknownNetworkError;
			break;
			
		case QNetworkReply::UnknownProxyError:
			aResult = SmfTransportOpUnknownProxyError;
			break;
			
		case QNetworkReply::UnknownContentError:
			aResult = SmfTransportOpUnknownContentError;
			break;
			
		case QNetworkReply::ProtocolFailure:
			aResult = SmfTransportOpProtocolFailure;
			break;
			
		default:
			;
		}
	}

