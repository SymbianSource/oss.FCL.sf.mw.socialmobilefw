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
 * Music related services
 */

#include <smfmusicrating.h>
#include <smfcomment.h>
#include <smfmusicfingerprint.h>

#include "smfmusic.h"
#include "smfmusic_p.h"


//SmfMusicService start
SmfMusicService::SmfMusicService(SmfProvider *baseProvider)
		: m_baseProvider(baseProvider)
	{
	m_private = new SmfMusicServicePrivate(this);
	}

SmfMusicService::~SmfMusicService()
	{
	if(m_private)
		{
		delete m_private;
		m_private = NULL;
		}
	}

void SmfMusicService::userinfo()
	{
	m_private->userinfo();
	}

void SmfMusicService::searchUser(SmfLocation venue, int pageNum, int perPage)
	{
	m_private->searchUser(venue,pageNum,perPage);
	}

void SmfMusicService::customRequest(const int& operationId,QByteArray* customData)
	{
	m_private->customRequest(operationId, customData);
	}

void SmfMusicService::postCurrentPlaying(SmfTrackInfo track)
	{
	m_private->postCurrentPlaying(track);
	}

void SmfMusicService::postRating(SmfTrackInfo track, SmfMusicRating rate)
	{
	m_private->postRating(track,rate);
	}

void SmfMusicService::postComments(SmfTrackInfo track, SmfComment comment)
	{
	m_private->postComments(track,comment);
	}

SmfProvider* SmfMusicService::getProvider() const
	{
	return m_baseProvider;
	}
//SmfMusicService end


//SmfMusicSearch start
SmfMusicSearch::SmfMusicSearch(SmfProvider *baseProvider)
		:m_baseProvider(baseProvider)
	{
	m_private = new SmfMusicSearchPrivate(this);
	}

SmfMusicSearch::~SmfMusicSearch()
	{
	if(m_private)
		{
		delete m_private;
		m_private = NULL;
		}
	}

void SmfMusicSearch::recommendations(SmfTrackInfo track, int pageNum, int perPage)
	{
	m_private->recommendations(track,pageNum,perPage);
	}

void SmfMusicSearch::tracks(SmfTrackInfo track, int pageNum, int perPage)
	{
	m_private->tracks(track,pageNum,perPage);
	}

void SmfMusicSearch::trackInfo(SmfMusicFingerPrint signature, int pageNum, int perPage)
	{
	m_private->trackInfo(signature,pageNum,perPage);
	}

void SmfMusicSearch::stores(SmfTrackInfo track, int pageNum, int perPage)
	{
	m_private->stores(track,pageNum,perPage);
	}

void SmfMusicSearch::customRequest(const int& operationId,QByteArray* customData)
	{
	m_private->customRequest(operationId, customData);
	}

SmfProvider* SmfMusicSearch::getProvider() const
	{
	return m_baseProvider;
	}
//SmfMusicSearch end


//SmfPlaylistService start
SmfPlaylistService::SmfPlaylistService(SmfProvider *baseProvider)
		: m_baseProvider(baseProvider)
	{
	m_private = new SmfPlaylistServicePrivate(this);
	}

SmfPlaylistService::~SmfPlaylistService()
	{
	if(m_private)
		{
		delete m_private;
		m_private = NULL;
		}
	}

void SmfPlaylistService::playlists(int pageNum, int perPage)
	{
	m_private->playlists(pageNum,perPage);
	}

void SmfPlaylistService::playlistsOf(SmfMusicProfile *user, int pageNum, int perPage)
	{
	m_private->playlistsOf(user,pageNum,perPage);
	}

int SmfPlaylistService::addToPlaylist(SmfPlaylist plst, SmfTrackInfoList *tracks)
	{
	return m_private->addToPlaylist(plst,tracks);
	}

int SmfPlaylistService::postCurrentPlayingPlaylist(SmfPlaylist plst)
	{
	return m_private->postCurrentPlayingPlaylist(plst);
	}

void SmfPlaylistService::customRequest(const int& operationId,QByteArray* customData)
	{
	m_private->customRequest(operationId, customData);
	}

SmfProvider* SmfPlaylistService::getProvider() const
	{
	return m_baseProvider;
	}
//SmfPlaylistService end


//SmfMusicEvents start
SmfMusicEvents::SmfMusicEvents(SmfProvider *baseProvider)
		:m_baseProvider(baseProvider)
	{
	m_private = new SmfMusicEventsPrivate(this);
	}

SmfMusicEvents::~SmfMusicEvents()
	{
	if(m_private)
		{
		delete m_private;
		m_private = NULL;
		}
	}

void SmfMusicEvents::events(SmfLocation venue, int pageNum, int perPage)
	{
	m_private->events(venue,pageNum,perPage);
	}

void SmfMusicEvents::venues(SmfLocation location, int pageNum, int perPage)
	{
	m_private->venues(location,pageNum,perPage);
	}

void SmfMusicEvents::postEvents(SmfEventList events)
	{
	m_private->postEvents(events);
	}

void SmfMusicEvents::customRequest(const int& operationId,QByteArray* customData)
	{
	m_private->customRequest(operationId, customData);
	}

SmfProvider* SmfMusicEvents::getProvider() const
	{
	return m_baseProvider;
	}
//SmfMusicEvents end


//SmfLyricsService start
SmfLyricsService::SmfLyricsService(SmfProvider *baseProvider)
		:m_baseProvider(baseProvider)
	{
	m_private = new SmfLyricsServicePrivate(this);
	}

SmfLyricsService::~SmfLyricsService()
	{
	if(m_private)
		{
		delete m_private;
		m_private = NULL;
		}
	}

void SmfLyricsService::lyrics(SmfTrackInfo track, int pageNum, int perPage)
	{
	m_private->lyrics(track,pageNum,perPage);
	}

void SmfLyricsService::subtitles(SmfTrackInfo track, SmfSubtitleSearchFilter filter, int pageNum, int perPage)
	{
	m_private->subtitles(track,filter,pageNum,perPage);
	}

void SmfLyricsService::customRequest(const int& operationId,QByteArray* customData)
	{
	m_private->customRequest(operationId, customData);
	}

SmfProvider* SmfLyricsService::getProvider() const
	{
	return m_baseProvider;
	}
//SmfLyricsService end
