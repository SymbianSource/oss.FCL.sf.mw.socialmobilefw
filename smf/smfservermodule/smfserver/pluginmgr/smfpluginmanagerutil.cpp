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
 * The Plugin Manager util class performs all plugin functional 
 * specific functionalities
 *
 */

// plugin interfaces
#include <smfcontactfetcherplugin.h>
#include <smfpostproviderplugin.h>
#include <smflyricsserviceplugin.h>
#include <smfmusiceventsplugin.h>
#include <smfmusicsearchplugin.h>
#include <smfmusicserviceplugin.h>
#include <smfplaylistserviceplugin.h>
#include <smfgalleryplugin.h>
#include <smfprovider.h>

#include "smfpluginmanagerutil.h"
#include "smfpluginmanager.h"

// Static data initialisation
SmfPluginManagerUtil* SmfPluginManagerUtil::m_myInstance = NULL;


/**
 * Method to get the instance of SmfPluginManagerUtil class
 * @param aParent The parent object
 * @return The instance of SmfPluginManagerUtil class
 */
SmfPluginManagerUtil* SmfPluginManagerUtil::getInstance ( QObject *aParent )
	{
	if(NULL == m_myInstance)
		m_myInstance = new SmfPluginManagerUtil( aParent );
	return m_myInstance;
	}


/**
 * Constructor with default argument
 * @param aParent The parent object
 */
SmfPluginManagerUtil::SmfPluginManagerUtil ( QObject */*aParent*/ )
	{
	}


/**
 * Destructor
 */
SmfPluginManagerUtil::~SmfPluginManagerUtil ( )
	{
	if(m_myInstance)
		delete m_myInstance;
	}


/**
 * Method called by Plugin Manager to create a web query.
 * This method calls the respective plugin methods for creation of 
 * web queries using the aOperation and aInputData parameters.
 * @param aInstance Instance of the loaded plugin that perform this operation
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @param aResult [out] SmfPluginManagerResult, The result of the operation
 * @see smfglobal.h
 */
void SmfPluginManagerUtil::createRequest ( QObject* aInstance, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfPluginManagerResult &aResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::createRequest");
	SmfPluginError pluginErrorVal = SmfPluginErrNone;
	
	// Find the type of service required
	switch(aOperation)
		{
		case SmfContactGetFriends:
		case SmfContactGetFollowers:
		case SmfContactSearch:
		case SmfContactSearchNear:
		case SmfContactGetGroups:
		case SmfContactSearchInGroup:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createContactFetcherRequest(aInstance, aOperation, 
					aInputData, aReqData, aResult);
			break;
			
		case SmfContactRetrievePosts:
		case SmfContactPost:
		case SmfContactUpdatePost:
		case SmfContactPostDirected:
		case SmfContactCommentOnAPost:
		case SmfContactPostAppearence:
		case SmfContactSharePost:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createContactPostRequest(aInstance, aOperation, 
					aInputData, aReqData, aResult);
			break;
			
		case SmfMusicGetLyrics:
		case SmfMusicGetSubtitle:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicLyricsRequest(aInstance, aOperation, 
					aInputData, aReqData, aResult);
			break;
	
		case SmfMusicGetEventsOnLoc:
		case SmfMusicGetVenueOnLoc:
		case SmfMusicGetEventsOnVenue:
		case SmfMusicPostEvents:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicEventsRequest(aInstance, aOperation, 
					aInputData, aReqData, aResult);
			break;
			
		case SmfMusicGetRecommendations:
		case SmfMusicGetTracks:
		case SmfMusicGetTrackInfo:
		case SmfMusicGetStores:
		case SmfMusicPostCurrentPlaying:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicSearchRequest(aInstance, aOperation, 
					aInputData, aReqData, aResult);
			break;
			
		case SmfMusicGetUserInfo:
		case SmfMusicSearchUser:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicServiceRequest(aInstance, aOperation, 
					aInputData, aReqData, aResult);
			break;
			
		case SmfMusicGetPlaylists:
		case SmfMusicGetPlaylistsOfUser:
		case SmfMusicAddToPlaylist:
		case SmfMusicPostCurrentPlayingPlaylist:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicPlaylistRequest(aInstance, aOperation, 
					aInputData, aReqData, aResult);
			break;
			
		case SmfPictureGetPictures:
		case SmfPictureDescription:
		case SmfPictureUpload:
		case SmfPictureMultiUpload:
		case SmfPicturePostComment:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createGalleryRequest(aInstance, aOperation, 
					aInputData, aReqData, aResult);
			break;
			
		default:
			SmfPluginManager::getInstance()->server()->writeLog("Unknown service type!!!");
			aResult = SmfPluginUnknownService;
		}
	if(SmfPluginErrNone == pluginErrorVal)
		aResult = SmfPluginRequestCreated;
	else
		aResult = SmfPluginRequestCreationFailed;
	}


