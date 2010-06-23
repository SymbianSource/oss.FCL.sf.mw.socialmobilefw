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
 * Nalina Hariharan
 * 
 * Description:
 * The Plugin that fetches contacts from the logged in user's flickr account
 *
 */

// Include files
#include <QtPlugin>
#include <QCryptographicHash>
#include <QTextStream>
#include <QFile>
#include <QMap>
#include <QListIterator>
#include <QDebug>
#ifdef SMF_XMLPARSING
#include <QXmlStreamReader>
#endif

#include "flickrcontactfetcherplugin.h"

/**
 * Destructor
 */
FlickrContactFetcherPlugin::~FlickrContactFetcherPlugin( )
	{
	if(m_provider)
		delete m_provider;
	}

/**
 * Method to get the list of friends
 * @param aRequest [out] The request data to be sent to network
 * @param aPageNum The page to be extracted
 * @param aItemsPerPage Number of items per page
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FlickrContactFetcherPlugin::friends( SmfPluginRequestData &aRequest,
		const int aPageNum, 
		const int aItemsPerPage )
	{
	qDebug()<<"Inside FlickrContactFetcherPlugin::friends()";
	
	SmfPluginError error = SmfPluginErrInvalidArguments;

	// invalid arguments
	if( aPageNum < 0 || aItemsPerPage < 0 )
		{
		qDebug()<<"Invalid arguments";
		return error;
		}
	
	qDebug()<<"Valid arguments";
	
#if 1
// Reading the keys, CSM Stubbed - START
	QFile file("c:\\data\\FlickrKeys.txt");
	if (!file.open(QIODevice::ReadOnly))
		{
		qDebug()<<"File to read the keys could not be opened";
		return SmfPluginErrUserNotLoggedIn;
		}
	
	qDebug()<<"Key file read, going to parse the key values from file";
	
	QByteArray arr = file.readAll();
	QList<QByteArray> list = arr.split('\n');
	file.close();
	
	QString apiKey(list[0]);
	QString apiSecret(list[1]);
	QString authToken(list[2]);
	
	qDebug()<<"Api Key = "<<apiKey;
	qDebug()<<"Api Secret = "<<apiSecret;
	qDebug()<<"Auth Token = "<<authToken;
// Reading the keys, CSM Stubbed - END
#endif
	
	// Create the API signature string
	QString baseString;
	baseString.append(apiSecret);
	baseString.append("api_key"+apiKey);
	baseString.append("auth_token"+authToken);
	baseString.append("filterfriends");
#ifdef SMF_XMLPARSING
	baseString.append("formatxml");
#else
	baseString.append("formatjson");
#endif
	baseString.append("methodflickr.contacts.getList");
	baseString.append("page"+QString::number(aPageNum));
	baseString.append("per_page"+QString::number(aItemsPerPage));
	
	// Create the url
	QUrl url("http://api.flickr.com/services/rest/?");
	url.addQueryItem("api_key", apiKey);
	url.addQueryItem("auth_token", authToken);
	url.addQueryItem("filter", "friends");
#ifdef SMF_XMLPARSING
	url.addQueryItem("format", "x");
#else
	url.addQueryItem("format", "json");
#endif
	url.addQueryItem("method", "flickr.contacts.getList");
	url.addQueryItem("page", QString::number(aPageNum));
	url.addQueryItem("per_page", QString::number(aItemsPerPage));
	url.addQueryItem("api_sig", generateSignature(baseString));
	
	// Create the request, set the url
	aRequest.iNetworkRequest.setUrl(url);
	aRequest.iRequestType = SmfContactGetFriends;
	aRequest.iPostData = NULL;
	aRequest.iHttpOperationType = QNetworkAccessManager::GetOperation;
	error = SmfPluginErrNone;

	qDebug()<<"Url string is : "<<aRequest.iNetworkRequest.url().toString();
	return error; 
	}

/**
 * Method called by plugins to generate a signature string from a base string
 * @param aBaseString The base string
 * @return The md5 hash of the base string
 */
QString FlickrContactFetcherPlugin::generateSignature(const QString aBaseString)
	{
	qDebug()<<"Inside FlickrContactFetcherPlugin::generateSignature()";
	
	// Create md5 hash of the signature string
    QByteArray byteArray;
    byteArray.insert(0, aBaseString.toAscii());

    QByteArray md5Hash = QCryptographicHash::hash(byteArray,QCryptographicHash::Md5 ).toHex();
    QString returnString (md5Hash);
    qDebug()<<"generated signature = "<<QString(returnString);
    return returnString;
	}

