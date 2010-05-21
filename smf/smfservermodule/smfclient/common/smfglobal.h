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
 * SMF wide global const and macros, to be shared among SMF components 
 * and SMF clients
 *
 */

#ifndef SMFGLOBAL_H
#define SMFGLOBAL_H

#ifdef Q_OS_SYMBIAN
#include <e32cmn.h>
#endif

#include <qglobal.h>
#include <QBuffer>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
/**
 * Server name 
 */

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


class SmfServerSymbianSession;
class SmfServerQtSession;


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
	SmfPluginErrDataSizeExceeded,
	SmfPluginErrParsingFailed
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
enum SmfRequestTypeID
	{
	SmfGetService,
	SmfGetServiceComplete,
	SmfTest,
	SmfTestComplete,
	SmfContactGetFriends,
	SmfContactGetFollowers,
	SmfContactSearch,
	SmfContactSearchNear,
	SmfContactGetGroups,
	SmfContactSearchInGroup,
	SmfContactRetrievePosts,
	SmfContactPost,
	SmfContactUpdatePost,
	SmfContactPostDirected,
	SmfContactCommentOnAPost,
	SmfContactPostAppearence,
	SmfContactSharePost,
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
	SmfPicturePostComment,
	SmfContactGetFriendsComplete,
	SmfContactGetFollowersComplete,
	SmfContactSearchComplete,
	SmfContactSearchNearComplete,
	SmfContactGetGroupsComplete,
	SmfContactSearchInGroupComplete,
	SmfContactRetrievePostsComplete,
	SmfContactPostComplete,
	SmfContactUpdatePostComplete,
	SmfContactPostDirectedComplete,
	SmfContactCommentOnAPostComplete,
	SmfContactPostAppearenceComplete,
	SmfContactSharePostComplete,
	SmfMusicGetLyricsComplete,
	SmfMusicGetSubtitleComplete,
	SmfMusicGetEventsOnLocComplete,
	SmfMusicGetVenueOnLocComplete,
	SmfMusicGetEventsOnVenueComplete,
	SmfMusicPostEventsComplete,
	SmfMusicGetRecommendationsComplete,
	SmfMusicGetTracksComplete,
	SmfMusicGetTrackInfoComplete,
	SmfMusicGetStoresComplete,
	SmfMusicPostCurrentPlayingComplete,
	SmfMusicGetUserInfoComplete,
	SmfMusicSearchUserComplete,
	SmfMusicGetPlaylistsComplete,
	SmfMusicGetPlaylistsOfUserComplete,
	SmfMusicAddToPlaylistComplete,
	SmfMusicPostCurrentPlayingPlaylistComplete,
	SmfPictureGetPicturesComplete,
	SmfPictureDescriptionComplete,
	SmfPictureUploadComplete,
	SmfPictureMultiUploadComplete,
	SmfPicturePostCommentComplete
	
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
 * The structure used to hold the request created by the plugins
 */
struct SmfPluginRequestData
	{
	/**
	 * Indicates the type of operation performed, like getting Friends 
	 * list, upload image etc
	 */
	SmfRequestTypeID iRequestType;
	
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


struct SmfClientAuthID
	{
#ifdef Q_OS_SYMBIAN
	TSecureId pid;
	SmfServerSymbianSession* session;
#else
	QString pid;
	SmfServerQtSession* session;
#endif
	
	};

/*enum SmfRequestTypeID
	{
	ESmfTest,
	ESmfTestComplete,
	ESmfGetService,
	ESmfGetServiceComplete,
	ESmfGetContact,
	ESmfGetContactComplete,
	ESmfGetFriend,
	ESmfGetFriendComplete,
	ESmfGetFollower,
	ESmfGetFollowerComplete,
	ESmfSearchFriend,
	ESmfSearchFriendComplete,
	ESmfGetGroup,
	ESmfGetGroupComplete,
	ESmfSearchGroup,
	ESmfSearchGroupComplete,
	ESmfGetPost,
	ESmfGetPostComplete,
	ESmfGetPicture,	//required for SmfGallery interface implementation
	ESmfGetPictureComplete,
	ESmfUploadPicture,
	ESmfUploadPictureComplete,
	ESmfPostComments,
	ESmfPostCommentsComplete,
	ESmfGetLyrics,	//required for music related implementations
	ESmfGetLyricsComplete,
	ESmfGetSubtitles,
	ESmfGetSubtitlesComplete,
	ESmfGetPlayList,
	ESmfGetPlayListComplete,
	ESmfaddToPlayList,
	ESmfaddToPlayListComplete,
	ESmfGetEvents,
	ESmfGetEventsComplete,
	ESmfGetVenues,
	ESmfGetVenuesComplete,
	ESmfPostEvent,
	ESmfPostEventComplete,
	ESmfGetMusicRecommendation,
	ESmfGetMusicRecommendationComplete,
	ESmfGetTrack,
	ESmfGetTrackComplete,
	ESmfSearchMusicUser,
	ESmfSearchMusicUserComplete,
	ESmfQueryAuthKeys,
	ESmfQueryAuthKeysComplete,
	ESmfAuthKeyExpired,
	ESmfAuthKeyExpiredComplete
	};*/
/**
 * Smf wide errors
 */
enum SmfError
	{
	SmfNoError,
	SmfInvalidInterface,
	SmfNoAuthorizedPlugin,
	SmfClientAuthFailed,
	SmfPluginErrorTooManyRequest,
	SmfPluginErrorRequestQuotaExceeded,
	SmfPluginErrorInvalidRequest,
	SmfPluginErrorUserNotLoggedIn,
	SmfPluginErrorAuthenticationExpired,
	SmfPluginErrorPermissionDenied,
	SmfPluginErrorInvalidApplication,
	SmfPluginErrorServiceUnavailable,
	SmfPluginErrorServiceTemporaryUnavailable,
	SmfPluginErrorFormatNotSupported, 
	SmfPluginErrorDataSizeExceeded ,
	SmfpluginNotFound,
	SmfpluginNotLoaded,
	SmfpluginLoaded,
	SmfpluginLoadError,
	SmfpluginAuthorised,
	SmfpluginNotAuthorised,
	SmfpluginRequestCreated,
	SmfpluginRequestCreationFailed,
	SmfpluginUnknownService,
	SmfpluginRequestSendingFailed,
	SmfpluginSOPCheckFailed,
	SmfpluginServiceError,
	SmfpluginResponseParsed,
	SmfpluginResponseParseFailure,
	SmfpluginSendRequestAgain,
	SmfpluginUnknownError,
	SmftransportOpConnectionRefusedError,
	SmftransportOpRemoteHostClosedError,
	SmftransportOpHostNotFoundError,
	SmftransportOpTimeoutError,
	SmftransportOpOperationCanceledError,
	SmftransportOpSslHandshakeFailedError,
	SmftransportOpProxyConnectionRefusedError,
	SmftransportOpProxyConnectionClosedError,
	SmftransportOpProxyNotFoundError,
	SmftransportOpProxyTimeoutError,
	SmftransportOpProxyAuthenticationRequiredError,
	SmftransportOpContentAccessDenied,
	SmftransportOpContentOperationNotPermittedError,
	SmftransportOpContentNotFoundError,
	SmftransportOpAuthenticationRequiredError,
	SmftransportOpContentReSendError,
	SmftransportOpProtocolUnknownError,
	SmftransportOpProtocolInvalidOperationError,
	SmftransportOpUnknownNetworkError,
	SmftransportOpUnknownProxyError,
	SmftransportOpUnknownContentError,
	SmftransportOpProtocolFailure,
	SmftransportOpUnknownError,
	SmftransportOpIAPChanged,
	SmftransportOpCancelled,
	SmftransportInitNetworkNotAvailable ,
	SmftransportInitRoamingNetworkUsageNotEnabled	
	};

/**
 * Smf wide panics
 */
enum SmfPanic
	{
	SmfRequestPending//to allow one outstanding request per session
	};
//interface names
const QString contactFetcherInterface("org.symbian.smf.client.contact.fetcher");
//TODO:- changed to match PM for the time being, PM must change later
const QString postProviderInterface("posts");
//const QString postProviderInterface("org.symbian.smf.client.contact.posts");
const QString galleryInterface("org.symbian.smf.client.gallery");



#endif /* SMFGLOBAL_H_ */
