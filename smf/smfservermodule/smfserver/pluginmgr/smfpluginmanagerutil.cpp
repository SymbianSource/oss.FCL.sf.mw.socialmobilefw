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
#include <smfactivityfetcherplugin.h>
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
SmfPluginManagerUtil::SmfPluginManagerUtil ( QObject *aParent )
	{
	Q_UNUSED(aParent)
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
 * @param aResult [out] SmfError, The result of the operation
 * It can be :-
 * SmfPluginNoError (if plugin has created the request successfully)
 * SmfPluginUnknownPluginService (if plugin service is not known or unsupported)
 * SmfPluginRequestCreationFailed (if request creation has failed)
 */
void SmfPluginManagerUtil::createRequest ( QObject* aInstance, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData,
		SmfError &aResult )
	{
	qDebug()<<"SmfPluginManagerUtil::createRequest()";
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;
	
	// Find the type of service required
	switch(aOperation)
		{
		case SmfActivitySelfActivity:
		case SmfActivityFriendsActivities:
		case SmfActivityFiltered:
		case SmfActivityCustomRequest:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createActivityFetcherRequest(aInstance, aOperation, 
					aInputData, aReqData);
			break;
			
		case SmfContactGetFriends:
		case SmfContactGetFollowers:
		case SmfContactSearch:
		case SmfContactSearchNear:
		case SmfContactGetGroups:
		case SmfContactSearchInGroup:
		case SmfContactCustomRequest:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createContactFetcherRequest(aInstance, aOperation, 
					aInputData, aReqData);
			break;
			
		case SmfContactRetrievePosts:
		case SmfContactPost:
		case SmfContactUpdatePost:
		case SmfContactPostDirected:
		case SmfContactCommentOnAPost:
		case SmfContactPostAppearence:
		case SmfContactSharePost:
		case SmfContactPostCustomRequest:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createContactPostRequest(aInstance, aOperation, 
					aInputData, aReqData);
			break;
			
		case SmfMusicGetLyrics:
		case SmfMusicGetSubtitle:
		case SmfMusicLyricsCustomRequest:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicLyricsRequest(aInstance, aOperation, 
					aInputData, aReqData);
			break;
	
		case SmfMusicGetEventsOnLoc:
		case SmfMusicGetVenueOnLoc:
		case SmfMusicGetEventsOnVenue:
		case SmfMusicPostEvents:
		case SmfMusicEventsCustomRequest:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicEventsRequest(aInstance, aOperation, 
					aInputData, aReqData);
			break;
			
		case SmfMusicGetRecommendations:
		case SmfMusicGetTracks:
		case SmfMusicGetTrackInfo:
		case SmfMusicGetStores:
		case SmfMusicPostCurrentPlaying:
		case SmfMusicSearchCustomRequest:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicSearchRequest(aInstance, aOperation, 
					aInputData, aReqData);
			break;
			
		case SmfMusicGetUserInfo:
		case SmfMusicSearchUser:
		case SmfMusicServiceCustomRequest:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicServiceRequest(aInstance, aOperation, 
					aInputData, aReqData);
			break;
			
		case SmfMusicGetPlaylists:
		case SmfMusicGetPlaylistsOfUser:
		case SmfMusicAddToPlaylist:
		case SmfMusicPostCurrentPlayingPlaylist:
		case SmfMusicPlaylistCustomRequest:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createMusicPlaylistRequest(aInstance, aOperation, 
					aInputData, aReqData);
			break;
			
		case SmfPictureGetPictures:
		case SmfPictureDescription:
		case SmfPictureUpload:
		case SmfPictureMultiUpload:
		case SmfPicturePostComment:
		case SmfPictureCustomRequest:
			// Authorise the plugin and call create request methods
			pluginErrorVal = createGalleryRequest(aInstance, aOperation, 
					aInputData, aReqData);
			break;
			
		default:
			qDebug()<<"Unknown service type!!!";
			aResult = SmfPMPluginUnknownPluginService;
		}
	qDebug()<<"return value = "<<pluginErrorVal;
	
	// Convert the SmfPluginError error type to SmfError type
	convertPluginErrorType(pluginErrorVal, aResult);
	}


/**
 * Method to create a web query to fetch activities
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @return SmfPluginError
 */
SmfPluginError SmfPluginManagerUtil::createActivityFetcherRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData )
	{
	qDebug()<<"SmfPluginManagerUtil::createActivityFetcherRequest()";
	
	// typecast instance to the activity - fetcher type, here SmfActivityFetcherPlugin
	SmfActivityFetcherPlugin *plugin = qobject_cast<SmfActivityFetcherPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;

		switch(aOperation)
			{
			case SmfActivitySelfActivity:
				qDebug()<<"Plugin Operation requested : selfactivities()";
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->selfActivities(aReqData, pageNum, itemPerPage);
				break;
				
			case SmfActivityFriendsActivities:
				{
				qDebug()<<"Plugin Operation requested : friendsActivities()";
				SmfContact contact;
				stream>>contact;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->friendsActivities(aReqData, contact, pageNum, itemPerPage);
				break;
				}
				
			case SmfActivityFiltered:
				{
				qDebug()<<"Plugin Operation requested : filtered()";
				QList<SmfActivityObjectType> list;
				QList<int> intList;
				stream>>intList;
				list.clear();
				foreach(int val, intList)
					{
					SmfActivityObjectType type = (SmfActivityObjectType)val;
					list.append(type);
					}
				
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->filtered(aReqData, list, pageNum, itemPerPage);
				break;
				}
				
			case SmfActivityCustomRequest:
				{
				qDebug()<<"Plugin Operation requested : customRequest()";
				int operationType;
				stream>>operationType;
				QByteArray data;
				stream>>data;
				pluginErrorVal = plugin->customRequest(aReqData, operationType, &data);
				break;
				}
				
			default:
				// do nothing, unknown service
				qDebug()<<"No API found for this operation type!!!";
			}
		}
	else
		{
		qDebug()<<"Plugin instance couldn't be typecasted!!!";
		pluginErrorVal = SmfPluginErrRequestNotCreated; 
		}
	
	return pluginErrorVal;
	}