/**
 * Method to get the list of followers
 * @param aRequest [out] The request data to be sent to network
 * @param aPageNum The page to be extracted
 * @param aItemsPerPage Number of items per page
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FlickrContactFetcherPlugin::followers( SmfPluginRequestData &aRequest,
		const int aPageNum , 
		const int aItemsPerPage  )
	{
	Q_UNUSED(aRequest)
	Q_UNUSED(aPageNum)
	Q_UNUSED(aItemsPerPage)
	qDebug()<<"Inside FlickrContactFetcherPlugin::followers()";
	return SmfPluginErrServiceNotSupported; 
	}

/**
 * Method to search for a contact
 * @param aRequest [out] The request data to be sent to network
 * @param aContact contact to be searched
 * @param aPageNum The page to be extracted
 * @param aItemsPerPage Number of items per page
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FlickrContactFetcherPlugin::search( SmfPluginRequestData &aRequest,
		const SmfContact &aContact,
		const int aPageNum , 
		const int aItemsPerPage  )
	{
	Q_UNUSED(aRequest)
	Q_UNUSED(aContact)
	Q_UNUSED(aPageNum)
	Q_UNUSED(aItemsPerPage)
	qDebug()<<"Inside FlickrContactFetcherPlugin::search()";
	return SmfPluginErrServiceNotSupported; 
	}

/**
 * Method to search for contacts (friends) who are near the user.
 * Proximity defines accuracy level
 * @param aRequest [out] The request data to be sent to network
 * @param aLocation The location search criteria
 * @param aProximity location search boundary
 * @param aPageNum The page to be extracted
 * @param aItemsPerPage Number of items per page
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FlickrContactFetcherPlugin::searchNear( SmfPluginRequestData &aRequest,
		const SmfLocation &aLocation,
		const SmfLocationSearchBoundary &aProximity,
		const int aPageNum , 
		const int aItemsPerPage  )
	{
	Q_UNUSED(aRequest)
	Q_UNUSED(aLocation)
	Q_UNUSED(aProximity)
	Q_UNUSED(aPageNum)
	Q_UNUSED(aItemsPerPage)
	qDebug()<<"Inside FlickrContactFetcherPlugin::searchNear()";
	return SmfPluginErrServiceNotSupported; 
	}

/**
 * Method to get the list of groups
 * @param aRequest [out] The request data to be sent to network
 * @param aPageNum The page to be extracted
 * @param aItemsPerPage Number of items per page
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FlickrContactFetcherPlugin::groups( SmfPluginRequestData &aRequest,
		const int aPageNum , 
		const int aItemsPerPage  )
	{
	Q_UNUSED(aRequest)
	Q_UNUSED(aPageNum)
	Q_UNUSED(aItemsPerPage)
	qDebug()<<"Inside FlickrContactFetcherPlugin::groups()";
	return SmfPluginErrServiceNotSupported; 
	}

/**
 * Method to search for a contact in a group
 * @param aRequest [out] The request data to be sent to network
 * @param aGroup the group in which to search
 * @param aPageNum The page to be extracted
 * @param aItemsPerPage Number of items per page
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FlickrContactFetcherPlugin::searchInGroup( SmfPluginRequestData &aRequest,
		const SmfGroup &aGroup,
		const int aPageNum , 
		const int aItemsPerPage  )
	{
	Q_UNUSED(aRequest)
	Q_UNUSED(aGroup)
	Q_UNUSED(aPageNum)
	Q_UNUSED(aItemsPerPage)
	qDebug()<<"Inside FlickrContactFetcherPlugin::searchInGroup()";
	return SmfPluginErrServiceNotSupported; 
	}

/**
 * Customised method for SmfContactFetcherPlugin interface
 * @param aRequest [out] The request data to be sent to network
 * @param aOperation The operation type (should be known between 
 * the client interface and the plugin)
 * @param aData The data required to form the request (The type 
 * of data should be known between client and the plugin)
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FlickrContactFetcherPlugin::customRequest( SmfPluginRequestData &aRequest, 
		const int &aOperation, QByteArray *aData )
	{
	Q_UNUSED(aRequest)
	Q_UNUSED(aOperation)
	Q_UNUSED(aData)
	qDebug()<<"Inside FlickrContactFetcherPlugin::customRequest()";
	return SmfPluginErrServiceNotSupported; 
	}

/**
 * The first method to be called in the plugin that implements this interface.
 * If this method is not called, plugin may not behave as expected.
 * Plugins are expected to save the aUtil handle and use and when required.
 * @param aUtil The instance of SmfPluginUtil
 */
void FlickrContactFetcherPlugin::initialize( SmfPluginUtil *aUtil )
	{
	// Save the SmfPluginUtil handle
	m_util = aUtil;
	
	// Create an instance of FlickrProviderBase
	m_provider = new FlickrProviderBase;
	m_provider->initialize();
	}

