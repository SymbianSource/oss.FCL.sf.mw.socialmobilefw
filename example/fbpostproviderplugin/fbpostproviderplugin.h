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
 * The Plugin that manages post operations on the logged in user's facebook account
 *
 */

#ifndef _FBPOSTPROVIDERPLUGIN_H
#define _FBPOSTPROVIDERPLUGIN_H

// Include files
#include <QDateTime>
#include <smfpostproviderplugin.h>

// Forward declarations
class FBProviderBase;
class QVariant;


/**
 * The Plugin that manages post operations on the logged in user's 
 * facebook account
 */
class FBPostProviderPlugin : public QObject, public SmfPostProviderPlugin
{
	Q_OBJECT
	Q_INTERFACES( SmfPostProviderPlugin SmfPluginBase )

public:
	/**
	 * Destructor
	 */
	virtual ~FBPostProviderPlugin( );
	
public: // From SmfPostProviderPlugin interface
	
	/**
	 * Method that returns maximum no of chars (unicode) that service 
	 * provider can post without truncation. Negative value means 
	 * no limit
	 * @return Max characters that can be posted without truncation
	 */
	qint32 maxCharsInPost( ) const;
	
	/**
	 * Method that returns maximum no of items that can be returned 
	 * in a single query to getPosts. Negative value means feature 
	 * not supported.
	 * @return Max items that can be returned in a single query
	 */
	qint32 maxItems( ) const;

	/**
	 * <Method that returns all the formatting of posts that this 
	 * service provider supports. May return 0 items to mean 
	 * only QString is supported.
	 * @return Supported formats of posts
	 */
	QVector<QTextFormat> supportedFormats ( ) const;
	
	/**
	 * Method that returns whether this SP supports Appearence
	 * @return Returns true if Appearance is supported, else false. 
	 * @see SmfAppearenceInfo
	 */
	bool supportsAppearence ( ) const;
	
	/**
	 * Method to get the latest posts
	 * @param aRequest [out] The request data to be sent to network
	 * @param aUser The user's contact in this SP, omit for self contact
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError retrieve( SmfPluginRequestData &aRequest,
			const SmfContact *aUser=0,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE );

	
	/**
	 * Method to update a post to own area.
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPostData The post data to be posted
	 * @param aLocation The location
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError post( SmfPluginRequestData &aRequest,
			const SmfPost &aPostData, 
			const SmfLocation &aLocation );
	
	/**
	 * Method to update the last post to own area with new data
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPostData The edited/new data to be posted
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError updatePost( SmfPluginRequestData &aRequest,
			const SmfPost &aPostData );
	
	/**
	 * Method to update a post to a particular contact
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPostData The post data to be posted
	 * @param aContact The contact where the data has to be posted
	 * @param aLocation The location
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError postDirected( SmfPluginRequestData &aRequest,
			const SmfPost &aPostData, 
			const SmfContact &aContact, 
			const SmfLocation *aLocation = NULL );
	
	
	/**
	 * Method to post a comment on a post.
	 * @param aRequest [out] The request data to be sent to network
	 * @param aTarget Post on which comment has to be posted
	 * @param aComment comment to be posted
	 * @param aLocation location data
	 */
	SmfPluginError commentOnAPost(SmfPluginRequestData &aRequest,
			const SmfPost &aTarget,
			const SmfPost &aComment, 
			const SmfLocation *aLocation = NULL);
	