/**
 * Method to create a web query to fetch contact details.
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @return SmfPluginError, it can be :-
 * SmfPluginErrNone (if request is created successfully) or
 * SmfPluginErrServiceNotSupported (if the service requested is not known or unsupported) or
 * SmfPluginErrInvalidArguments (if the arguments are invalid) or 
 * SmfPluginErrRequestNotCreated (if request could not be created) 
 */
SmfPluginError SmfPluginManagerUtil::createContactFetcherRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData )
	{
	qDebug()<<"SmfPluginManagerUtil::createContactFetcherRequest()";
	
	// typecast instance to the contact - fetcher type, here SmfContactFetcherPlugin
	SmfContactFetcherPlugin *plugin = qobject_cast<SmfContactFetcherPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;

		switch(aOperation)
			{
			case SmfContactGetFriends:
				qDebug()<<"Plugin Operation requested : friends()";
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->friends(aReqData, pageNum, itemPerPage);
				break;
				
			case SmfContactGetFollowers:
				qDebug()<<"Plugin Operation requested : followers()";
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->followers(aReqData, pageNum, itemPerPage);
				break;
				
			case SmfContactSearch:
				{
				qDebug()<<"Plugin Operation requested : search()";
				SmfContact searchContact;
				stream>>searchContact;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->search(aReqData, searchContact, pageNum, itemPerPage);
				break;
				}
				
			case SmfContactSearchNear:
				{
				qDebug()<<"Plugin Operation requested : searchNear()";
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
				{
				qDebug()<<"Plugin Operation requested : groups()";
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->groups(aReqData, pageNum, itemPerPage);
				break;
				}
				
			case SmfContactSearchInGroup:
				{
				qDebug()<<"Plugin Operation requested : searchInGroup()";
				SmfGroup group;
				stream>>group;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->searchInGroup(aReqData, group, pageNum, itemPerPage);
				break;
				}
				
			case SmfContactCustomRequest:
				{
				qDebug()<<"Plugin Operation requested : customRequest()";
				int operationType;
				stream>>operationType;
				QByteArray data;
				stream>>data;
				pluginErrorVal = plugin->customRequest(aReqData, operationType, &data);
				break;
				}
				
			default:
				// do nothing, unknown service
				qDebug()<<"No API found for this operation type!!!";
			}
		}
	else
		{
		qDebug()<<"Plugin instance couldn't be typecasted!!!";
		pluginErrorVal = SmfPluginErrRequestNotCreated; 
		}
	
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
 * @return SmfPluginError, it can be :-
 * SmfPluginErrNone (if request is created successfully) or
 * SmfPluginErrServiceNotSupported (if the service requested is not known or unsupported) or
 * SmfPluginErrInvalidArguments (if the arguments are invalid) or 
 * SmfPluginErrRequestNotCreated (if request could not be created) 
 */
SmfPluginError SmfPluginManagerUtil::createContactPostRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData )
	{
	qDebug()<<"SmfPluginManagerUtil::createContactPostRequest";
	
	// typecast instance to SmfPostProviderPlugin
	SmfPostProviderPlugin *plugin = qobject_cast<SmfPostProviderPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;	
	
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
				qDebug()<<"Plugin Operation requested : retrieve()";
				stream>>contact;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->retrieve(aReqData, &contact, pageNum, itemPerPage);
				break;
				
			case SmfContactPost:
				qDebug()<<"Plugin Operation requested : post()";
				stream>>post;
				stream>>location;
				pluginErrorVal = plugin->post(aReqData, post, location);
				break;
				
			case SmfContactUpdatePost:
				qDebug()<<"Plugin Operation requested : updatePost()";
				stream>>post;
				pluginErrorVal = plugin->updatePost(aReqData, post);
				break;
				
			case SmfContactPostDirected:
				qDebug()<<"Plugin Operation requested : postDirected()";
				stream>>post;
				stream>>contact;
				stream>>location;
				qDebug()<<"Post data = "<<post.description();
				qDebug()<<"Contact ID = "<<contact.value("Guid").value<QContactGuid>().guid();
				pluginErrorVal = plugin->postDirected(aReqData, post, contact, &location);
				break;
				
			case SmfContactCommentOnAPost:
				{
				qDebug()<<"Plugin Operation requested : commentOnAPost()";
				SmfPost post2;
				stream>>post;
				stream>>post2;
				stream>>location;
				pluginErrorVal = plugin->commentOnAPost(aReqData, post, post2, &location);
				break;
				}
				
			case SmfContactPostAppearence:
				{
				qDebug()<<"Plugin Operation requested : postAppearence()";
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
				qDebug()<<"Plugin Operation requested : sharePost()";
				bool edited;
				stream>>post;
				stream>>contact;
				stream>>edited;
				pluginErrorVal = plugin->sharePost(aReqData, post, contact, edited);
				break;
				}
				
			case SmfContactPostCustomRequest:
				{
				qDebug()<<"Plugin Operation requested : customRequest()";
				int operationType;
				stream>>operationType;
				QByteArray data;
				stream>>data;
				pluginErrorVal = plugin->customRequest(aReqData, operationType, &data);
				break;
				}
				
			default:
				// do nothing, unknown service
				qDebug()<<"No API found for this operation type!!!";
			}
		}
	else
		{
		qDebug()<<"Plugin instance couldn't be typecasted!!!";
		pluginErrorVal = SmfPluginErrRequestNotCreated; 
		}
	
	return pluginErrorVal;
	}
	