/**
 * Method to get the provider information
 * @return Instance of SmfProviderBase
 */
SmfProviderBase* FlickrContactFetcherPlugin::getProviderInfo( )
	{
	return m_provider;
	}

/**
 * Method to get the result for a network request.
 * @param aOperation The type of operation to be requested
 * @param aTransportResult The result of transport operation
 * @param aResponse The QByteArray instance containing the network response.
 * The plugins should delete this instance once they have read the 
 * data from it.
 * @param aResult [out] An output parameter to the plugin manager.If the 
 * return value is SmfSendRequestAgain, QVariant will be of type 
 * SmfPluginRequestData.
 * For SmfGalleryPlugin: If last operation was pictures(), aResult will 
 * be of type QList<SmfPicture>. If last operation was description(), 
 * aResult will be of type QString. If last operation was upload() or 
 * postComment(), aResult will be of type bool.
 * @param aRetType [out] SmfPluginRetType
 * @param aPageResult [out] The SmfResultPage structure variable
 */
SmfPluginError FlickrContactFetcherPlugin::responseAvailable( 
		const SmfRequestTypeID aOperation,
		const SmfTransportResult &aTransportResult, 
		QByteArray *aResponse, 
		QVariant* aResult, 
		SmfPluginRetType &aRetType,
		SmfResultPage &aPageResult )
	{
	Q_UNUSED(aPageResult)
	qDebug()<<"Inside FlickrContactFetcherPlugin::responseAvailable()";
	
	SmfPluginError error = SmfPluginErrNetworkError;
	
	if( !aResponse || (0 == aResponse->size()) )
		{
		qDebug()<<"Response is NULL or empty";
		aRetType = SmfRequestError;
		return error;
		}
	
	QByteArray response(*aResponse);
	delete aResponse;
	qDebug()<<"FB response = "<<QString(response);
	qDebug()<<"FB response size = "<<response.size();
	
	if(SmfTransportOpNoError == aTransportResult)
		{
		qDebug()<<"No transport error";
		
		if(SmfContactGetFriends == aOperation)
			{
			qDebug()<<"For getting friends response";
			
			QList<SmfContact> list;
			
#ifdef SMF_XMLPARSING // Xml parsing
			// For getting contacts from xml response
			QXmlStreamReader xml(response);
			while (!xml.atEnd())
				{
				xml.readNext();
				if (xml.tokenType() == QXmlStreamReader::StartElement)
					{
					// If the tag is contact
					if (xml.name() == "contact")
						{
						qDebug()<<"Contact tag found";
						SmfContact contact;
						QStringRef str;
						QContactName contactname;
						QString username = xml.attributes().value("username").toString();
						qDebug()<<"Username = "<<username;
						contactname.setFirstName(username);
						contactname.setLastName(username);
						QVariant namevar1 = QVariant::fromValue(contactname);
						contact.setValue("Name",namevar1);
						list.append(contact);
						}
					}
				}
#else
			// To remove the "jsonFlickrApi(" and also remove the last ")" from the response,
			// as these gives a Json parsing error
			response.remove(0, 14);
			response.chop(1);
			
			// For getting contacts from json response
			bool ok;
			QVariantMap result = m_util->parse(response, &ok).toMap();
			if (!ok) {
				qDebug()<<"An error occurred during json parsing";
				aResult->setValue(list);
				aRetType = SmfRequestError;
				return SmfPluginErrParsingFailed;
			}
			
			QVariantMap map1 = result["contacts"].toMap();
			qDebug()<<"page = "<<map1["page"].toString();
			qDebug()<<"pages = "<<map1["pages"].toString();
			qDebug()<<"per_page = "<<map1["per_page"].toString();
			qDebug()<<"perpage = "<<map1["perpage"].toString();
			qDebug()<<"total = "<<map1["perpage"].toString();
			
			QList<QVariant> list1 = map1["contact"].toList();
			
			QListIterator<QVariant> i(list1);
			while(i.hasNext())
				{
				SmfContact contact;
				QVariantMap map2 = i.next().toMap();
				qDebug()<<"nsid = "<<map2["nsid"].toString();
				qDebug()<<"username = "<<map2["username"].toString();
				qDebug()<<"iconserver = "<<map2["iconserver"].toString();
				qDebug()<<"iconfarm = "<<map2["iconfarm"].toString();
				qDebug()<<"ignored = "<<map2["ignored"].toString();
				qDebug()<<"realname = "<<map2["realname"].toString();
				qDebug()<<"friend = "<<map2["friend"].toString();
				qDebug()<<"family = "<<map2["family"].toString();
				qDebug()<<"path_alias = "<<map2["path_alias"].toString();
				qDebug()<<"location = "<<map2["location"].toString();
				
				QContactName contactname;
				QString username = map2["username"].toString();
				qDebug()<<"Username = "<<username;
				contactname.setFirstName(username);
				contactname.setLastName(username);
				QVariant nameVar = QVariant::fromValue(contactname);
				contact.setValue("Name",nameVar);
				list.append(contact);
				}
#endif
			
			qDebug()<<"list count = "<<list.count();
			aResult->setValue(list);
			aRetType = SmfRequestComplete;
			error = SmfPluginErrNone;
			}
		else
			{
			qDebug()<<"Service unsupported, currently only SmfContactGetFriends !!!";
			aRetType = SmfRequestError;
			error = SmfPluginErrServiceNotSupported;
			}
		}

	else if(SmfTransportOpOperationCanceledError == aTransportResult)
		{
		qDebug()<<"Operation Cancelled !!!";
		error = SmfPluginErrCancelComplete;
		aRetType = SmfRequestComplete;
		}

	else
		{
		qDebug()<<"Transport Error !!!";
		error = SmfPluginErrNetworkError;
		aRetType = SmfRequestError;
		}
	
	return error;
	}