/**
 * Method to create a web query to fetch contact details.
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @param aResult [out] SmfPluginManagerResult, The result of the operation
 * @return SmfPluginError
 * @see smfglobal.h
 */
SmfPluginError SmfPluginManagerUtil::createContactFetcherRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfPluginManagerResult &aResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::createContactFetcherRequest");
	
	// typecast instance to the contact - fetcher type, here SmfContactFetcherPlugin
	SmfContactFetcherPlugin *plugin = qobject_cast<SmfContactFetcherPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrNone;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;

		switch(aOperation)
			{
			case SmfContactGetFriends:
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->friends(aReqData, pageNum, itemPerPage);
				break;
				
			case SmfContactGetFollowers:
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->followers(aReqData, pageNum, itemPerPage);
				break;
				
			case SmfContactSearch:
				{
				SmfContact searchContact;
				stream>>searchContact;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->search(aReqData, searchContact, pageNum, itemPerPage);
				break;
				}
				
			case SmfContactSearchNear:
				{
				SmfLocation location;
				int i;
				stream>>i;
				SmfLocationSearchBoundary proximity = (SmfLocationSearchBoundary)i;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->searchNear(aReqData, location, proximity, pageNum, itemPerPage);
				break;
				}
				
			case SmfContactGetGroups:
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->groups(aReqData, pageNum, itemPerPage);
				break;
				
			case SmfContactSearchInGroup:
				{
				SmfGroup group;
				stream>>group;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->searchInGroup(aReqData, group, pageNum, itemPerPage);
				break;
				}
				
			default:
				// do nothing, unknown service
				aResult = SmfPluginUnknownService;
				SmfPluginManager::getInstance()->server()->writeLog("No API found for this operation type!!!");
				return SmfPluginErrInvalidRequest;
			}

			if(SmfPluginErrNone == pluginErrorVal)
				aResult = SmfPluginRequestCreated;
			else
				aResult = SmfPluginServiceError;
		}
		else
			aResult = SmfPluginLoadError;
	
	return pluginErrorVal;
	}
	

/**
 * Method to create a web query to do post operation on contacts 
 * (like scraps or messages)
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @param aResult [out] SmfPluginManagerResult, The result of the operation
 * @return SmfPluginError
 * @see smfglobal.h
 */
SmfPluginError SmfPluginManagerUtil::createContactPostRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfPluginManagerResult &aResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::createContactPostRequest");
	
	// typecast instance to SmfPostProviderPlugin
	SmfPostProviderPlugin *plugin = qobject_cast<SmfPostProviderPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrNone;	
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		SmfContact contact;
		SmfPost post;
		SmfLocation location;
		int pageNum, itemPerPage;
	
		switch(aOperation)
			{
			case SmfContactRetrievePosts:
				stream>>contact;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->retrieve(aReqData, &contact, pageNum, itemPerPage);
				break;
				
			case SmfContactPost:
				stream>>post;
				stream>>location;
				pluginErrorVal = plugin->post(aReqData, post, location);
				break;
				
			case SmfContactUpdatePost:
				stream>>post;
				pluginErrorVal = plugin->updatePost(aReqData, post);
				break;
				
			case SmfContactPostDirected:
				stream>>post;
				stream>>contact;
				stream>>location;
				pluginErrorVal = plugin->postDirected(aReqData, post, contact, &location);
				break;
				
			case SmfContactCommentOnAPost:
				{
				SmfPost post2;
				stream>>post;
				stream>>post2;
				stream>>location;
				pluginErrorVal = plugin->commentOnAPost(aReqData, post, post2, &location);
				break;
				}
				
			case SmfContactPostAppearence:
				{
				QString status;
				int i;
				stream>>i;
				SmfPresenceInfo presence = (SmfPresenceInfo)i;
				stream>>status;
				pluginErrorVal = plugin->postAppearence(aReqData, presence, status);
				break;
				}
				
			case SmfContactSharePost:
				{
				bool edited;
				stream>>post;
				stream>>contact;
				stream>>edited;
				pluginErrorVal = plugin->sharePost(aReqData, post, contact, edited);
				break;
				}
				
			default:
				// do nothing, unknown service
				aResult = SmfPluginUnknownService;
				SmfPluginManager::getInstance()->server()->writeLog("No API found for this operation type!!!");
				return SmfPluginErrInvalidRequest;
			}
	
			if(SmfPluginErrNone == pluginErrorVal)
				aResult = SmfPluginRequestCreated;
			else
				aResult = SmfPluginServiceError;
		}
		else
			aResult = SmfPluginLoadError;
	
	return pluginErrorVal;
	}
	


