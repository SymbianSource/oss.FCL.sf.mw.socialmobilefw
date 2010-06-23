
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
SmfProvider* SmfMusicService::getProvider()
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
SmfProvider* SmfMusicSearch::getProvider()
	{
	return m_baseProvider;
	}
void SmfMusicSearch::postCurrentPlaying(SmfTrackInfo track)
	{
	m_private->postCurrentPlaying(track);
	}
void SmfMusicSearch::postRating(SmfTrackInfo track, SmfMusicRating rate)
	{
	m_private->postRating(track,rate);
	}
void SmfMusicSearch::postComments(SmfTrackInfo track, SmfComment comment)
	{
	m_private->postComments(track,comment);
	}
//SmfMusicSearch end

//SmfPlaylistService start
SmfPlaylistService::SmfPlaylistService(SmfProvider *baseProvider)
: m_baseProvider(baseProvider)
	{
	m_private = new SmfPlaylistServicePrivate(this);
	
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
	m_private->addToPlaylist(plst,tracks);
	}
int SmfPlaylistService::postCurrentPlayingPlaylist(SmfPlaylist plst)
	{
	m_private->postCurrentPlayingPlaylist(plst);
	}
SmfProvider* SmfPlaylistService::getProvider()
	{
	return m_baseProvider;
	}
SmfPlaylistService::~SmfPlaylistService()
	{
	
	}
//SmfPlaylistService end

//SmfMusicEvents start
SmfMusicEvents::SmfMusicEvents(SmfProvider *baseProvider)
:m_baseProvider(baseProvider)
	{
	m_private = new SmfMusicEventsPrivate(this);
	}
void SmfMusicEvents::events(QContactGeoLocation location, int pageNum, int perPage)
	{
	m_private->events(location,pageNum,perPage);
	}
void SmfMusicEvents::events(SmfLocation venue, int pageNum, int perPage)
	{
	m_private->events(venue,pageNum,perPage);
	}
void SmfMusicEvents::venues(QContactGeoLocation location, int pageNum, int perPage)
	{
	m_private->venues(location,pageNum,perPage);
	}
void SmfMusicEvents::postEvents(SmfEventList events)
	{
	m_private->postEvents(events);
	}
SmfProvider* SmfMusicEvents::getProvider()
	{
	return m_baseProvider;
	}
SmfMusicEvents::~SmfMusicEvents()
	{
	
	}

//SmfMusicEvents end


//SmfLyricsService start
SmfLyricsService::SmfLyricsService(SmfProvider *baseProvider)
:m_baseProvider(baseProvider)
	{
	m_private = new SmfLyricsServicePrivate(this);
	}
void SmfLyricsService::lyrics(SmfTrackInfo track, int pageNum, int perPage)
	{
	m_private->lyrics(track,pageNum,perPage);
	}
void SmfLyricsService::subtitles(SmfTrackInfo track, SmfSubtitleSearchFilter filter, int pageNum, int perPage)
	{
	m_private->subtitles(track,filter,pageNum,perPage);
	}
SmfProvider* SmfLyricsService::getProvider()
	{
	return m_baseProvider;
	}

SmfLyricsService::~SmfLyricsService()
	{
	}

//SmfLyricsService end