/**
 * Method to create a web query to do music lyrics or subtitle search
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @return SmfPluginError, it can be :-
 * SmfPluginErrNone (if request is created successfully) or
 * SmfPluginErrServiceNotSupported (if the service requested is not known or unsupported) or
 * SmfPluginErrInvalidArguments (if the arguments are invalid) or 
 * SmfPluginErrRequestNotCreated (if request could not be created) 
 */
SmfPluginError SmfPluginManagerUtil::createMusicLyricsRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData )
	{
	qDebug()<<"SmfPluginManagerUtil::createMusicLyricsRequest";
	
	// typecast instance to SmfLyricsServicePlugin
	SmfLyricsServicePlugin *plugin = qobject_cast<SmfLyricsServicePlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		SmfTrackInfo trackInfo;
		int pageNum, itemPerPage;

		switch(aOperation)
			{
			case SmfMusicGetLyrics:
				qDebug()<<"Plugin Operation requested : lyrics()";
				stream>>trackInfo;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->lyrics(aReqData, trackInfo, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetSubtitle:
				{
				qDebug()<<"Plugin Operation requested : subtitles()";
				stream>>trackInfo;
				int i;
				stream>>i;
				SmfSubtitleSearchFilter filter = (SmfSubtitleSearchFilter)i;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->subtitles(aReqData, trackInfo, filter, pageNum, itemPerPage);
				break;
				}
				
			case SmfMusicLyricsCustomRequest:
				{
				qDebug()<<"Plugin Operation requested : customRequest()";
				int operationType;
				stream>>operationType;
				QByteArray data;
				stream>>data;
				pluginErrorVal = plugin->customRequest(aReqData, operationType, &data);
				break;
				}
				
			default:
				// do nothing, unknown service
				qDebug()<<"No API found for this operation type!!!";
			}
		}
	else
		{
		qDebug()<<"Plugin instance couldn't be typecasted!!!";
		pluginErrorVal = SmfPluginErrRequestNotCreated; 
		}
	
	return pluginErrorVal;
	}


/**
 * Method to create a web query to do music event search or event post operation 
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @return SmfPluginError, it can be :-
 * SmfPluginErrNone (if request is created successfully) or
 * SmfPluginErrServiceNotSupported (if the service requested is not known or unsupported) or
 * SmfPluginErrInvalidArguments (if the arguments are invalid) or 
 * SmfPluginErrRequestNotCreated (if request could not be created) 
 */
