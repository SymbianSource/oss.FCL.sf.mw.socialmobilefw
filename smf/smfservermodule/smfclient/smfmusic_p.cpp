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
 * Private implementation of music related services
 * 
 */

#include <qdebug.h>
#include <smfmusicrating.h>
#include <smfcomment.h>
#include <smfmusicfingerprint.h>
#include <smfcontact.h>

#include "smfmusic.h"
#include "smfmusic_p.h"
#ifdef Q_OS_SYMBIAN
	#include "smfclientsymbian.h"
#else
	#include "smfclientqt.h"
#endif


//SmfMusicServicePrivate start
SmfMusicServicePrivate::SmfMusicServicePrivate(SmfMusicService* aMusicService)
		:m_musicService(aMusicService)
	{
#ifdef Q_OS_SYMBIAN
	//private impl for symbian
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
#else
	//TODO:- Use private impl for other platforms or else Qt impl
#endif
	}

SmfMusicServicePrivate::~SmfMusicServicePrivate()
	{
	if(m_SmfClientPrivate)
		{
		delete m_SmfClientPrivate;
		m_SmfClientPrivate = NULL;
		}
	}

void SmfMusicServicePrivate::userinfo()
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicService->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	
	QString intfName(musicServiceInterface);
	int maxalloc = SmfMusicProfileMaxSize;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName, 
			SmfMusicGetUserInfo, maxalloc);
	}

void SmfMusicServicePrivate::searchUser(SmfLocation venue, int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicService->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<venue;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(musicServiceInterface);
	int maxalloc = SmfMusicProfileMaxSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
	SmfMusicSearchUser, maxalloc);
	}

void SmfMusicServicePrivate::customRequest ( const int& operationId, QByteArray* customData )
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicService->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<operationId;
	if(customData)
		{
		write<<m_argFlag;
		write<<*customData;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	
	QString intfName(musicServiceInterface);
	//ToDo:- How much size to allocate for custo data? keeping SmfMusicProfileMaxSize for now
	int maxAlloc = SmfMusicProfileMaxSize;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicServiceCustomRequest, maxAlloc);
	}

void SmfMusicServicePrivate::postCurrentPlaying(SmfTrackInfo track) 
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicService->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<track;
	
	QString intfName(musicServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicPostCurrentPlaying, maxalloc);
	}

void SmfMusicServicePrivate::postRating(SmfTrackInfo track, SmfMusicRating rate) 
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicService->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<track;
	write<<m_argFlag;
	write<<rate;

	QString intfName(musicServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
	SmfMusicPostRating, maxalloc);
	}

void SmfMusicServicePrivate::postComments(SmfTrackInfo track, SmfComment comment) 
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicService->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<track;
	write<<m_argFlag;
	write<<comment;
	
	QString intfName(musicServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicPostComment, maxalloc);
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
			SmfMusicProfile* m_profile = new SmfMusicProfile;
			reader>>*m_profile;

			emit m_musicService->userInfoAvailable(m_profile,error);
			break;
			}
				
		case SmfMusicSearchUser:
			{
			SmfMusicProfileList* m_profileList = new SmfMusicProfileList;
			reader>>*m_profileList;
			
			//ToDo :- not incorporating paging now
			SmfResultPage page;

			emit m_musicService->searchInfoAvailable(m_profileList, error, page);
			break;
			}
			
		case SmfMusicPostCurrentPlaying:
		case SmfMusicPostRating:
		case SmfMusicPostComment:
			{
			emit m_musicService->postfinished(error);
			break;
			}
			
		case SmfMusicServiceCustomRequest:
   			{
   			int operationId;
   			QByteArray *data = new QByteArray;
   			reader>>operationId;
   			reader>>*data;
   			qDebug()<<"operationId = "<<operationId;
   			qDebug()<<"data size = "<<data->size();
   			emit m_musicService->customDataAvailable(operationId, data);
   			break;
   			}
   			
		default:
			User::Panic(_L("Music Service Private = "),opcode);
		}
	}
//SmfMusicServicePrivate start


