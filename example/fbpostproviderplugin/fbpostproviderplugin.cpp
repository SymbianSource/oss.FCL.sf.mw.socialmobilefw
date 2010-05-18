
// Include files
#include <QtPlugin>
#include <QCryptographicHash>
#include <QTextFormat>
#include <QTextStream>
#include <QFile>
#include <QNetworkReply>
#include <QXmlStreamReader>

#include "fbpostproviderplugin.h"

// Added for flickr testing - start - put your registered app's  keys here
static const QString apiKey = "";
static const QString apiSecret = "";
static const QString sessionKey = "";
static const QString sessionSecret = "";


/**
 * Method called by plugins to generate a signature string from a base string
 * @param aBaseString The base string
 * @return The md5 hash of the base string
 */
QString FBPostProviderPlugin::generateSignature(const QString aBaseString)
	{
	writeLog("FBPostProviderPlugin::generateSignature");

	// Create md5 hash of the signature string
    QByteArray byteArray;
    byteArray.insert(0, aBaseString.toAscii());

    QByteArray md5Hash = QCryptographicHash::hash(byteArray,QCryptographicHash::Md5 ).toHex();
    QString returnString (md5Hash);
    return returnString;
	}


/**
 * Method called by plugins for logging
 * @param log string to be logged
 */