SmfPluginError SmfPluginManagerUtil::createMusicEventsRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData )
	{
	qDebug()<<"SmfPluginManagerUtil::createMusicEventsRequest";
	
	// typecast instance to SmfMusicEventsPlugin
	SmfMusicEventsPlugin *plugin = qobject_cast<SmfMusicEventsPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;
		QtMobility::QGeoPositionInfo location;

		switch(aOperation)
			{
			case SmfMusicGetEventsOnLoc:
				qDebug()<<"Plugin Operation requested : events() OnLoc";
				stream>>location;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->events(aReqData, location, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetVenueOnLoc:
				qDebug()<<"Plugin Operation requested : venues()";
				stream>>location;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->venues(aReqData, location, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetEventsOnVenue:
				{
				qDebug()<<"Plugin Operation requested : events() OnVenue";
				SmfLocation venue;
				stream>>venue;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->events(aReqData, venue, pageNum, itemPerPage);
				break;
				}
				
			case SmfMusicPostEvents:
				{
				qDebug()<<"Plugin Operation requested : postEvents()";
				QList<SmfEvent> list;
				stream>>list;
				pluginErrorVal = plugin->postEvents(aReqData, list);
				break;
				}
				
			case SmfMusicEventsCustomRequest:
				{
				qDebug()<<"Plugin Operation requested : customRequest()";
				int operationType;
				stream>>operationType;
				QByteArray data;
				stream>>data;
				pluginErrorVal = plugin->customRequest(aReqData, operationType, &data);
				break;
				}
				
			default:
				// do nothing, unknown service
				qDebug()<<"No API found for this operation type!!!";
			}
		}
	else
		{
		qDebug()<<"Plugin instance couldn't be typecasted!!!";
		pluginErrorVal = SmfPluginErrRequestNotCreated; 
		}
	
	return pluginErrorVal;
	}
	


/**
 * Method to create a web query to do music search operation 
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @return SmfPluginError, it can be :-
 * SmfPluginErrNone (if request is created successfully) or
 * SmfPluginErrServiceNotSupported (if the service requested is not known or unsupported) or
 * SmfPluginErrInvalidArguments (if the arguments are invalid) or 
 * SmfPluginErrRequestNotCreated (if request could not be created) 
 */
SmfPluginError SmfPluginManagerUtil::createMusicSearchRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData )
	{
	qDebug()<<"SmfPluginManagerUtil::createMusicSearchRequest";
	
	// typecast instance to SmfMusicSearchPlugin
	SmfMusicSearchPlugin *plugin = qobject_cast<SmfMusicSearchPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;
		SmfTrackInfo trackInfo;

		switch(aOperation)
			{
			case SmfMusicGetRecommendations:
				qDebug()<<"Plugin Operation requested : recommendations()";
				stream>>trackInfo;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->recommendations(aReqData, trackInfo, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetTracks:
				qDebug()<<"Plugin Operation requested : tracks()";
				stream>>trackInfo;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->tracks(aReqData, trackInfo, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetTrackInfo:
				{
				qDebug()<<"Plugin Operation requested : trackInfo()";
				SmfMusicFingerPrint fp;
				stream>>fp;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->trackInfo(aReqData, fp, pageNum, itemPerPage);
				break;
				}
				
			case SmfMusicGetStores:
				qDebug()<<"Plugin Operation requested : stores()";
				stream>>trackInfo;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->stores(aReqData, trackInfo, pageNum, itemPerPage);
				break;
				
			case SmfMusicPostCurrentPlaying:
				qDebug()<<"Plugin Operation requested : postCurrentPlaying()";
				stream>>trackInfo;
				pluginErrorVal = plugin->postCurrentPlaying(aReqData, trackInfo);
				break;
				
			case SmfMusicPostRating:
				{
				qDebug()<<"Plugin Operation requested : postRating()";
				stream>>trackInfo;
				SmfMusicRating rating;
				stream>>rating;
				pluginErrorVal = plugin->postRating(aReqData, trackInfo, rating);
				break;
				}
				
			case SmfMusicPostComment:
				{
				qDebug()<<"Plugin Operation requested : postComment()";
				stream>>trackInfo;
				SmfComment comment;
				stream>>comment;
				pluginErrorVal = plugin->postComments(aReqData, trackInfo, comment);
				break;
				}
				
			case SmfMusicSearchCustomRequest:
				{
				qDebug()<<"Plugin Operation requested : customRequest()";
				int operationType;
				stream>>operationType;
				QByteArray data;
				stream>>data;
				pluginErrorVal = plugin->customRequest(aReqData, operationType, &data);
				break;
				}
				
			default:
				// do nothing, unknown service
				qDebug()<<"No API found for this operation type!!!";
			}
		}
	else
		{
		qDebug()<<"Plugin instance couldn't be typecasted!!!";
		pluginErrorVal = SmfPluginErrRequestNotCreated; 
		}
	
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
 * @return SmfPluginError, it can be :-
 * SmfPluginErrNone (if request is created successfully) or
 * SmfPluginErrServiceNotSupported (if the service requested is not known or unsupported) or
 * SmfPluginErrInvalidArguments (if the arguments are invalid) or 
 * SmfPluginErrRequestNotCreated (if request could not be created) 
 */
SmfPluginError SmfPluginManagerUtil::createMusicServiceRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData )
	{
	qDebug()<<"SmfPluginManagerUtil::createMusicServiceRequest";
	
	// typecast instance to SmfMusicServicePlugin
	SmfMusicServicePlugin *plugin = qobject_cast<SmfMusicServicePlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;

		switch(aOperation)
			{
			case SmfMusicGetUserInfo:
				qDebug()<<"Plugin Operation requested : userInfo()";
				pluginErrorVal = plugin->userInfo(aReqData);
				break;
				
			case SmfMusicSearchUser:
				{
				qDebug()<<"Plugin Operation requested : serachNear() for Music";
				SmfLocation place;
				stream>>place;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->searchUser(aReqData, place, pageNum, itemPerPage);
				break;
				}
				
			case SmfMusicServiceCustomRequest:
				{
				qDebug()<<"Plugin Operation requested : customRequest()";
				int operationType;
				stream>>operationType;
				QByteArray data;
				stream>>data;
				pluginErrorVal = plugin->customRequest(aReqData, operationType, &data);
				break;
				}
				
			default:
				// do nothing, unknown service
				qDebug()<<"No API found for this operation type!!!";
			}
		}
	else
		{
		qDebug()<<"Plugin instance couldn't be typecasted!!!";
		pluginErrorVal = SmfPluginErrRequestNotCreated; 
		}
	
	return pluginErrorVal;
	}
	


