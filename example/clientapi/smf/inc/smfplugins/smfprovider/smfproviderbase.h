/**
 * @file	smfproviderbase.h
 * @author  Nalina Hariharan, Sasken Communication Technologies Ltd - Initial contribution
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2010 Sasken Communication Technologies Ltd. 
 * All rights reserved.
 * This component and the accompanying materials are made available 
 * under the terms of the "{License}" 
 * which accompanies  this distribution, and is available 
 * at the URL "{LicenseUrl}".
 * 
 * @section DESCRIPTION
 *
 * The Provider Base class is the class that has to be contained as a member 
 * in all Plug-in Interfaces. 
 */

#ifndef SMFPROVIDERBASE_H_
#define SMFPROVIDERBASE_H_

#include <QImage>
#include <QUrl>
#include <QBuffer>
#include <QNetworkAccessManager>


/**
 * The enumeration used to track the plugin methods return type
 */
enum SmfPluginRetType
	{
	SmfSendRequestAgain = 0,
	SmfRequestComplete
	};

/**
 * The enumeration used to indicate result of transport to the plugins
 */
enum SmfTransportResult
	{
	SmfNoError = 0,
	SmfNetworkTimeOut,
	SmfIAPChanged,
	SmfUnknownError
	};

/**
 * The enumeration used to denote errors reported by plugin
 * Smf can't continue without handling these errors
 */
enum SmfPluginError
	{
	SmfPluginErrNone = 0, 
	SmfPluginErrTooManyRequest,
	SmfPluginErrRequestQuotaExceeded,
	SmfPluginErrInvalidRequest,
	SmfPluginErrUserNotLoggedIn,
	SmfPluginErrAuthenticationExpired,
	SmfPluginErrPermissionDenied,
	SmfPluginErrInvalidApplication,
	SmfPluginErrServiceUnavailable,
	SmfPluginErrServiceTemporaryUnavailable,
	SmfPluginErrFormatNotSupported, 
	SmfPluginErrDataSizeExceeded  
	};


/**
 * The enumeration used to indicate the type of network operation done
 */
enum SmfRequestOperation
	{
	SmfContactGetFriends = 0,
	SmfContactGetFollowers,
	SmfContactSearch,
	SmfContactGerGroups,
	SmfContactSearchInGroup,
	SmfContactGetPosts,
	SmfContactUpdatePost,
	SmfContactUpdatePostDirected,
	SmfContactPostAppearence,
	SmfMusicGetLyrics,
	SmfMusicGetSubtitle,
	SmfMusicGetEventsOnLoc,
	SmfMusicGetVenueOnLoc,
	SmfMusicGetEventsOnVenue,
	SmfMusicPostEvents,
	SmfMusicGetRecommendations,
	SmfMusicGetTracks,
	SmfMusicGetTrackInfo,
	SmfMusicGetStores,
	SmfMusicPostCurrentPlaying,
	SmfMusicGetUserInfo,
	SmfMusicSearchUser,
	SmfMusicGetPlaylists,
	SmfMusicGetPlaylistsOfUser,
	SmfMusicAddToPlaylist,
	SmfMusicPostCurrentPlayingPlaylist,
	SmfPictureGetPictures,
	SmfPictureDescription,
	SmfPictureUpload,
	SmfPictureMultiUpload,
	SmfPicturePostComment
	
	};


/**
 * The structure used to track the data usage of each plugins
 */
struct SmfPluginDataUsage
	{
	/**
	 * Application that called the plugin
	 */
	QString iAppName;
	
	/**
	 * Interface implemented by the plugin
	 */
	QString iInterfaceName;
	
	/**
	 * The service provider
	 */
	QString iServiceProviderName;
	
	/**
	 * number of bytes sent for this plugin
	 */
	uint iBytesSent;
	
	/**
	 * number of bytes received for this plugin
	 */
	uint iBytesReceived;
	
	};


/**
 * The structure used to hold the request created by the plugins
 */
struct SmfPluginRequestData
	{
	/**
	 * Indicates the type of operation performed, like getting Friends 
	 * list, upload image etc
	 */
	SmfRequestOperation iRequestType;
	
	/**
	 * The QNetworkRequest that has to be filled up by the plugins
	 */
	QNetworkRequest *iNetworkRequest;
	
	/**
	 * The data to be posted in case of HTTP POST operation
	 */
	QBuffer *iPostData;
	
	/**
	 * The type of HTTP transaction, like GET, POST etc
	 */
	QNetworkAccessManager::Operation iHttpOperationType;
	
	};

/**
 * The Provider Base class is the class that has to be contained as a member 
 * in all Plug-in Interfaces.
 * 
 * All of plug-ins should contain this class as a member and should also 
 * contain a public method to get instance of this class. 
 */
class SmfProviderBase : public QObject
	{
	Q_OBJECT

public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfProviderBase( QObject* aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfProviderBase( );

	/**
	 * Method to get the Localisable name of the service.
	 * @return The Localisable name of the service.
	 */
	QString serviceName( );
	
	/**
	 * Method to get the Logo of the service
	 * @return The Logo of the service
	 */
	QImage serviceIcon( );
	
	/**
	 * Method to get the Readable service description
	 * @return The Readable service description
	 */
	QString description( );
	
	/**
	 * Method to get the Website of the service
	 * @return The Website of the service
	 */
	QUrl serviceUrl( );
	
	/**
	 * Method to get the URL of the Application providing this service
	 * @return The URL of the Application providing this service
	 */
	QUrl applicationUrl( );
	
	/**
	 * Method to get the Icon of the application
	 * @return The Icon of the application
	 */
	QImage applicationIcon( );
	
	/**
	 * Method to get the Plugin specific ID
	 * @return The Plugin specific ID
	 */
	uint pluginId( );
	
	/**
	 * Method to get the ID of the authentication application 
	 * for this service
	 * @param aProgram The authentication application name
	 * @param aArguments List of arguments required for authentication app
	 * @param aMode Strting mode for authentication application
	 * @return The ID of the authentication application 
	 */
	uint authenticationApp( QString &aProgram, QStringList & aArguments, 
			QIODevice::OpenModeFlag aMode = QIODevice::ReadWrite );	
	
	/**
	 * Method to get the unique registration ID provided by the 
	 * Smf for authorised plugins
	 * @return The unique registration ID/token provided by the Smf for 
	 * authorised plugins
	 */
	QString smfRegistrationId( );
	
	/**
	 * Method to get the data usage of each plugin
	 * @param aUsage The data usage structure
	 */
	void getDataUsage( SmfPluginDataUsage &aUsage );
	
	/**
	 * Method to get the reference count for loading.
	 * The reference count increased with each load request and decreased 
	 * with each unload request
	 * @return The reference count value for loading of the plugins
	 */
	uint getLoadRefCount( );
	
private:
	QString m_serviceName;	// the Localisable name of the service
	QImage m_serviceIcon;	// the Logo of the service
	QString m_description;	// the Readable service description
	QUrl m_serviceUrl;		// the Website of the service
	QUrl m_applicationUrl; 	// URL of the Application providing this service
	QImage m_applicationIcon;	//Icon of the application
	uint m_pluginId;		// The Plugin specific ID
	uint m_authenticationAppId; // ID of the authentication application for this service
	QString m_registrationId; 	// unique registration ID provided by the 
							// Smf for authorised plugins
	SmfPluginDataUsage m_usage;	//data usage of each plugin
	uint m_loadRefCount;	// reference count increased with each load request 
							// and decreased with each unload request
	};

#endif /* SMFPROVIDERBASE_H_ */
