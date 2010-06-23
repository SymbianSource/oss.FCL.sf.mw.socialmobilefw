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
 * 
 */
#include "smfmusic_p.h"


SmfMusicServicePrivate::SmfMusicServicePrivate(SmfMusicService* aMusicService)
:m_musicService(aMusicService)
	{
	//private impl for symbian
	#ifdef Q_OS_SYMBIAN
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
	#else
	//TODO:- Use private impl for other platforms or else Qt impl
	#endif
	}

void SmfMusicServicePrivate::searchUser(SmfLocation venue, int pageNum, int perPage)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicService->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfLocation
	  Xtrawrite<<venue;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicServiceInterface);
	  int maxalloc = SmfMusicProfileMaxSize*perPage;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicSearchUser,maxalloc,m_xtraInfoSerialized);
	}
void SmfMusicServicePrivate::userinfo()
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicService->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  m_xtraInfoSerialized.clear();
	  m_xtraInfoFlag = 0;
	  write<<m_xtraInfoFlag;

	  QString intfName(musicServiceInterface);
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetUserInfo,maxalloc,m_xtraInfoSerialized);
	}
void SmfMusicServicePrivate::resultsAvailable(QByteArray result, SmfRequestTypeID opcode, SmfError error)
	{
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//Order of serialization Error value followed by data
	
	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfMusicGetUserInfo:
				{
					if(m_profile)
						{
						delete m_profile;
						m_profile = NULL;
						}
		
					m_profile = new SmfMusicProfile;
					//TODO:- serialization/deserialization of SmfMusicProfile
					reader>>*(m_profile);
					//not incorporating paging now
					SmfResultPage page;
		
					emit m_musicService->userInfoAvailable(m_profile,error);
				
				}
				break;
		case SmfMusicSearchUser:
				{
					m_profileList.clear();
					//TODO:- serialization/deserialization of SmfMusicProfile
					reader>>m_profileList;
					//not incorporating paging now
					SmfResultPage page;
		
					emit m_musicService->searchInfoAvailable(m_profileList,error,page);
				
				}
				break;
		}
	}

SmfMusicServicePrivate::~SmfMusicServicePrivate()
	{
		if(m_SmfClientPrivate)
		{
			delete m_SmfClientPrivate;
			m_SmfClientPrivate = NULL;
		}
	}

SmfMusicSearchPrivate::SmfMusicSearchPrivate(SmfMusicSearch *aMusicSearch)
: m_musicSearch(aMusicSearch)
	{
		//private impl for symbian
		#ifdef Q_OS_SYMBIAN
		m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
		#else
		//TODO:- Use private impl for other platforms or else Qt impl
		#endif
	}

SmfMusicSearchPrivate::~SmfMusicSearchPrivate()
	{
		if(m_SmfClientPrivate)
		{
			delete m_SmfClientPrivate;
			m_SmfClientPrivate = NULL;
		}
	}

void SmfMusicSearchPrivate::stores(SmfTrackInfo track, int pageNum, int perPage)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicSearch->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<track;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicSearchInterface);
	  /** @TODO:-revisit all the maxalloc */
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetStores,maxalloc,m_xtraInfoSerialized);
	}
void SmfMusicSearchPrivate::recommendations(SmfTrackInfo track, int pageNum, int perPage)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicSearch->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<track;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicSearchInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetRecommendations,maxalloc,m_xtraInfoSerialized);
	}

void SmfMusicSearchPrivate::tracks(SmfTrackInfo track, int pageNum, int perPage)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicSearch->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<track;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicSearchInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetTracks,maxalloc,m_xtraInfoSerialized);
	}

void SmfMusicSearchPrivate::trackInfo(SmfMusicFingerPrint signature, int pageNum, int perPage)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicSearch->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<signature;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicSearchInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetTrackInfo,maxalloc,m_xtraInfoSerialized);
	}
void SmfMusicSearchPrivate::postCurrentPlaying(SmfTrackInfo track) 
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicSearch->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;

	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<track;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicSearchInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicPostCurrentPlaying,maxalloc,m_xtraInfoSerialized);
	}
void SmfMusicSearchPrivate::postRating(SmfTrackInfo track, SmfMusicRating rate) 
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicSearch->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;

	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<track;
	  Xtrawrite<<rate;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicSearchInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicPostRating,maxalloc,m_xtraInfoSerialized);
	}
void SmfMusicSearchPrivate::postComments(SmfTrackInfo track, SmfComment comment) 
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicSearch->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;

	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<track;
	  Xtrawrite<<comment;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicSearchInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicPostComment,maxalloc,m_xtraInfoSerialized);
	}