/**
 * Method to create a web query to do music playlist related operation
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @return SmfPluginError, it can be :-
 * SmfPluginErrNone (if request is created successfully) or
 * SmfPluginErrServiceNotSupported (if the service requested is not known or unsupported) or
 * SmfPluginErrInvalidArguments (if the arguments are invalid) or 
 * SmfPluginErrRequestNotCreated (if request could not be created) 
 */
SmfPluginError SmfPluginManagerUtil::createMusicPlaylistRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData )
	{
	qDebug()<<"SmfPluginManagerUtil::createMusicPlaylistRequest";
	
	// typecast instance to SmfPlaylistServicePlugin
	SmfPlaylistServicePlugin *plugin = qobject_cast<SmfPlaylistServicePlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		int pageNum, itemPerPage;
		SmfPlaylist playlist;

		switch(aOperation)
			{
			case SmfMusicGetPlaylists:
				qDebug()<<"Plugin Operation requested : playlists()";
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->playlists(aReqData, pageNum, itemPerPage);
				break;
				
			case SmfMusicGetPlaylistsOfUser:
				{
				qDebug()<<"Plugin Operation requested : playlistsOf()";
				SmfMusicProfile user;
				stream>>user;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->playlistsOf(aReqData, user, pageNum, itemPerPage);
				break;
				}
				
			case SmfMusicAddToPlaylist:
				{
				qDebug()<<"Plugin Operation requested : addToPlaylist()";
				QList<SmfTrackInfo> list;
				stream>>playlist;
				stream>>list;
				pluginErrorVal = plugin->addToPlaylist(aReqData, playlist, list);
				break;
				}
				
			case SmfMusicPostCurrentPlayingPlaylist:
				qDebug()<<"Plugin Operation requested : postCurrentPlayingPlaylist()";
				stream>>playlist;
				pluginErrorVal = plugin->postCurrentPlayingPlaylist(aReqData, playlist);
				break;
				
			case SmfMusicPlaylistCustomRequest:
				{
				qDebug()<<"Plugin Operation requested : customRequest()";
				int operationType;
				stream>>operationType;
				QByteArray data;
				stream>>data;
				pluginErrorVal = plugin->customRequest(aReqData, operationType, &data);
				break;
				}
				
			default:
				// do nothing, unknown service
				qDebug()<<"No API found for this operation type!!!";
			}
		}
	else
		{
		qDebug()<<"Plugin instance couldn't be typecasted!!!";
		pluginErrorVal = SmfPluginErrRequestNotCreated; 
		}
	
	return pluginErrorVal;
	}
	

/**
 * Method to create a web query to do gallery related operation
 * @param aPlugin The instance of the loaded plugin that performs the 
 * contact fetch operation.
 * @param aOperation The type of operation to be performed
 * @param aInputData The data required to create the web query
 * @param aReqData [out] The request data created by the plugin
 * @return SmfPluginError, it can be :-
 * SmfPluginErrNone (if request is created successfully) or
 * SmfPluginErrServiceNotSupported (if the service requested is not known or unsupported) or
 * SmfPluginErrInvalidArguments (if the arguments are invalid) or 
 * SmfPluginErrRequestNotCreated (if request could not be created) 
 */