void FBPostProviderPlugin::writeLog(QString log) const
	{
	QFile file("c:\\data\\PluginLogs.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
	         ;
    QTextStream out(&file);
    out << log << "\n";
    file.close();
	}

/**
 * Destructor
 */
FBPostProviderPlugin::~FBPostProviderPlugin( )
	{
	if(m_provider)
		delete m_provider;
	}

/**
 * Method that returns maximum no of chars (unicode) that service
 * provider can post without truncation. Negative value means
 * no limit
 * @return Max characters that can be posted without truncation
 */
qint32 FBPostProviderPlugin::maxCharsInPost( ) const
		{
	qint32 maxCharsInPost = 256;
	return maxCharsInPost;
		}
/**
 * Method that returns maximum no of items that can be returned
 * in a single query to getPosts. Negative value means feature
 * not supported.
 * @return Max items that can be returned in a single query
 */
qint32 FBPostProviderPlugin::maxItems( ) const
		{
	qint32 maxItems = 10;
	return maxItems;
		}

/**
 * <Method that returns all the formatting of posts that this
 * service provider supports. May return 0 items to mean
 * only QString is supported.
 * @return Supported formats of posts
 */
QVector<QTextFormat> FBPostProviderPlugin::supportedFormats ( ) const
		{
	QVector<QTextFormat> data;
	return data;
		}

/**
 * Method that returns whether this SP supports Appearence
 * @return Returns true if Appearance is supported, else false.
 * @see SmfAppearenceInfo
 */
bool FBPostProviderPlugin::supportsAppearence ( ) const
		{
	return false;
		}

/**
 * Method to get the latest posts
 * @param aRequest [out] The request data to be sent to network
 * @param aUser The user's contact in this SP, omit for self contact
 * @param aPageNum The page to be extracted
 * @param aItemsPerPage Number of items per page
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FBPostProviderPlugin::retrieve( SmfPluginRequestData &aRequest,
		const SmfContact *aUser,
		const int aPageNum ,
		const int aItemsPerPage  )
	{
	Q_UNUSED(aUser)
	writeLog("Inside FBPostProviderPlugin::retrieve");

	SmfPluginError error = SmfPluginErrInvalidRequest;

	//// Currently considering for self contatc , ie, omitting aUser
	// invalid arguments
	if( aPageNum < 0 || aItemsPerPage < 0 )
		return error;
	else
		{
		// Get the current date and time and convert it to sec as a string
		QString call_id = QString::number(QDateTime::currentDateTime().toTime_t(), 10);

		// Create the API signature string
		QString baseString;
		baseString.append("api_key="+apiKey);
		baseString.append("call_id="+call_id);
		baseString.append("format=XML");
		baseString.append("method=stream.get");
		baseString.append("session_key="+sessionKey);
		baseString.append("ss=1");
		baseString.append("v=1.0");
		baseString.append(sessionSecret);

		// Create the url
		QUrl url("http://api.facebook.com/restserver.php?");
		url.addQueryItem("api_key", apiKey);
		url.addQueryItem("call_id", call_id);
		url.addQueryItem("format", "XML");
		url.addQueryItem("method", "stream.get");
		url.addQueryItem("session_key", sessionKey);
		url.addQueryItem("ss", "1");
		url.addQueryItem("v", "1.0");
		url.addQueryItem("sig", generateSignature(baseString));

		// Create the request, set the url
		aRequest.iNetworkRequest.setUrl(url);
		aRequest.iRequestType = SmfContactRetrievePosts;
		aRequest.iPostData = NULL;
		aRequest.iHttpOperationType = QNetworkAccessManager::GetOperation;
		error = SmfPluginErrNone;
		}
	writeLog("Url string is : "+aRequest.iNetworkRequest.url().toString());
	return error;
	}

/**
 * Method to update a post to own area.
 * @param aRequest [out] The request data to be sent to network
 * @param aPostData The post data to be posted
 * @param aLocation The location
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FBPostProviderPlugin::post( SmfPluginRequestData &aRequest,
		const SmfPost &aPostData,
		const SmfLocation &aLocation )
	{
	Q_UNUSED(aRequest)
Q_UNUSED(aPostData)
Q_UNUSED(aLocation)
	SmfPluginError error = SmfPluginErrInvalidRequest;
	return error;
	}

/**
 * Method to update the last post to own area with new data
 * @param aRequest [out] The request data to be sent to network
 * @param aPostData The edited/new data to be posted
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FBPostProviderPlugin::updatePost( SmfPluginRequestData &aRequest,
		const SmfPost &aPostData )
	{
	Q_UNUSED(aRequest)
Q_UNUSED(aPostData)
	SmfPluginError error = SmfPluginErrInvalidRequest;
	return error;
	}

/**
 * Method to update a post to a particular contact
 * @param aRequest [out] The request data to be sent to network
 * @param aPostData The post data to be posted
 * @param aContact The contact where the data has to be posted
 * @param aLocation The location
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FBPostProviderPlugin::postDirected( SmfPluginRequestData &aRequest,
		const SmfPost &aPostData,
		const SmfContact &aContact,
		const SmfLocation *aLocation  )
	{
	Q_UNUSED(aRequest)
Q_UNUSED(aPostData)
Q_UNUSED(aContact)
Q_UNUSED(aLocation)
	SmfPluginError error = SmfPluginErrInvalidRequest;
	return error;
	}


/**
 * Method to post a comment on a post.
 * @param aRequest [out] The request data to be sent to network
 * @param aTarget Post on which comment has to be posted
 * @param aComment comment to be posted
 * @param aLocation location data
 */
SmfPluginError FBPostProviderPlugin::commentOnAPost(SmfPluginRequestData &aRequest,
		const SmfPost &aTarget,
		const SmfPost &aComment,
		const SmfLocation *aLocation )
	{
	Q_UNUSED(aRequest)
Q_UNUSED(aTarget)
Q_UNUSED(aComment)
Q_UNUSED(aLocation)
	SmfPluginError error = SmfPluginErrInvalidRequest;
	return error;
	}

/**
 * Method to update the presence information of the user
 * @param aRequest [out] The request data to be sent to network
 * @param aAppearence The appearence information
 * @param aStatus The status string
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FBPostProviderPlugin::postAppearence( SmfPluginRequestData &aRequest,
		const SmfPresenceInfo &aAppearence,
		const QString &aStatus )
	{
	Q_UNUSED(aRequest)
Q_UNUSED(aAppearence)
Q_UNUSED(aStatus)
	SmfPluginError error = SmfPluginErrInvalidRequest;
	return error;
	}

/**
 * Share a contact's post to user's friends and followers
 * (e.g. retweet in twitter, share on facebook)
 * @param aRequest [out] The request data to be sent to network
 * @param aPostData data to be posted
 * @param aContact contact to which the post belonged
 * @param aEdited whether user changed items within the post
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FBPostProviderPlugin::sharePost( SmfPluginRequestData &aRequest,
		const SmfPost &aPostData,
		const SmfContact &aContact,
		const bool &aEdited)
	{
	Q_UNUSED(aRequest)
Q_UNUSED(aPostData)
Q_UNUSED(aContact)
Q_UNUSED(aEdited)
	SmfPluginError error = SmfPluginErrInvalidRequest;
	return error;
	}

/**
 * Customised method for SmfPostProviderPlugin interface
 * @param aRequest [out] The request data to be sent to network
 * @param aOperation The operation type (should be known between
 * the client interface and the plugin)
 * @param aData The data required to form the request (The type
 * of data should be known between client and the plugin)
 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
 */
SmfPluginError FBPostProviderPlugin::customRequest( SmfPluginRequestData &aRequest,
		const int &aOperation, QByteArray *aData )
	{
	Q_UNUSED(aRequest)
Q_UNUSED(aOperation)
Q_UNUSED(aData)
	SmfPluginError error = SmfPluginErrInvalidRequest;
	return error;
	}


/**
 * The first method to be called in the plugin that implements this interface.
 * If this method is not called, plugin may not behave as expected.
 * Plugins are expected to save the aUtil handle and use and when required.
 * @param aUtil The instance of SmfPluginUtil
 */
void FBPostProviderPlugin::initialize( SmfPluginUtil *aUtil )
	{
	// Save the SmfPluginUtil handle
	m_util = aUtil;

	// Create an instance of FlickrProviderBase
	m_provider = new FBProviderBase;
	m_provider->initialize();
	}

/**
 * Method to get the provider information
 * @return Instance of SmfProviderBase
 */
SmfProviderBase* FBPostProviderPlugin::getProviderInfo( )
	{
	return m_provider;
	}

/**
 * Method to get the result for a network request.
 * @param aTransportResult The result of transport operation
 * @param aResponse The QByteArray instance containing the network response.
 * The plugins should delete this instance once they have read the
 * data from it.
 * @param aResult [out] An output parameter to the plugin manager.If the
 * return value is SmfSendRequestAgain, QVariant will be of type
 * SmfPluginRequestData.
 * For SmfPostProviderPlugin: If last operation was retrieve(), aResult will be
 * of type QList<SmfPost>. If last operation was post() or updatePost() or
 * postDirected() or commentOnAPost() or postAppearence() or sharePost(),
 * aResult will be of type bool
 * @param aRetType [out] SmfPluginRetType
 * @param aPageResult [out] The SmfResultPage structure variable
 */
SmfPluginError FBPostProviderPlugin::responseAvailable(
		const SmfTransportResult &aTransportResult,
		QByteArray *aResponse,
		QVariant* aResult,
		SmfPluginRetType &aRetType,
		SmfResultPage &aPageResult )
	{
	writeLog("FBPostProviderPlugin::::responseAvailable");
	Q_UNUSED(aPageResult)
	//This API is slightly changed by Manasij
	SmfPluginError error;
	if(SmfTransportOpNoError == aTransportResult)
		{
		writeLog("No transport error");

		// Write the response to a file
		QFile file("c:\\data\\fbresponse.txt");
		writeLog("response data written to c:\\data\\fbresponse.txt");
		if (!file.open(QIODevice::Append | QIODevice::Text))
				 ;
		file.write(aResponse->data());
		file.close();

		QList<SmfPost> list;

		// For getting contacts
		QXmlStreamReader xml(aResponse->data());
		while (!xml.atEnd())
			{
			xml.readNext();
			if (xml.tokenType() == QXmlStreamReader::StartElement)
				{
				// If the tag is contact
				if (xml.name() == "message")
					{
					writeLog("message tag found");

					SmfPost post;
					QString descStr(xml.readElementText());
					post.setDescription(descStr);

                    list.append(post);
					}
				}
			}

		aResult->setValue(list);
		aRetType = SmfRequestComplete;
		error = SmfPluginErrNone;
		}

	else
		{
		error = SmfPluginErrInvalidRequest;
		aRetType = SmfRequestError;
		}
	delete aResponse;
	return error;
	}


/**
 * Destructor
 */
FBProviderBase::~FBProviderBase( )
	{
	}

/**
 * Method to get the Localisable name of the service.
 * @return The Localisable name of the service.
 */
QString FBProviderBase::serviceName( ) const
	{
	return m_serviceName;
	}

/**
 * Method to get the Logo of the service
 * @return The Logo of the service
 */
QImage FBProviderBase::serviceIcon( ) const
	{
	return m_serviceIcon;
	}

/**
 * Method to get the Readable service description
 * @return The Readable service description
 */
QString FBProviderBase::description( ) const
	{
	return m_description;
	}

/**
 * Method to get the Website of the service
 * @return The Website of the service
 */
QUrl FBProviderBase::serviceUrl( ) const
	{
	return m_serviceUrl;
	}

/**
 * Method to get the URL of the Application providing this service
 * @return The URL of the Application providing this service
 */
QUrl FBProviderBase::applicationUrl( ) const
	{
	return m_applicationUrl;
	}

/**
 * Method to get the Icon of the application
 * @return The Icon of the application
 */
QImage FBProviderBase::applicationIcon( ) const
	{
	return m_applicationIcon;
	}

/**
 * Method to get the Plugin specific ID
 * @return The Plugin specific ID
 */
QString FBProviderBase::pluginId( ) const
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
QString FBProviderBase::authenticationApp( QString &aProgram,
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
QString FBProviderBase::smfRegistrationId( ) const
	{
	return m_smfRegToken;
	}

void FBProviderBase::initialize()
	{
	m_serviceName = "Facebook";
	m_description = "Facebook plugin description";
	m_serviceUrl = QUrl(QString("http://api.facebook.com"));
	m_pluginId = "fbpostproviderplugin.qtplugin";
	m_authAppId = "Facebook AuthAppId";
	m_smfRegToken = "Facebook RegToken";
	}


/*
 * Export Macro
 * plugin name : fbpostproviderplugin
 * plugin class : FBPostProviderPlugin
 */
Q_EXPORT_PLUGIN2( fbpostproviderplugin, FBPostProviderPlugin )