//SmfMusicSearchPrivate start
SmfMusicSearchPrivate::SmfMusicSearchPrivate(SmfMusicSearch *aMusicSearch)
		: m_musicSearch(aMusicSearch)
	{
#ifdef Q_OS_SYMBIAN
	//private impl for symbian
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

void SmfMusicSearchPrivate::recommendations(SmfTrackInfo track, int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray
	m_serializedDataToServer.clear();
	SmfProvider* m_baseProvider = m_musicSearch->getProvider();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<track;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
		
	QString intfName(musicSearchInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicGetRecommendations, maxalloc);
	}

void SmfMusicSearchPrivate::tracks(SmfTrackInfo track, int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicSearch->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<track;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(musicSearchInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicGetTracks, maxalloc);
	}

void SmfMusicSearchPrivate::trackInfo(SmfMusicFingerPrint signature, int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicSearch->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<signature;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(musicSearchInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName, 
			SmfMusicGetTrackInfo, maxalloc);
	}

void SmfMusicSearchPrivate::stores(SmfTrackInfo track, int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicSearch->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<track;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(musicSearchInterface);
	/** @TODO:-revisit all the maxalloc */
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicGetStores, maxalloc);
	}

void SmfMusicSearchPrivate::customRequest ( const int& operationId, QByteArray* customData )
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicSearch->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<operationId;
	
	if(customData)
		{
		write<<m_argFlag;
		write<<*customData;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	
	QString intfName(musicSearchInterface);
	/** @TODO:-revisit all the maxalloc */
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicSearchCustomRequest, maxalloc);
	}



void SmfMusicSearchPrivate::resultsAvailable(QByteArray result, SmfRequestTypeID opcode, SmfError error)
	{
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//Order of serialization Error value followed by data
	
	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfMusicGetRecommendations:
		case SmfMusicGetTracks:
		case SmfMusicGetTrackInfo:
				{
				SmfTrackInfoList* m_trackInfoList = new SmfTrackInfoList;
				reader>>*(m_trackInfoList);
				
				//ToDo :- not incorporating paging now
				SmfResultPage page;
	
				emit m_musicSearch->trackSearchAvailable(m_trackInfoList,error,page);
				break;
				}
				
		case SmfMusicGetStores:
			{
			SmfProviderList* m_providers = new SmfProviderList;
			reader>>*(m_providers);
			
			//ToDo :- not incorporating paging now
			SmfResultPage page;

			emit m_musicSearch->storeSearchAvailable(m_providers,error,page);
			break;
			}

		case SmfMusicSearchCustomRequest:
   			{
   			int operationId;
   			QByteArray *data = new QByteArray;
   			reader>>operationId;
   			reader>>*data;
   			qDebug()<<"operationId = "<<operationId;
   			qDebug()<<"data size = "<<data->size();
   			emit m_musicSearch->customDataAvailable(operationId, data);
   			break;
   			}

		default:
			User::Panic(_L("Music Search Private = "),opcode);
		}
	}
//SmfMusicSearchPrivate end


//SmfPlaylistServicePrivate start
SmfPlaylistServicePrivate::SmfPlaylistServicePrivate(SmfPlaylistService *aPlayLstSrvc)
		: m_playlstSrvc(aPlayLstSrvc)
	{
#ifdef Q_OS_SYMBIAN
	//private impl for symbian
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
#else
	//TODO:- Use private impl for other platforms or else Qt impl
#endif
	}

SmfPlaylistServicePrivate::~SmfPlaylistServicePrivate()
	{
	if(m_SmfClientPrivate)
		{
		delete m_SmfClientPrivate;
		m_SmfClientPrivate = NULL;
		}	
	}

void SmfPlaylistServicePrivate::playlists(int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_playlstSrvc->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(playlistServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicGetPlaylists, maxalloc);
	}