/**
 * Method to create a web query to do music lyrics or subtitle search
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @param aResult [out] SmfPluginManagerResult, The result of the operation
 * @return SmfPluginError
 * @see smfglobal.h
 */
SmfPluginError SmfPluginManagerUtil::createMusicLyricsRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfPluginManagerResult &aResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::createMusicLyricsRequest");
	
	// typecast instance to SmfLyricsServicePlugin
	SmfLyricsServicePlugin *plugin = qobject_cast<SmfLyricsServicePlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrNone;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		SmfTrackInfo trackInfo;
		int pageNum, itemPerPage;

		switch(aOperation)
			{
			case SmfMusicGetLyrics:
				stream>>trackInfo;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->lyrics(aReqData, trackInfo, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetSubtitle:
				{
				stream>>trackInfo;
				int i;
				stream>>i;
				SmfSubtitleSearchFilter filter = (SmfSubtitleSearchFilter)i;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->subtitles(aReqData, trackInfo, filter, pageNum, itemPerPage);
				break;
				}
				
			default:
				// do nothing, unknown service
				aResult = SmfPluginUnknownService;
				SmfPluginManager::getInstance()->server()->writeLog("No API found for this operation type!!!");
				return SmfPluginErrInvalidRequest;
			}

			if(SmfPluginErrNone == pluginErrorVal)
				aResult = SmfPluginRequestCreated;
			else
				aResult = SmfPluginServiceError;
		}
		else
			aResult = SmfPluginLoadError;
	
	return pluginErrorVal;
	}


/**
 * Method to create a web query to do music event search or event post operation 
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @param aResult [out] SmfPluginManagerResult, The result of the operation
 * @return SmfPluginError
 * @see smfglobal.h
 */
SmfPluginError SmfPluginManagerUtil::createMusicEventsRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfPluginManagerResult &aResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::createMusicEventsRequest");
	
	// typecast instance to SmfMusicEventsPlugin
	SmfMusicEventsPlugin *plugin = qobject_cast<SmfMusicEventsPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrNone;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;
		QtMobility::QGeoPositionInfo location;

		switch(aOperation)
			{
			case SmfMusicGetEventsOnLoc:
				stream>>location;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->events(aReqData, location, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetVenueOnLoc:
				stream>>location;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->venues(aReqData, location, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetEventsOnVenue:
				{
				SmfLocation venue;
				stream>>venue;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->events(aReqData, venue, pageNum, itemPerPage);
				break;
				}
				
			case SmfMusicPostEvents:
				{
				QList<SmfEvent> list;
				stream>>list;
				pluginErrorVal = plugin->postEvents(aReqData, list);
				break;
				}
				
			default:
				// do nothing, unknown service
				aResult = SmfPluginUnknownService;
				SmfPluginManager::getInstance()->server()->writeLog("No API found for this operation type!!!");
				return SmfPluginErrInvalidRequest;
			}

			if(SmfPluginErrNone == pluginErrorVal)
				aResult = SmfPluginRequestCreated;
			else
				aResult = SmfPluginServiceError;
		}
		else
			aResult = SmfPluginLoadError;
	
	return pluginErrorVal;
	}
	