/**
 * Destructor
 */
FlickrProviderBase::~FlickrProviderBase( )
	{
	}

/**
 * Method to get the Localisable name of the service.
 * @return The Localisable name of the service.
 */
QString FlickrProviderBase::serviceName( ) const
	{
	return m_serviceName;
	}

/**
 * Method to get the Logo of the service
 * @return The Logo of the service
 */
QImage FlickrProviderBase::serviceIcon( ) const
	{
	return m_serviceIcon;
	}

/**
 * Method to get the Readable service description
 * @return The Readable service description
 */
QString FlickrProviderBase::description( ) const
	{
	return m_description;
	}

/**
 * Method to get the Website of the service
 * @return The Website of the service
 */
QUrl FlickrProviderBase::serviceUrl( ) const
	{
	return m_serviceUrl;
	}

/**
 * Method to get the URL of the Application providing this service
 * @return The URL of the Application providing this service
 */
QUrl FlickrProviderBase::applicationUrl( ) const
	{
	return m_applicationUrl;
	}

/**
 * Method to get the Icon of the application
 * @return The Icon of the application
 */
QImage FlickrProviderBase::applicationIcon( ) const
	{
	return m_applicationIcon;
	}

/**
* Method to get the list of interfaces that this provider support
* @return List of supported Interafces
*/
QList<QString> FlickrProviderBase::supportedInterfaces( ) const
	{
	return m_supportedInterfaces;
	}

/**
* Method to get the list of languages supported by this service provider
* @return a QStringList of languages supported by this service 
* provider in 2 letter ISO 639-1 format.
*/
QStringList FlickrProviderBase::supportedLanguages( ) const
	{
	return m_supportedLangs;
	}

/**
 * Method to get the Plugin specific ID
 * @return The Plugin specific ID
 */
QString FlickrProviderBase::pluginId( ) const
	{
	return m_pluginId;
	}

/**
 * Method to get the ID of the authentication application 
 * for this service
 * @param aProgram The authentication application name
 * @param aArguments List of arguments required for authentication app
 * @param aMode Strting mode for authentication application
 * @return The ID of the authentication application 
 */
QString FlickrProviderBase::authenticationApp( QString &aProgram, 
		QStringList & aArguments, 
		QIODevice::OpenModeFlag aMode ) const
	{
	Q_UNUSED(aProgram)
	Q_UNUSED(aArguments)
	Q_UNUSED(aMode)
	return m_authAppId;
	}

/**
 * Method to get the unique registration ID provided by the 
 * Smf for authorised plugins
 * @return The unique registration ID/token provided by the Smf for 
 * authorised plugins
 */
QString FlickrProviderBase::smfRegistrationId( ) const
	{
	return m_smfRegToken;
	}

void FlickrProviderBase::initialize()
	{
	m_serviceName = "Flickr";
	m_description = "Flickr contact fetcher plugin description";
	m_serviceUrl = QUrl(QString("http://api.flickr.com"));
	m_pluginId = "flickrcontactfetcherplugin.qtplugin";
	m_authAppId = "Flickr AuthAppId";
	m_smfRegToken = "Flickr RegToken";
	m_supportedInterfaces.append("org.symbian.smf.plugin.contact.fetcher/v0.2");
	}


/*
 * Export Macro
 * plugin name : flickrcontactfetcherplugin
 * plugin class : FlickrContactFetcherPlugin
 */
Q_EXPORT_PLUGIN2( flickrcontactfetcherplugin, FlickrContactFetcherPlugin )

