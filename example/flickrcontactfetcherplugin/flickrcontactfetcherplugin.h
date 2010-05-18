#ifndef _FLICKRCONTACTFETCHERPLUGIN_H
#define _FLICKRCONTACTFETCHERPLUGIN_H

// Include files
#include <smfcontactfetcherplugin.h>
#include <smfpluginutil.h>

// Forward declarations
class FlickrProviderBase;
class QVariant;
class QNetworkReply;

// Class declaration
class FlickrContactFetcherPlugin : public QObject, public SmfContactFetcherPlugin
	{
	Q_OBJECT
	Q_INTERFACES( SmfContactFetcherPlugin SmfPluginBase )

public:
	virtual ~FlickrContactFetcherPlugin( );
	
public: // From SmfContactFetcherPlugin
	
	/**
	 * Method to get the list of friends
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError friends( SmfPluginRequestData &aRequest,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE );
	
	/**
	 * Method to get the list of followers
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError followers( SmfPluginRequestData &aRequest,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE );
	
	/**
	 * Method to search for a contact
	 * @param aRequest [out] The request data to be sent to network
	 * @param aContact contact to be searched
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError search( SmfPluginRequestData &aRequest,
			const SmfContact &aContact,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE );
	
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
	SmfPluginError searchNear( SmfPluginRequestData &aRequest,
			const SmfLocation &aLocation,
			const SmfLocationSearchBoundary &aProximity,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE );
	
	/**
	 * Method to get the list of groups
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError groups( SmfPluginRequestData &aRequest,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE );
	
	/**
	 * Method to search for a contact in a group
	 * @param aRequest [out] The request data to be sent to network
	 * @param aGroup the group in which to search
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError searchInGroup( SmfPluginRequestData &aRequest,
			const SmfGroup &aGroup,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE );
	
	/**
	 * Customised method for SmfContactFetcherPlugin interface
	 * @param aRequest [out] The request data to be sent to network
	 * @param aOperation The operation type (should be known between 
	 * the client interface and the plugin)
	 * @param aData The data required to form the request (The type 
	 * of data should be known between client and the plugin)
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError customRequest( SmfPluginRequestData &aRequest, 
			const int &aOperation, QByteArray *aData );
	
public: // From SmfPluginBase
	/**
	 * The first method to be called in the plugin that implements this interface.
	 * If this method is not called, plugin may not behave as expected.
	 * Plugins are expected to save the aUtil handle and use and when required.
	 * @param aUtil The instance of SmfPluginUtil
	 */
	void initialize( SmfPluginUtil *aUtil );
	
	/**
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	SmfProviderBase* getProviderInfo( );
	
	/**
	 * Method to get the result for a network request.
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
	SmfPluginError responseAvailable( 
			const SmfTransportResult &aTransportResult, 
			QByteArray *aResponse, 
			QVariant* aResult, 
			SmfPluginRetType &aRetType,
			SmfResultPage &aPageResult );
	
private:
	/**
	 * Method called by plugins to generate a signature string from a base string
	 * @param aBaseString The base string
	 * @return The md5 hash of the base string
	 */
	QString generateSignature(const QString aBaseString);
	
	/**
	 * Method called by plugins for logging
	 * @param log string to be logged
	 */
	void writeLog(QString log) const;
    
private:
	/**
	 * Method called by plugins to generate a request data
	 * @param aRequest [out] The request data to be sent to network
	 * @param aOperation The type of http operation
	 * @param aSignatureMethod The signature method to be used
	 * @param aParams A map of parameters to its values
	 * @param aMode The mode of creation of the request
	 * @param aPostData The data to be posted (for HTTP POST 
	 * only, else it will be NULL)
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError createRequest( SmfPluginRequestData &aRequest,
			const QNetworkAccessManager::Operation aOperation, 
			const SmfSignatureMethod aSignatureMethod, 
			QMultiMap<QByteArray, QByteArray> &aParams, 
			const SmfParsingMode aMode,
			QBuffer *aPostData );

private:
	FlickrProviderBase *m_provider;
	SmfPluginUtil *m_util;
	
	};


// Class declaration
class FlickrProviderBase : public QObject, public SmfProviderBase
	{
	Q_OBJECT
	Q_INTERFACES( SmfProviderBase )

public:
	virtual ~FlickrProviderBase( );

	/**
	 * Method to get the Localisable name of the service.
	 * @return The Localisable name of the service.
	 */
	QString serviceName( ) const;
	
	/**
	 * Method to get the Logo of the service
	 * @return The Logo of the service
	 */
	QImage serviceIcon( ) const;
	
	/**
	 * Method to get the Readable service description
	 * @return The Readable service description
	 */
	QString description( ) const;
	
	/**
	 * Method to get the Website of the service
	 * @return The Website of the service
	 */
	QUrl serviceUrl( ) const;
	
	/**
	 * Method to get the URL of the Application providing this service
	 * @return The URL of the Application providing this service
	 */
	QUrl applicationUrl( ) const;
	
	/**
	 * Method to get the Icon of the application
	 * @return The Icon of the application
	 */
	QImage applicationIcon( ) const;
	
	/**
	 * Method to get the Plugin specific ID
	 * @return The Plugin specific ID
	 */
	QString pluginId( ) const;
	
	/**
	 * Method to get the ID of the authentication application 
	 * for this service
	 * @param aProgram The authentication application name
	 * @param aArguments List of arguments required for authentication app
	 * @param aMode Strting mode for authentication application
	 * @return The ID of the authentication application 
	 */
	QString authenticationApp( QString &aProgram, QStringList & aArguments, 
			QIODevice::OpenModeFlag aMode = QIODevice::ReadWrite ) const;
	
	/**
	 * Method to get the unique registration ID provided by the 
	 * Smf for authorised plugins
	 * @return The unique registration ID/token provided by the Smf for 
	 * authorised plugins
	 */
	QString smfRegistrationId( ) const;
	
private:
	friend class FlickrContactFetcherPlugin;
	void initialize();
	QString m_serviceName;
	QImage m_serviceIcon;
	QString m_description;
	QUrl m_serviceUrl;
	QUrl m_applicationUrl;
	QImage m_applicationIcon;
	QString m_pluginId;
	QString m_authAppId;
	QString m_smfRegToken;
	};

#endif /*_FLICKRCONTACTFETCHERPLUGIN_H*/