void SmfMusicSearchPrivate::resultsAvailable(QByteArray result, SmfRequestTypeID opcode, SmfError error)
	{
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//Order of serialization Error value followed by data
	
	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfMusicGetStores:
				{
					if(m_providers)
						{
						delete m_providers;
						m_providers = NULL;
						}
		
					m_providers = new SmfProviderList;
					//TODO:- serialization/deserialization of SmfMusicProfile
					reader>>*(m_providers);
					//not incorporating paging now
					SmfResultPage page;
		
					emit m_musicSearch->storeSearchAvailable(m_providers,error,page);
				
				}
				break;
		case SmfMusicGetRecommendations:
		case SmfMusicGetTracks:
		case SmfMusicGetTrackInfo:
				{

				if(m_trackInfoList)
					{
					delete m_trackInfoList;
					m_trackInfoList = NULL;
					}
	
				m_trackInfoList = new SmfTrackInfoList;
				//TODO:- serialization/deserialization of SmfMusicProfile
				reader>>*(m_trackInfoList);
				//not incorporating paging now
				SmfResultPage page;
	
				emit m_musicSearch->trackSearchAvailable(m_trackInfoList,error,page);
			
				//trackSearchAvailable(SmfTrackInfoList* result
				}
				break;
		case SmfMusicPostCurrentPlaying:
		case SmfMusicPostRating:
		case SmfMusicPostComment:
			{
			emit m_musicSearch->postfinished(error);
			}
			break;
		}
	}

SmfPlaylistServicePrivate::SmfPlaylistServicePrivate(SmfPlaylistService *aPlayLstSrvc)
: m_playlstSrvc(aPlayLstSrvc)
	{
	//private impl for symbian
	#ifdef Q_OS_SYMBIAN
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
	#else
	//TODO:- Use private impl for other platforms or else Qt impl
	#endif
	}
SmfPlaylistServicePrivate::~SmfPlaylistServicePrivate()
	{
	
	}
int SmfPlaylistServicePrivate::postCurrentPlayingPlaylist(SmfPlaylist plst)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_playlstSrvc->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<plst;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(playlistServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicPostCurrentPlayingPlaylist,maxalloc,m_xtraInfoSerialized);
	}

void SmfPlaylistServicePrivate::playlistsOf(SmfMusicProfile *user, int pageNum, int perPage)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_playlstSrvc->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<*(user);
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(playlistServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetPlaylistsOfUser,maxalloc,m_xtraInfoSerialized);
	}


void SmfPlaylistServicePrivate::playlists(int pageNum, int perPage)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_playlstSrvc->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  m_xtraInfoFlag = 0;
	  write<<m_xtraInfoFlag;

	  QString intfName(playlistServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetPlaylists,maxalloc);
	}



int SmfPlaylistServicePrivate::addToPlaylist(SmfPlaylist plst, SmfTrackInfoList *tracks)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_playlstSrvc->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;

	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<plst;
	  Xtrawrite<<*(tracks);
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(playlistServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicAddToPlaylist,maxalloc,m_xtraInfoSerialized);
	}
void SmfPlaylistServicePrivate::resultsAvailable(QByteArray result, SmfRequestTypeID opcode, SmfError error)
	{
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//Order of serialization Error value followed by data
	
	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfMusicPostCurrentPlayingPlaylist:
		case SmfMusicAddToPlaylist:
				{
				emit m_playlstSrvc->playlistUpdated(error);				
				}
				break;
		case SmfMusicGetPlaylistsOfUser:
		case SmfMusicGetPlaylists:
			{
			if(m_playlistList)
				{
				delete m_playlistList;
				m_playlistList = NULL;
				}

			m_playlistList = new SmfPlaylistList;
			//TODO:- serialization/deserialization
			reader>>*(m_playlistList);
			SmfResultPage page;
			emit m_playlstSrvc->playlistsListAvailable(m_playlistList,error,page);
			}
			break;
		default:
			Q_ASSERT_X(1,"SmfPlaylistServicePrivate::resultsAvailable","default");
		}
	}

SmfMusicEventsPrivate::SmfMusicEventsPrivate(SmfMusicEvents *aMusicEvent)
:m_musicEvent(aMusicEvent)
	{
	//private impl for symbian
	#ifdef Q_OS_SYMBIAN
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
	#else
	//TODO:- Use private impl for other platforms or else Qt impl
	#endif
	}

SmfMusicEventsPrivate::~SmfMusicEventsPrivate()
	{
	
	}

void SmfMusicEventsPrivate::events(SmfLocation venue, int pageNum, int perPage)
	{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicEvent->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<venue;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicEventServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetEventsOnVenue,maxalloc,m_xtraInfoSerialized);
	}

