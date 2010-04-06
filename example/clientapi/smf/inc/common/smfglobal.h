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
 * The smfglobal.h files contains all global data common to Smf framework
 *
 */

#ifndef SMFGLOBAL_H_
#define SMFGLOBAL_H_

#include <QBuffer>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

/**
 * Exact definition to be decided later
 */
#define SMF_SERVICE_NAME(INTERFACE,INTERFACENAME)

/**
 * Indicates first page for a web query
 */
const int SMF_FIRST_PAGE=0;

/**
 * Default value for per page item for web queries
 */
const int SMF_ITEMS_PER_PAGE=10;


/**
 * Enum declaration for Network status information
 */
enum SmfNetworkStatus
	{
	SmfNetworkConnectedHome = 0,
	SmfNetworkConnectedRoaming,	// value = 1
	SmfNetworkStateNotKnown,	// value = 2
	SmfNetworkConnected,		// value = 3
	SmfNetworkNotConnected		// value = 4
	};

/**
 * Enum declaration for result of transport initialize operation
 */
enum SmfTransportInitializeResult
	{
	SmfTransportInitNetworkNotAvailable = 0,
	SmfTransportInitRoamingNetworkUsageNotEnabled,	// value = 1
	SmfTransportInitNoError							// value = 2
	};

/**
 * The enumeration used to indicate result of transport to the plugins
 */
enum SmfTransportResult
	{
	SmfTransportOpNoError = 0,
	SmfTransportOpConnectionRefusedError,
	SmfTransportOpRemoteHostClosedError,
	SmfTransportOpHostNotFoundError,
	SmfTransportOpTimeoutError,
	SmfTransportOpOperationCanceledError,
	SmfTransportOpSslHandshakeFailedError,
	SmfTransportOpProxyConnectionRefusedError,
	SmfTransportOpProxyConnectionClosedError,
	SmfTransportOpProxyNotFoundError,
	SmfTransportOpProxyTimeoutError,
	SmfTransportOpProxyAuthenticationRequiredError,
	SmfTransportOpContentAccessDenied,
	SmfTransportOpContentOperationNotPermittedError,
	SmfTransportOpContentNotFoundError,
	SmfTransportOpAuthenticationRequiredError,
	SmfTransportOpContentReSendError,
	SmfTransportOpProtocolUnknownError,
	SmfTransportOpProtocolInvalidOperationError,
	SmfTransportOpUnknownNetworkError,
	SmfTransportOpUnknownProxyError,
	SmfTransportOpUnknownContentError,
	SmfTransportOpProtocolFailure,
	SmfTransportOpUnknownError,
	SmfTransportOpIAPChanged,
	SmfTransportOpCancelled
	};

/**
 * The enumeration used to indicate result of plugin manager operations
 */
enum SmfPluginManagerResult
	{
	SmfPluginNoError = 0,
	SmfPluginNotFound,
	SmfPluginNotLoaded,
	SmfPluginLoaded,
	SmfPluginLoadError,
	SmfPluginAuthorised,
	SmfPluginNotAuthorised,
	SmfPluginUnknownService,
	SmfPluginRequestSendingFailed,
	SmfPluginSOPCheckFailed,
	SmfPluginServiceError,
	SmfPluginUnknownError
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
 * The enumeration used to track the plugin methods return type
 */
enum SmfPluginRetType
	{
	SmfSendRequestAgain = 0,
	SmfRequestComplete,
	SmfRequestError
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
	QNetworkRequest iNetworkRequest;
	
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
 * The structure used to hold the request created by the plugins
 */
struct SmfResultPage
	{
	/**
	 * The total number of items
	 */
	uint iTotalItems;
	
	/**
	 * The number of items per page
	 */
	uint iItemsPerPage;
	
	/**
	 * The total number of pages
	 */
	uint iTotalPages;
	
	/**
	 * The current fetched page number
	 */
	uint iPageNum;
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

#endif /* SMFGLOBAL_H_ */