SmfPluginError SmfPluginManagerUtil::createGalleryRequest ( QObject *aPlugin, 
		const SmfRequestTypeID &aOperation, 
		QByteArray &aInputData,
		SmfPluginRequestData &aReqData )
	{
	qDebug()<<"SmfPluginManagerUtil::createGalleryRequest";
	
	// typecast instance to SmfGalleryPlugin
	SmfGalleryPlugin *plugin = qobject_cast<SmfGalleryPlugin *>(aPlugin);
	SmfPluginError pluginErrorVal = SmfPluginErrServiceNotSupported;
	
	if(plugin)
		{
		QDataStream stream(aInputData);
		SmfPicture picture;
		
		switch(aOperation)
			{
			
			case SmfPictureGetAlbums:
				{
				qDebug()<<"Plugin Operation requested : albums()";
				QStringList names;
				SmfContact contact;
				int pageNum, itemPerPage;
				stream>>names;
				stream>>contact;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->albums(aReqData, names, &contact, pageNum, itemPerPage);
				break;
				}
			
			case SmfPictureGetPictures:
				{
				qDebug()<<"Plugin Operation requested : pictures()";
				int pageNum, itemPerPage;
				SmfPictureAlbumList albumList;
				stream>>albumList;
				stream>>pageNum;
				stream>>itemPerPage;
				pluginErrorVal = plugin->pictures(aReqData, albumList, pageNum, itemPerPage);
				break;
				}
				
			case SmfPictureDescription:
				qDebug()<<"Plugin Operation requested : description()";
				stream>>picture;
				pluginErrorVal = plugin->description(aReqData, picture);
				break;
				
			case SmfPictureUpload:
				{
				qDebug()<<"Plugin Operation requested : upload() single";
				SmfPictureAlbum album;
				stream>>picture;
				stream>>album;
				pluginErrorVal = plugin->upload(aReqData, picture, &album);
				break;
				}
				
			case SmfPictureMultiUpload:
				{
				qDebug()<<"Plugin Operation requested : upload() Multiple";
				QList<SmfPicture> list;
				SmfPictureAlbum album;
				stream>>list;
				stream>>album;
				pluginErrorVal = plugin->upload(aReqData, list, &album);
				break;
				}
				
			case SmfPicturePostComment:
				{
				qDebug()<<"Plugin Operation requested : postComment()";
				SmfComment comment;
				stream>>picture;
				stream>>comment;
				pluginErrorVal = plugin->postComment(aReqData, picture, comment);
				break;
				}
				
			case SmfPictureCustomRequest:
				{
				qDebug()<<"Plugin Operation requested : customRequest()";
				int operationType;
				stream>>operationType;
				QByteArray data;
				stream>>data;
				pluginErrorVal = plugin->customRequest(aReqData, operationType, &data);
				break;
				}
				
			default:
				// do nothing, unknown service
				qDebug()<<"No API found for this operation type!!!";
			}
		}
	else
		{
		qDebug()<<"Plugin instance couldn't be typecasted!!!";
		pluginErrorVal = SmfPluginErrRequestNotCreated; 
		}
	
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
 * @return SmfError, The result of the operation
 */	
SmfError SmfPluginManagerUtil::responseAvailable ( 
		SmfPluginBase *aInstance,
		const SmfRequestTypeID &aOperation,
		const SmfTransportResult &aTransportResult, 
		QByteArray *aResponse,
		QVariant *aResult,
		SmfPluginRetType &aRetType,
		SmfResultPage &aPageResult )
	{
	qDebug()<<"Inside SmfPluginManagerUtil::responseAvailable()";
	
	SmfPluginError pluginRet = SmfPluginErrServiceNotSupported;
	SmfError result = SmfPMPluginUnknownPluginService;

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
			pluginRet = aInstance->responseAvailable(aOperation, aTransportResult, aResponse, 
					aResult, aRetType, aPageResult );
			
			// When plugin returns no error
			if( SmfPluginErrNone == pluginRet )
				{
				// Request is complete, parsed data available with aResult
				if( SmfRequestComplete == aRetType )
					{
					qDebug()<<"Parsing successful";
					result =  SmfNoError;
					}
				
				// Send the request again
				else if(SmfSendRequestAgain == aRetType )
					{
					qDebug()<<"Send request again";
					result = SmfPMPluginSendRequestAgain;
					}
				else
					convertPluginErrorType(pluginRet, result);
				}

			// Plugin returns error
			else
				{
				qDebug()<<"Plugin returned error!!!";
				convertPluginErrorType(pluginRet, result);
				if(SmfPluginErrorNetworkError == result)
					convertNetworkErrorType(aTransportResult, result);
				}
			
			break;
			}
			
		default:
			// Unknown service, saved data in Plugin manager is corrupted
			qDebug()<<"No operation type found!!!";
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
	qDebug()<<"SmfPluginManagerUtil::serializeResult";
	switch(aOperation)
		{
		// FOR CONTACT - FETCHER
		case SmfContactGetFriends:
		case SmfContactGetFollowers:
		case SmfContactSearch:
		case SmfContactSearchNear:
		case SmfContactSearchInGroup:
			{
			qDebug()<<"Serializing to : QList<SmfContact>";
			QList<SmfContact> contactList;
			if( aResult->canConvert<SmfContactList>() )
				contactList = aResult->value<SmfContactList>();
			aDataStream<<contactList;
			break;
			}

		case SmfContactGetGroups:
			{
			qDebug()<<"Serializing to : QList<SmfGroup>";
			QList<SmfGroup> groupList;
			if( aResult->canConvert<SmfGroupList>() )
				groupList = aResult->value<SmfGroupList>();
			aDataStream<<groupList;

			break;
			}
			
		// FOR CONTACT - POSTS
		case SmfContactRetrievePosts:
			{
			qDebug()<<"Serializing to : QList<SmfPost>";
			qDebug()<<"Serialize - retrieveposts() result";
			
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
			qDebug()<<"Serializing to : bool";
			bool value;
			if( QVariant::Bool == aResult->type() )
				value = aResult->toBool();
			aDataStream<<value;
			break;
			}
			
		// FOR MUSIC - LYRICS SERVICE
		case SmfMusicGetLyrics:
			{
			qDebug()<<"Serializing to : QList<SmfLyrics>";
			QList<SmfLyrics> lyricsList;
			if( aResult->canConvert<SmfLyricsList>() )
				lyricsList = aResult->value<SmfLyricsList>();
			aDataStream<<lyricsList;

			break;
			}

		case SmfMusicGetSubtitle:
			{
			qDebug()<<"Serializing to : QList<SmfSubtitle>";
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
			qDebug()<<"Serializing to : QList<SmfEvent>";
			QList<SmfEvent> eventList;
			if( aResult->canConvert<SmfEventList>() )
				eventList = aResult->value<SmfEventList>();
			aDataStream<<eventList;

			break;
			}
			
		case SmfMusicGetVenueOnLoc:
			{
			qDebug()<<"Serializing to : QList<SmfLocation>";
			QList<SmfLocation> venueList;
			if( aResult->canConvert<SmfLocationList>() )
				venueList = aResult->value<SmfLocationList>();
			aDataStream<<venueList;
			break;
			}
		
		case SmfMusicPostEvents:
			{
			qDebug()<<"Serializing to : bool";
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
			qDebug()<<"Serializing to : QList<SmfTrackInfo>";
			QList<SmfTrackInfo> trackList;
			if( aResult->canConvert<SmfTrackInfoList>() )
				trackList = aResult->value<SmfTrackInfoList>();
			aDataStream<<trackList;

			break;
			}
			
		case SmfMusicGetStores:
			{
			qDebug()<<"Serializing to : QList<SmfProvider>";
			QList<SmfProvider> storeList;
			if( aResult->canConvert<SmfProviderList>() )
				storeList = aResult->value<SmfProviderList>();
			aDataStream<<storeList;
			break;
			}
			
		case SmfMusicPostCurrentPlaying:
			{
			qDebug()<<"Serializing to : bool";
			bool value;
			if( QVariant::Bool == aResult->type() )
				value = aResult->toBool();
			aDataStream<<value;
			break;
			}
		

		// FOR MUSIC - SERVICE
		case SmfMusicGetUserInfo:
			{
			qDebug()<<"Serializing to : SmfMusicProfile";
			SmfMusicProfile user;
			if(aResult->canConvert<SmfMusicProfile>())
				user = aResult->value<SmfMusicProfile>();
			aDataStream<<user;
			break;
			}
			
		case SmfMusicSearchUser:
			{
			qDebug()<<"Serializing to : QList<SmfMusicProfile>";
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
			qDebug()<<"Serializing to : QList<SmfPlaylist>";
			QList<SmfPlaylist> playlists;
			if( aResult->canConvert<SmfPlaylistList>() )
				playlists = aResult->value<SmfPlaylistList>();
			aDataStream<<playlists;
			break;
			}
			
		case SmfMusicAddToPlaylist:
		case SmfMusicPostCurrentPlayingPlaylist:
			{
			qDebug()<<"Serializing to : bool";
			bool value;
			if( QVariant::Bool == aResult->type() )
				value = aResult->toBool();
			aDataStream<<value;
			break;
			}
				 
			// FOR PICTURES - GALLERY SERVICES
		case SmfPictureGetPictures:
			{
			qDebug()<<"Serializing to : QList<SmfPicture>";
			QList<SmfPicture> picList;
			if( aResult->canConvert<SmfPictureList>() )
				picList = aResult->value<SmfPictureList>();
			aDataStream<<picList;
			break;
			}
			
		case SmfPictureDescription:
			{
			qDebug()<<"Serializing to : QString";
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
			qDebug()<<"Serializing to : bool";
			bool value;
			if( QVariant::Bool == aResult->type() )
				value = aResult->toBool();
			aDataStream<<value;
			break;
			}
			
		default:
			// Unknown service, saved data in Plugin manager is corrupted
			qDebug()<<"No operation type found!!!";
		}
	}