void SmfMusicEventsPrivate::events(QContactGeoLocation location, int pageNum, int perPage)
{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicEvent->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  QString latLong;
	  latLong += QString::number(location.latitude());
	  latLong += ",";
	  latLong += QString::number(location.longitude());
	  Xtrawrite<<latLong;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicEventServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetEventsOnLoc,maxalloc,m_xtraInfoSerialized);
	}



void SmfMusicEventsPrivate::venues(QContactGeoLocation location, int pageNum, int perPage)
{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicEvent->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  QString latLong;
	  latLong += QString::number(location.latitude());
	  latLong += ",";
	  latLong += QString::number(location.longitude());
	  Xtrawrite<<latLong;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicEventServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetVenueOnLoc,maxalloc,m_xtraInfoSerialized);
	}

void SmfMusicEventsPrivate::postEvents(SmfEventList events)
{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_musicEvent->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;

	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<events;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(musicEventServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicPostEvents,maxalloc,m_xtraInfoSerialized);
	}
void SmfMusicEventsPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error)
	{
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//Order of serialization Error value followed by data
	
	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfMusicGetEventsOnVenue:
			{
			if(m_events)
				{
				delete m_events;
				m_events = NULL;
				}
			m_events = new SmfEventList;
			reader>>*(m_events);
			SmfResultPage page;
			m_musicEvent->eventsAvailable(m_events,error,page);
			}
			break;
		case SmfMusicGetVenueOnLoc:
			{
			if(m_venues)
				{
				delete m_venues;
				m_venues = NULL;
				}
			m_venues = new SmfLocationList;
			reader>>*(m_venues);
			SmfResultPage page;
			m_musicEvent->venuesAvailable(m_venues,error,page);
			}
			break;
		case SmfMusicPostEvents:
			{
			m_musicEvent->eventsUpdated(error);
			}
			break;
		case SmfMusicGetEventsOnLoc:
			{
			if(m_venues)
				{
				delete m_venues;
				m_venues = NULL;
				}
			m_venues = new SmfLocationList;
			reader>>*(m_venues);
			SmfResultPage page;
			m_musicEvent->venuesAvailable(m_venues,error,page);
			}
			break;
		default:
			Q_ASSERT_X(1,"SmfMusicEventsPrivate::resultsAvailable","default");
		}
	}
SmfLyricsServicePrivate::SmfLyricsServicePrivate(SmfLyricsService *aLyricsSrvc)
: m_lyricsSrvc(aLyricsSrvc)
{
	//private impl for symbian
	#ifdef Q_OS_SYMBIAN
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
	#else
	//TODO:- Use private impl for other platforms or else Qt impl
	#endif
}

SmfLyricsServicePrivate::~SmfLyricsServicePrivate()
	{
	
	}



void SmfLyricsServicePrivate::subtitles(SmfTrackInfo track, SmfSubtitleSearchFilter filter, int pageNum, int perPage)
{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_lyricsSrvc->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<track;
	  Xtrawrite<<filter;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(lyricsServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetSubtitle,maxalloc,m_xtraInfoSerialized);
	}

void SmfLyricsServicePrivate::lyrics(SmfTrackInfo track, int pageNum, int perPage)
{
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_lyricsSrvc->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  //TODO:- provide serialization/deserialization of SmfTrackInfo
	  Xtrawrite<<track;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(lyricsServiceInterface);
	  //TODO:-revisit all the maxalloc
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfMusicGetLyrics,maxalloc,m_xtraInfoSerialized);
	}
void SmfLyricsServicePrivate::resultsAvailable(QByteArray result, SmfRequestTypeID opcode, SmfError error)
	{
	//note:- "result" is serialized and we need to de-serialize it as per opcode

	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfMusicGetLyrics:
			{
			if(m_lyricsList)
				{
				delete m_lyricsList;
				m_lyricsList = NULL;
				}
			m_lyricsList = new SmfLyricsList;
			reader>>*(m_lyricsList);
			SmfResultPage page;
			emit m_lyricsSrvc->lyricsAvailable(m_lyricsList,error,page);
			}
			break;
		case SmfMusicGetSubtitle:
			{
			if(m_subList)
				{
				delete m_subList;
				m_subList = NULL;
				}
			m_subList = new SmfSubtitleList;
			reader>>*(m_subList);
			SmfResultPage page;
			emit m_lyricsSrvc->subtitleAvailable(m_subList,error,page);
			}
			break;
		default:
			Q_ASSERT_X(1,"SmfLyricsServicePrivate::resultsAvailable","default");
		}
	}