/**
 * Method to create a web query to do music search operation 
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @param aResult [out] SmfPluginManagerResult, The result of the operation
 * @return SmfPluginError
 * @see smfglobal.h
 */
SmfPluginError SmfPluginManagerUtil::createMusicSearchRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfPluginManagerResult &aResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::createMusicSearchRequest");
	
	// typecast instance to SmfMusicSearchPlugin
	SmfMusicSearchPlugin *plugin = qobject_cast<SmfMusicSearchPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrNone;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;
		SmfTrackInfo trackInfo;

		switch(aOperation)
			{
			case SmfMusicGetRecommendations:
				stream>>trackInfo;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->recommendations(aReqData, trackInfo, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetTracks:
				stream>>trackInfo;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->tracks(aReqData, trackInfo, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetTrackInfo:
				{
				SmfMusicFingerPrint fp;
				stream>>fp;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->trackInfo(aReqData, fp, pageNum, itemPerPage);
				break;
				}
				
			case SmfMusicGetStores:
				stream>>trackInfo;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->stores(aReqData, trackInfo, pageNum, itemPerPage);
				break;
				
			case SmfMusicPostCurrentPlaying:
				stream>>trackInfo;
				pluginErrorVal = plugin->postCurrentPlaying(aReqData, trackInfo);
				break;
				
			default:
				// do nothing, unknown service
				aResult = SmfPluginUnknownService;
				SmfPluginManager::getInstance()->server()->writeLog("No API found for this operation type!!!");
				return SmfPluginErrInvalidRequest;
			}

			if(SmfPluginErrNone == pluginErrorVal)
				aResult = SmfPluginRequestCreated;
			else
				aResult = SmfPluginServiceError;
		}
		else
			aResult = SmfPluginLoadError;
	
	return pluginErrorVal;
	}
	

/**
 * Method to create a web query to do music service request operation 
 * like getting self profile or search profiles of other users
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @param aResult [out] SmfPluginManagerResult, The result of the operation
 * @return SmfPluginError
 * @see smfglobal.h
 */
SmfPluginError SmfPluginManagerUtil::createMusicServiceRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfPluginManagerResult &aResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::createMusicServiceRequest");
	
	// typecast instance to SmfMusicServicePlugin
	SmfMusicServicePlugin *plugin = qobject_cast<SmfMusicServicePlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrNone;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;

		switch(aOperation)
			{
			case SmfMusicGetUserInfo:
				pluginErrorVal = plugin->userInfo(aReqData);
				break;
				
			case SmfMusicSearchUser:
				{
				SmfLocation place;
				stream>>place;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->searchUser(aReqData, place, pageNum, itemPerPage);
				break;
				}
				
			default:
				// do nothing, unknown service
				aResult = SmfPluginUnknownService;
				SmfPluginManager::getInstance()->server()->writeLog("No API found for this operation type!!!");
				return SmfPluginErrInvalidRequest;
			}

			if(SmfPluginErrNone == pluginErrorVal)
				aResult = SmfPluginRequestCreated;
			else
				aResult = SmfPluginServiceError;
		}
		else
			aResult = SmfPluginLoadError;
	
	return pluginErrorVal;
	}
	


/**
 * Method to create a web query to do music playlist related operation
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @param aResult [out] SmfPluginManagerResult, The result of the operation
 * @return SmfPluginError
 * @see smfglobal.h
 */