/**
 * Method to convert SmfPluginError Error to the type SmfError 
 * @param aPluginError The Error code returned by the plugin
 * @param aSMFError [out] The Smf specific common error code
 */
void SmfPluginManagerUtil::convertPluginErrorType( 
		const SmfPluginError &aPluginError,  
		SmfError &aSMFError )
	{
	qDebug()<<"Converting Plugin error code to SmfError";
	switch(aPluginError)
		{
		case SmfPluginErrNone:
			aSMFError = SmfNoError;
			break;
			
		case SmfPluginErrTooManyRequest:
			aSMFError = SmfPluginErrorTooManyRequest;
			break;
			
		case SmfPluginErrRequestQuotaExceeded:
			aSMFError = SmfPluginErrorRequestQuotaExceeded;
			break;
			
		case SmfPluginErrInvalidRequest:
			aSMFError = SmfPluginErrorInvalidRequest;
			break;
			
		case SmfPluginErrUserNotLoggedIn:
			aSMFError = SmfPluginErrorUserNotLoggedIn;
			break;
			
		case SmfPluginErrAuthenticationExpired:
			aSMFError = SmfPluginErrorAuthenticationExpired;
			break;
			
		case SmfPluginErrPermissionDenied:
			aSMFError = SmfPluginErrorPermissionDenied;
			break;
			
		case SmfPluginErrInvalidApplication:
			aSMFError = SmfPluginErrorInvalidApplication;
			break;
			
		case SmfPluginErrServiceUnavailable:
			aSMFError = SmfPluginErrorServiceUnavailable;
			break;
			
		case SmfPluginErrServiceTemporaryUnavailable:
			aSMFError = SmfPluginErrorServiceTemporaryUnavailable;
			break;
			
		case SmfPluginErrFormatNotSupported:
			aSMFError = SmfPluginErrorFormatNotSupported;
			break;
			
		case SmfPluginErrDataSizeExceeded:
			aSMFError = SmfPluginErrorDataSizeExceeded;
			break;
			
		case SmfPluginErrServiceNotSupported:
			aSMFError = SmfPMPluginUnknownPluginService;
			break;
			
		case SmfPluginErrInvalidArguments:
			aSMFError = SmfPluginErrorInvalidArguments;
			break;
			
		case SmfPluginErrRequestNotCreated:
			aSMFError = SmfPMPluginRequestCreationFailed;
			break;
			
		case SmfPluginErrParsingFailed:
			aSMFError = SmfPluginErrorParsingFailed;
			break;
			
		case SmfPluginErrNetworkError:
			aSMFError = SmfPluginErrorNetworkError;
			break;
			
		case SmfPluginErrCancelComplete:
			aSMFError = SmfPluginErrorCancelComplete;
			break;
			
		default:
			aSMFError = SmfUnknownError;
		}
	}