void SmfPlaylistServicePrivate::playlistsOf(SmfMusicProfile *user, int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_playlstSrvc->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	if(user)
		{
		m_argFlag = 1;
		write<<m_argFlag;
		write<<*user;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	m_argFlag = 1;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
		
	QString intfName(playlistServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicGetPlaylistsOfUser, maxalloc);
	}

int SmfPlaylistServicePrivate::addToPlaylist(SmfPlaylist plst, SmfTrackInfoList *tracks)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_playlstSrvc->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<plst;
	if(tracks)
		{
		write<<m_argFlag;
		write<<*tracks;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
		
	QString intfName(playlistServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	return m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicAddToPlaylist, maxalloc);
	}

int SmfPlaylistServicePrivate::postCurrentPlayingPlaylist(SmfPlaylist plst)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_playlstSrvc->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<plst;
	
	QString intfName(playlistServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	return m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicPostCurrentPlayingPlaylist, maxalloc);
	}

void SmfPlaylistServicePrivate::customRequest ( const int& operationId, QByteArray* customData )
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_playlstSrvc->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<operationId;
	
	if(customData)
		{
		write<<m_argFlag;
		write<<*customData;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	
	QString intfName(playlistServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicPlaylistCustomRequest, maxalloc);
	}

void SmfPlaylistServicePrivate::resultsAvailable(QByteArray result, SmfRequestTypeID opcode, SmfError error)
	{
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//Order of serialization Error value followed by data
	
	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfMusicGetPlaylists:
		case SmfMusicGetPlaylistsOfUser:
			{
			SmfPlaylistList* m_playlistList = new SmfPlaylistList;
			reader>>*(m_playlistList);
			
			//ToDo:-
			SmfResultPage page;
			
			emit m_playlstSrvc->playlistsListAvailable(m_playlistList,error,page);
			break;
			}
			
		case SmfMusicAddToPlaylist:
		case SmfMusicPostCurrentPlayingPlaylist:
			{
			emit m_playlstSrvc->playlistUpdated(error);
			break;
			}
			
		case SmfMusicPlaylistCustomRequest:
   			{
   			int operationId;
   			QByteArray *data = new QByteArray;
   			reader>>operationId;
   			reader>>*data;
   			qDebug()<<"operationId = "<<operationId;
   			qDebug()<<"data size = "<<data->size();
   			emit m_playlstSrvc->customDataAvailable(operationId, data);
   			break;
   			}
		default:
			Q_ASSERT_X(1,"SmfPlaylistServicePrivate::resultsAvailable","unknown opcode");
		}
	}
//SmfPlaylistServicePrivate end


//SmfMusicEventsPrivate start
SmfMusicEventsPrivate::SmfMusicEventsPrivate(SmfMusicEvents *aMusicEvent)
		:m_musicEvent(aMusicEvent)
	{
#ifdef Q_OS_SYMBIAN
	//private impl for symbian
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
#else
	//TODO:- Use private impl for other platforms or else Qt impl
#endif
	}

SmfMusicEventsPrivate::~SmfMusicEventsPrivate()
	{
	if(m_SmfClientPrivate)
		{
		delete m_SmfClientPrivate;
		m_SmfClientPrivate = NULL;
		}
	}

void SmfMusicEventsPrivate::events(SmfLocation venue, int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicEvent->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<venue;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
		
	QString intfName(musicEventServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicGetEventsOnLoc, maxalloc);
	}

void SmfMusicEventsPrivate::venues(SmfLocation location, int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicEvent->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<location;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
		
	QString intfName(musicEventServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicGetVenueOnLoc, maxalloc);
	}

void SmfMusicEventsPrivate::postEvents(SmfEventList events)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicEvent->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<events;
	
	QString intfName(musicEventServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicPostEvents, maxalloc);
	}

void SmfMusicEventsPrivate::customRequest ( const int& operationId, QByteArray* customData )
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_musicEvent->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<operationId;
	
	if(customData)
		{
		write<<m_argFlag;
		write<<*customData;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	
	QString intfName(musicEventServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicEventsCustomRequest, maxalloc);
	}

void SmfMusicEventsPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error)
	{
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//Order of serialization Error value followed by data
	
	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfMusicGetEventsOnLoc:
			{
			SmfEventList* m_events = new SmfEventList;
			reader>>*(m_events);
			
			// ToDo :-
			SmfResultPage page;
			
			m_musicEvent->eventsAvailable(m_events,error,page);
			break;
			}
			
		case SmfMusicGetVenueOnLoc:
			{
			SmfLocationList* m_venues = new SmfLocationList;
			reader>>*(m_venues);
			
			//ToDo :-
			SmfResultPage page;
			
			m_musicEvent->venuesAvailable(m_venues,error,page);
			break;
			}
			
		case SmfMusicPostEvents:
			m_musicEvent->eventsUpdated(error);
			break;
			
		case SmfMusicEventsCustomRequest:
   			{
   			int operationId;
   			QByteArray *data = new QByteArray;
   			reader>>operationId;
   			reader>>*data;
   			qDebug()<<"operationId = "<<operationId;
   			qDebug()<<"data size = "<<data->size();
   			emit m_musicEvent->customDataAvailable(operationId, data);
   			break;
   			}
   			
		default:
			Q_ASSERT_X(1,"SmfMusicEventsPrivate::resultsAvailable","unknown opcode");
		}
	}
//SmfMusicEventsPrivate end


//SmfLyricsServicePrivate start
SmfLyricsServicePrivate::SmfLyricsServicePrivate(SmfLyricsService *aLyricsSrvc)
		: m_lyricsSrvc(aLyricsSrvc)
	{
#ifdef Q_OS_SYMBIAN
	//private impl for symbian
	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
#else
	//TODO:- Use private impl for other platforms or else Qt impl
#endif
	}

SmfLyricsServicePrivate::~SmfLyricsServicePrivate()
	{
	if(m_SmfClientPrivate)
		{
		delete m_SmfClientPrivate;
		m_SmfClientPrivate = NULL;
		}
	}

void SmfLyricsServicePrivate::lyrics(SmfTrackInfo track, int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_lyricsSrvc->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<track;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
		
	QString intfName(lyricsServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicGetLyrics, maxalloc);
	}

void SmfLyricsServicePrivate::subtitles(SmfTrackInfo track, SmfSubtitleSearchFilter filter, 
		int pageNum, int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_lyricsSrvc->getProvider();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*(m_baseProvider);
	m_argFlag = 1;
	write<<m_argFlag;
	write<<track;
	write<<m_argFlag;
	write<<filter;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
		
	QString intfName(lyricsServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicGetSubtitle, maxalloc);
	}

void SmfLyricsServicePrivate::customRequest ( const int& operationId, QByteArray* customData )
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_lyricsSrvc->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<operationId;
	
	if(customData)
		{
		write<<m_argFlag;
		write<<*customData;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	
	QString intfName(lyricsServiceInterface);
	//TODO:-revisit all the maxalloc
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfMusicLyricsCustomRequest, maxalloc);
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
			SmfLyricsList* m_lyricsList = new SmfLyricsList;
			reader>>*(m_lyricsList);
			
			// ToDo :-
			SmfResultPage page;
			
			emit m_lyricsSrvc->lyricsAvailable(m_lyricsList,error,page);
			break;
			}
			
		case SmfMusicGetSubtitle:
			{
			SmfSubtitleList* m_subList = new SmfSubtitleList;
			reader>>*(m_subList);
			
			// ToDo :-
			SmfResultPage page;
			
			emit m_lyricsSrvc->subtitleAvailable(m_subList,error,page);
			break;
			}
			
		case SmfMusicLyricsCustomRequest:
   			{
   			int operationId;
   			QByteArray *data = new QByteArray;
   			reader>>operationId;
   			reader>>*data;
   			qDebug()<<"operationId = "<<operationId;
   			qDebug()<<"data size = "<<data->size();
   			emit m_lyricsSrvc->customDataAvailable(operationId, data);
   			break;
   			}
   			
		default:
			Q_ASSERT_X(1,"SmfLyricsServicePrivate::resultsAvailable","unknown opcode");
		}
	}

//SmfLyricsServicePrivate end