SmfPluginError SmfPluginManagerUtil::createMusicPlaylistRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfPluginManagerResult &aResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::createMusicPlaylistRequest");
	
	// typecast instance to SmfPlaylistServicePlugin
	SmfPlaylistServicePlugin *plugin = qobject_cast<SmfPlaylistServicePlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrNone;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;
		SmfPlaylist playlist;

		switch(aOperation)
			{
			case SmfMusicGetPlaylists:
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->playlists(aReqData, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetPlaylistsOfUser:
				{
				SmfMusicProfile user;
				stream>>user;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->playlistsOf(aReqData, user, pageNum, itemPerPage);
				break;
				}
				
			case SmfMusicAddToPlaylist:
				{
				QList<SmfTrackInfo> list;
				stream>>playlist;
				stream>>list;
				pluginErrorVal = plugin->addToPlaylist(aReqData, playlist, list);
				break;
				}
				
			case SmfMusicPostCurrentPlayingPlaylist:
				stream>>playlist;
				pluginErrorVal = plugin->postCurrentPlayingPlaylist(aReqData, playlist);
				break;
				
			default:
				// do nothing, unknown service
				aResult = SmfPluginUnknownService;
				SmfPluginManager::getInstance()->server()->writeLog("No API found for this operation type!!!");
				return SmfPluginErrInvalidRequest;
			}

			if(SmfPluginErrNone == pluginErrorVal)
				aResult = SmfPluginRequestCreated;
			else
				aResult = SmfPluginServiceError;
		}
		else
			aResult = SmfPluginLoadError;
	
	return pluginErrorVal;
	}
	

/**
 * Method to create a web query to do gallery related operation
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @param aResult [out] SmfPluginManagerResult, The result of the operation
 * @return SmfPluginError
 * @see smfglobal.h
 */
SmfPluginError SmfPluginManagerUtil::createGalleryRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfPluginManagerResult &aResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::createGalleryRequest");
	
	// typecast instance to SmfGalleryPlugin
	SmfGalleryPlugin *plugin = qobject_cast<SmfGalleryPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrNone;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		SmfPicture picture;
		
		switch(aOperation)
			{
			case SmfPictureGetPictures:
				{
				int pageNum, itemPerPage;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->pictures(aReqData, pageNum, itemPerPage);
				break;
				}
				
			case SmfPictureDescription:
				stream>>picture;
				pluginErrorVal = plugin->description(aReqData, picture);
				break;
				
			case SmfPictureUpload:
				stream>>picture;
				pluginErrorVal = plugin->upload(aReqData, picture);
				break;
				
			case SmfPictureMultiUpload:
				{
				QList<SmfPicture> list;
				stream>>list;
				pluginErrorVal = plugin->upload(aReqData, list);
				break;
				}
				
			case SmfPicturePostComment:
				{
				SmfComment comment;
				stream>>picture;
				stream>>comment;
				pluginErrorVal = plugin->postComment(aReqData, picture, comment);
				break;
				}
				
			default:
				// do nothing, unknown service
				aResult = SmfPluginUnknownService;
				SmfPluginManager::getInstance()->server()->writeLog("No API found for this operation type!!!");
				return SmfPluginErrInvalidRequest;
			}

			if(SmfPluginErrNone == pluginErrorVal)
				aResult = SmfPluginRequestCreated;
			else
				aResult = SmfPluginServiceError;
		}
		else
			aResult = SmfPluginLoadError;
	
	return pluginErrorVal;
	}
	
	
/**
 * Method called by Plugin Manager when network response is available
 * @param aInstance The instance of the loaded plugin that requested 
 * the network operation.
 * @param aOperation The type of operation to be performed
 * @param aTransportResult The result of Transport Operation
 * @param aResponse The network response data
 * @param aResult [out] An output parameter to the plugin manager 
 * containing the data parsed by the plugins
 * @param aRetType [out] The Plugin return value
 * @param aPageResult [out] The page information filled by the plugins
 * @return SmfPluginManagerResult, The result of the operation
 * @see smfglobal.h
 */	