/**
 * Method to convert SmfTransportResult Error to the type SmfError 
 * @param aTMError The Error code returned by the TM
 * @param aSMFError [out] The Smf specific common error code
 */
void SmfPluginManagerUtil::convertNetworkErrorType( 
		const SmfTransportResult &aTMError,  
		SmfError &aSMFError )
	{
	qDebug()<<"Converting Transport error code to SmfError";
	switch(aTMError)
		{
		case SmfTransportOpNoError:
			aSMFError = SmfNoError;
			break;
			
		case SmfTransportOpConnectionRefusedError:
			aSMFError = SmfTMConnectionRefusedError;
			break;
			
		case SmfTransportOpRemoteHostClosedError:
			aSMFError = SmfTMRemoteHostClosedError;
			break;
			
		case SmfTransportOpHostNotFoundError:
			aSMFError = SmfTMHostNotFoundError;
			break;
			
		case SmfTransportOpTimeoutError:
			aSMFError = SmfTMTimeoutError;
			break;
			
		case SmfTransportOpOperationCanceledError:
			aSMFError = SmfTMOperationCanceledError;
			break;
			
		case SmfTransportOpSslHandshakeFailedError:
			aSMFError = SmfTMSslHandshakeFailedError;
			break;
			
		case SmfTransportOpProxyConnectionRefusedError:
			aSMFError = SmfTMProxyConnectionRefusedError;
			break;
			
		case SmfTransportOpProxyConnectionClosedError:
			aSMFError = SmfTMProxyConnectionClosedError;
			break;
			
		case SmfTransportOpProxyNotFoundError:
			aSMFError = SmfTMProxyNotFoundError;
			break;
			
		case SmfTransportOpProxyTimeoutError:
			aSMFError = SmfTMProxyTimeoutError;
			break;
			
		case SmfTransportOpProxyAuthenticationRequiredError:
			aSMFError = SmfTMProxyAuthenticationRequiredError;
			break;
			
		case SmfTransportOpContentAccessDenied:
			aSMFError = SmfTMContentAccessDenied;
			break;
			
		case SmfTransportOpContentOperationNotPermittedError:
			aSMFError = SmfTMContentOperationNotPermittedError;
			break;
			
		case SmfTransportOpContentNotFoundError:
			aSMFError = SmfTMContentNotFoundError;
			break;
			
		case SmfTransportOpAuthenticationRequiredError:
			aSMFError = SmfTMAuthenticationRequiredError;
			break;
			
		case SmfTransportOpContentReSendError:
			aSMFError = SmfTMContentReSendError;
			break;
			
		case SmfTransportOpProtocolUnknownError:
			aSMFError = SmfTMProtocolUnknownError;
			break;
			
		case SmfTransportOpProtocolInvalidOperationError:
			aSMFError = SmfTMProtocolInvalidOperationError;
			break;
			
		case SmfTransportOpUnknownNetworkError:
			aSMFError = SmfTMUnknownNetworkError;
			break;
			
		case SmfTransportOpUnknownProxyError:
			aSMFError = SmfTMUnknownProxyError;
			break;
			
		case SmfTransportOpUnknownContentError:
			aSMFError = SmfTMUnknownContentError;
			break;
			
		case SmfTransportOpProtocolFailure:
			aSMFError = SmfTMProtocolFailure;
			break;
			
		case SmfTransportOpUnknownError:
			aSMFError = SmfTMUnknownError;
			break;
			
		case SmfTransportOpIAPChanged:
			aSMFError = SmfTMIAPChanged;
			break;
			
		case SmfTransportOpCancelled:
			aSMFError = SmfTMCancelled;
			break;
			
		case SmfTransportOpUnsupportedContentEncodingFormat:
			aSMFError = SmfTMUnsupportedContentEncodingFormat;
			break;
			
		default:
			aSMFError = SmfUnknownError;
		}
	}