	/**
	 * Method to update the presence information of the user
	 * @param aRequest [out] The request data to be sent to network
	 * @param aAppearence The appearence information
	 * @param aStatus The status string
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError postAppearence( SmfPluginRequestData &aRequest,  
			const SmfPresenceInfo &aAppearence,
			const QString &aStatus );
	
	/**
	 * Share /a contact's post to user's friends and followers
	 * (e.g. retweet in twitter, share on facebook)
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPostData data to be posted
	 * @param aContact contact to which the post belonged
	 * @param aEdited whether user changed items within the post
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError sharePost( SmfPluginRequestData &aRequest, 
			const SmfPost &aPostData, 
			const SmfContact &aContact, 
			const bool &aEdited);
	
	/**
	 * Customised method for SmfPostProviderPlugin interface
	 * @param aRequest [out] The request data to be sent to network
	 * @param aOperation The operation type (should be known between 
	 * the client interface and the plugin)
	 * @param aData The data required to form the request (The type 
	 * of data should be known between client and the plugin)
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError customRequest( SmfPluginRequestData &aRequest, 
			const int &aOperation, QByteArray *aData );
	
public: // From SmfPluginBase interface
	/**
	 * The first method to be called in the plugin that implements this interface.
	 * If this method is not called, plugin may not behave as expected.
	 */
	void initialize( );
	
	/**
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	SmfProviderBase* getProviderInfo( );
	
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
	 * For SmfPostProviderPlugin: If last operation was retrieve(), aResult will be 
	 * of type QList<SmfPost>. If last operation was post() or updatePost() or 
	 * postDirected() or commentOnAPost() or postAppearence() or sharePost(), 
	 * aResult will be of type bool
	 * @param aRetType [out] SmfPluginRetType
	 * @param aPageResult [out] The SmfResultPage structure variable
	 */
	SmfPluginError responseAvailable( 
			const SmfRequestTypeID aOperation,
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
	QString generateSignature( const QString aBaseString );
	
	/**
	 * Method to get the user's facebook ID
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError getFacebookUserId( SmfPluginRequestData &aRequest );

	/**
	 * Method to get the user's posts
	 * @param aRequest [out] The request data to be sent to network
	 * @param aUser The user's contact in this SP, omit for self contact
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	SmfPluginError getPosts( SmfPluginRequestData &aRequest,
			const SmfContact *aUser,
			const int aPageNum , 
			const int aItemsPerPage );
	
	/**
	 * Method to interpret the key sets obtained from credential manager 
	 * @param aApiKey [out] The api key
	 * @param aApiSecret [out] The api secret
	 * @param aSessionKey [out] The session key
	 * @param aSessionSecret [out] The session secret
	 */
	void fetchKeys(	QString &aApiKey, 
			QString &aApiSecret, 
			QString &aSessionKey, 
			QString &aSessionSecret );
	
private:
	FBProviderBase *m_provider;
};


/**
 * The Plugin class that implements SmfProviderBase for this facebook plugin
 */
class FBProviderBase : public QObject, public SmfProviderBase
	{
	Q_OBJECT
	Q_INTERFACES( SmfProviderBase )

public:
	/**
	 * Destructor
	 */
	virtual ~FBProviderBase( );

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
	* Method to get the list of interfaces that this provider support
	* @return List of supported Interafces
	*/
	QList<QString> supportedInterfaces( ) const;
	
	/**
	* Method to get the list of languages supported by this service provider
	* @return a QStringList of languages supported by this service 
	* provider in 2 letter ISO 639-1 format.
	*/
	QStringList supportedLanguages( ) const;
	
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
	 * Method to get the authentication application process name
	 * @return The authentication application process name (eg: "FlickrAuthApp.exe")
	 */
	QString authenticationAppName( ) const;
	
	/**
	 * Method to get the unique registration ID provided by the 
	 * Smf for authorised plugins
	 * @return The unique registration ID/token provided by the Smf for 
	 * authorised plugins
	 */
	QString smfRegistrationId( ) const;
	
private:
	/**
	 * Method that initializes this class. This method should be called 
	 * from the initialize() method of the FBPostProviderPlugin class
	 */
	void initialize();
	
private:
	friend class FBPostProviderPlugin;
	QString m_serviceName;
	QImage m_serviceIcon;
	QString m_description;
	QUrl m_serviceUrl;
	QUrl m_applicationUrl;
	QImage m_applicationIcon;
	QString m_pluginId;
	QString m_authAppId;
	QString m_authAppName;
	QString m_smfRegToken;
	QList<QString> m_supportedInterfaces;
	QStringList m_supportedLangs;
	QDateTime m_validity;
	};

#endif /*_FBPOSTPROVIDERPLUGIN_H*/