SmfPluginManagerResult SmfPluginManagerUtil::responseAvailable ( 
		SmfPluginBase *aInstance,
		const SmfRequestTypeID &aOperation,
		const SmfTransportResult &aTransportResult, 
		QByteArray *aResponse,
		QVariant *aResult,
		SmfPluginRetType &aRetType,
		SmfResultPage &aPageResult )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::responseAvailable");
	
	SmfPluginError pluginRet;
	SmfPluginManagerResult result = SmfPluginUnknownError;

	switch(aOperation)
		{
		case SmfContactGetFriends:
		case SmfContactGetFollowers:
		case SmfContactSearch:
		case SmfContactSearchNear:
		case SmfContactGetGroups:
		case SmfContactSearchInGroup:
		case SmfContactRetrievePosts:
		case SmfContactPost:
		case SmfContactUpdatePost:
		case SmfContactPostDirected:
		case SmfContactCommentOnAPost:
		case SmfContactPostAppearence:
		case SmfContactSharePost:
		case SmfMusicGetLyrics:
		case SmfMusicGetSubtitle:
		case SmfMusicGetEventsOnLoc:
		case SmfMusicGetVenueOnLoc:
		case SmfMusicGetEventsOnVenue:
		case SmfMusicPostEvents:
		case SmfMusicGetRecommendations:
		case SmfMusicGetTracks:
		case SmfMusicGetTrackInfo:
		case SmfMusicGetStores:
		case SmfMusicPostCurrentPlaying:
		case SmfMusicGetUserInfo:
		case SmfMusicSearchUser:
		case SmfMusicGetPlaylists:
		case SmfMusicGetPlaylistsOfUser:
		case SmfMusicAddToPlaylist:
		case SmfMusicPostCurrentPlayingPlaylist:
		case SmfPictureGetPictures:
		case SmfPictureDescription:
		case SmfPictureUpload:
		case SmfPictureMultiUpload:
		case SmfPicturePostComment:
			{
			pluginRet = aInstance->responseAvailable(aTransportResult, aResponse, 
					aResult, aRetType, aPageResult );
			
			// When plugin returns no error
			if( SmfPluginErrNone == pluginRet )
				{
				// Request is complete, parsed data available with aResult
				if( SmfRequestComplete == aRetType )
					result =  SmfPluginResponseParsed;
				
				// Send the request again
				else if(SmfSendRequestAgain == aRetType )
					result = SmfPluginSendRequestAgain;
				else
					result = SmfPluginResponseParseFailure;
				}
			// Parsing failed
			else
				result =  SmfPluginResponseParseFailure;
			
			break;
			}
			
		default:
			// Unknown service, saved data in Plugin manager is corrupted
			SmfPluginManager::getInstance()->server()->writeLog("No operation type found!!!");
		}
	
	return result;
	}


/**
 * Method to serialize the result of parsing (which is done by the 
 * plugins) to QByteArray to be sent to Smf server.
 * @param aOperation The type of operation to be performed
 * @param aResult The data to be serialized
 * @param aDataStream Stream to be written
 */
void SmfPluginManagerUtil::serializeResult ( 
		const SmfRequestTypeID &aOperation, 
		QVariant* aResult,
		QDataStream &aDataStream )
	{
	SmfPluginManager::getInstance()->server()->writeLog("SmfPluginManagerUtil::serializeResult");
	switch(aOperation)
		{
		// FOR CONTACT - FETCHER
		case SmfContactGetFriends:
		case SmfContactGetFollowers:
		case SmfContactSearch:
		case SmfContactSearchNear:
		case SmfContactSearchInGroup:
			{
			QList<SmfContact> contactList;
			if( aResult->canConvert<SmfContactList>() )
				contactList = aResult->value<SmfContactList>();
			aDataStream<<contactList;
			break;
			}

		case SmfContactGetGroups:
			{
			QList<SmfGroup> groupList;
			if( aResult->canConvert<SmfGroupList>() )
				groupList = aResult->value<SmfGroupList>();
			aDataStream<<groupList;

			break;
			}
			
		// FOR CONTACT - POSTS
		case SmfContactRetrievePosts:
			{
			SmfPluginManager::getInstance()->server()->writeLog("Serialize - retrieveposts() result");
			
			QList<SmfPost> postList;
			if( aResult->canConvert<SmfPostList>() )
				postList = aResult->value<SmfPostList>();
			aDataStream<<postList;
			break;
			}
			
		case SmfContactPost:
		case SmfContactUpdatePost:
		case SmfContactPostDirected:
		case SmfContactCommentOnAPost:
		case SmfContactPostAppearence:
		case SmfContactSharePost:
			{
			bool value;
			if( QVariant::Bool == aResult->type() )
				value = aResult->toBool();
			aDataStream<<value;
			break;
			}
			
		// FOR MUSIC - LYRICS SERVICE
		case SmfMusicGetLyrics:
			{
			QList<SmfLyrics> lyricsList;
			if( aResult->canConvert<SmfLyricsList>() )
				lyricsList = aResult->value<SmfLyricsList>();
			aDataStream<<lyricsList;

			break;
			}

		case SmfMusicGetSubtitle:
			{
			QList<SmfSubtitle> subtitleList;
			if( aResult->canConvert<SmfSubtitleList>() )
				subtitleList = aResult->value<SmfSubtitleList>();
			aDataStream<<subtitleList;

			break;
			}

		// FOR MUSIC - EVENTS
		case SmfMusicGetEventsOnLoc:
		case SmfMusicGetEventsOnVenue:
			{
			QList<SmfEvent> eventList;
			if( aResult->canConvert<SmfEventList>() )
				eventList = aResult->value<SmfEventList>();
			aDataStream<<eventList;

			break;
			}
			
		case SmfMusicGetVenueOnLoc:
			{
			QList<SmfLocation> venueList;
			if( aResult->canConvert<SmfLocationList>() )
				venueList = aResult->value<SmfLocationList>();
			aDataStream<<venueList;
			break;
			}
		
		case SmfMusicPostEvents:
			{
			bool value;
			if( QVariant::Bool == aResult->type() )
				value = aResult->toBool();
			aDataStream<<value;
			break;
			}

		// FOR MUSIC - SEARCH
		case SmfMusicGetRecommendations:
		case SmfMusicGetTracks:
		case SmfMusicGetTrackInfo:
			{
			QList<SmfTrackInfo> trackList;
			if( aResult->canConvert<SmfTrackInfoList>() )
				trackList = aResult->value<SmfTrackInfoList>();
			aDataStream<<trackList;

			break;
			}
			
		case SmfMusicGetStores:
			{
			QList<SmfProvider> storeList;
			if( aResult->canConvert<SmfProviderList>() )
				storeList = aResult->value<SmfProviderList>();
			aDataStream<<storeList;
			break;
			}
			
		case SmfMusicPostCurrentPlaying:
			{
			bool value;
			if( QVariant::Bool == aResult->type() )
				value = aResult->toBool();
			aDataStream<<value;
			break;
			}
		

		// FOR MUSIC - SERVICE
		case SmfMusicGetUserInfo:
			{
			SmfMusicProfile user;
			if(aResult->canConvert<SmfMusicProfile>())
				user = aResult->value<SmfMusicProfile>();
			aDataStream<<user;
			break;
			}
			
		case SmfMusicSearchUser:
			{
			QList<SmfMusicProfile> userList;
			if( aResult->canConvert<SmfMusicProfileList>() )
				userList = aResult->value<SmfMusicProfileList>();
			aDataStream<<userList;
			break;
			}
	 
			// FOR MUSIC - PLAYLIST SERVICE
		case SmfMusicGetPlaylists:
		case SmfMusicGetPlaylistsOfUser:
			{
			QList<SmfPlaylist> playlists;
			if( aResult->canConvert<SmfPlaylistList>() )
				playlists = aResult->value<SmfPlaylistList>();
			aDataStream<<playlists;
			break;
			}
			
		case SmfMusicAddToPlaylist:
		case SmfMusicPostCurrentPlayingPlaylist:
			{
			bool value;
			if( QVariant::Bool == aResult->type() )
				value = aResult->toBool();
			aDataStream<<value;
			break;
			}
				 
			// FOR PICTURES - GALLERY SERVICES
		case SmfPictureGetPictures:
			{
			QList<SmfPicture> picList;
			if( aResult->canConvert<SmfPictureList>() )
				picList = aResult->value<SmfPictureList>();
			aDataStream<<picList;
			break;
			}
			
		case SmfPictureDescription:
			{
			QString str;
			if( QVariant::String == aResult->type() )
				str = aResult->toString();
			aDataStream<<str;
			break;
			}
			
		case SmfPictureUpload:
		case SmfPictureMultiUpload:
		case SmfPicturePostComment:
			{
			bool value;
			if( QVariant::Bool == aResult->type() )
				value = aResult->toBool();
			aDataStream<<value;
			break;
			}
			
		default:
			// Unknown service, saved data in Plugin manager is corrupted
			SmfPluginManager::getInstance()->server()->writeLog("No operation type found!!!");
		}
	}

