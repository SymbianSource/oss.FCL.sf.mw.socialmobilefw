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
 *
 */

#ifndef SMFMUSIC_H
#define SMFMUSIC_H

#include <qcontactgeolocation.h>
#include <smfglobal.h>
#include <smfevent.h>
#include <smfsubtitle.h>
#include <smfmusicprofile.h>
#include <smflyrics.h>
#include <smfplaylist.h>
#include <smftrackinfo.h>
#include <smflocation.h>
#include <smfprovider.h>

class SmfMusicServicePrivate;
class SmfMusicSearchPrivate;
class SmfLyricsServicePrivate;
class SmfPlaylistServicePrivate;
class SmfMusicEventsPrivate;
class SmfMusicFingerPrint;
class SmfMusicRating;
class SmfComment;

using namespace QtMobility;


/**
 * @ingroup smf_client_group
 * Basic music service ("org.symbian.smf.client.music.service")
 */
class SMFCLIENT_EXPORT SmfMusicService : public QObject
	{
	Q_OBJECT

public:
	/**
	 * Constructs SmfMusicService.
	 * @param baseProvider The base provider info
	 */
	SmfMusicService ( SmfProvider* baseProvider = 0 );
	
	/**
	 * Destructor
	 */
	~SmfMusicService ( );

public slots:
	/**
	 * Gets self profile information asynchronously.
	 * userInfoAvailable() signal is emitted with SmfMusicProfile when the info is arrived
	 */
	void userinfo ( );

	/**
	 * Asynchronously searches information about other service users for a particular venue
	 * searchInfoAvailable() signal is emitted with SmfMusicProfileList when the info is arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void searchUser ( SmfLocation venue, int pageNum = SMF_FIRST_PAGE,
						int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Request for a custom operation. The signal customDataAvailable() is emitted 
	 * when the result is available.
	 * @param operationId OperationId
	 * @param customData Custom data to be sent
	 * Note:-Interpretation of operationId and customData is upto the concerned
	 * plugin and client application. service provider should provide some
	 * serializing-deserializing utilities for these custom data
	 */
	void customRequest ( const int& operationId, QByteArray* customData );
	
public slots:
	/**
	 * Posts currently playing track.
	 * Success can be checked by checking the signal postFinished()
	 * @param track Track to post
	 */
	void postCurrentPlaying ( SmfTrackInfo track);
	
	void postRating ( SmfTrackInfo track, SmfMusicRating rate);
	
	void postComments ( SmfTrackInfo track, SmfComment comment);

signals:
	/**
	 * Notification on arrival of the self profile as result of userinfo().
	 * @param profile The self profile
	 */
	void userInfoAvailable ( SmfMusicProfile* profile, SmfError error );
	
	/**
	 * Notification on arrival of search info
	 * @param profileList List of music profiles
	 */
	void searchInfoAvailable ( SmfMusicProfileList* profileList, 
			SmfError error, SmfResultPage resultPage );
	
	/**
	 * Signals availability of the result of the customRequest
	 * @param operationId The ID to be shared between clients and the plugin
	 * @param customData data
	 */
	void customDataAvailable( const int& operationId, QByteArray* customData );
	
	/**
	 * Signals availability of the result of the posting a track or a comment or a rating
	 * @param error Error if any
	 */
	void postfinished ( SmfError error );
	
private:
	/**
	 * Gets the base provider info
	 */
	SmfProvider* getProvider() const;
	
// Friend Class
	friend class SmfMusicServicePrivate;
	
private:
	SmfProvider* m_baseProvider;
	SmfMusicServicePrivate* m_private;

	};

SMF_SERVICE_NAME(SmfMusicService, "org.symbian.smf.client.music.service\0.2")


/**
 * Provides service ("org.symbian.smf.client.music.search") for music search
 */
class SMFCLIENT_EXPORT SmfMusicSearch : public QObject
	{
	Q_OBJECT
	
public:
	/**
	 * Constructs SmfMusicSearch with base provider as arguement.
	 */
	SmfMusicSearch ( SmfProvider* baseProvider = 0 );
	
	/**
	 * Destructor
	 */
	~SmfMusicSearch();
	
public:
	/**
	 * Searches for music recommendations similar to a particulartrack asynchronously.
	 * The signal trackSearchAvailable() is emitted with SmfTrackInfoList
	 * once its arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param track The track for which similar recommendations need to be fetched.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void recommendations ( SmfTrackInfo track, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Searches for tracks similar to a given track asynchronously.
	 * The signal trackSearchAvailable() is emitted with SmfTrackInfoList
	 * once its arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param track The search criteria, similar tracks are searched
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void tracks ( SmfTrackInfo track, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Searches for a track having similar finger print asynchronously.
	 * The signal trackSearchAvailable() is emitted with SmfTrackInfoList
	 * once its arrived.
	 * @param signature The search criteria,signature to be searched for
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void trackInfo ( SmfMusicFingerPrint signature, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Search information about where to buy this song from asynchronously.
	 * The signal storeSearchAvailable() is emitted with SmfProviderList once its arrived.
	 * @param track The search criteria for stores
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void stores ( SmfTrackInfo track, int pageNum = SMF_FIRST_PAGE, 
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Request for a custom operation. The signal customDataAvailable() is emitted 
	 * when the result is available.
	 * @param operationId OperationId
	 * @param customData Custom data to be sent
	 * Note:-Interpretation of operationId and customData is upto the concerned
	 * plugin and client application. service provider should provide some
	 * serializing-deserializing utilities for these custom data
	 */
	void customRequest ( const int& operationId, QByteArray* customData );
	
signals:
	/**
	 * Emitted when the search result for a track is available.
	 * Note if number of tacks in the search is large, then it can download the list page by page.
	 * In that case this signal is emitted multiple times.
	 * @param resultPage Page number info
	 */
	void trackSearchAvailable ( SmfTrackInfoList* result, 
			SmfError error, SmfResultPage resultPage);
	
	/**
	 *  Emitted when the search result for a store is available.
	 *  Note if number of tacks in the search is large, then it can download the list page by page.
	 *  In that case this signal is emitted multiple times.
	 *  @param resultPage Page number info
	 */
	void storeSearchAvailable ( SmfProviderList* result, 
			SmfError error, SmfResultPage resultPage );
	
	/**
	 * Signals availability of the result of the customRequest
	 * @param operationId The ID to be shared between clients and the plugin
	 * @param customData data
	 */
	void customDataAvailable( const int& operationId, QByteArray* customData );
	
private:
	/**
	 * Gets the base provider info
	 */
	SmfProvider* getProvider() const;
	
private:
	SmfProvider* m_baseProvider;
	SmfMusicSearchPrivate* m_private;
	friend class SmfMusicSearchPrivate;
	
	};

SMF_SERVICE_NAME(SmfMusicSearch, "org.symbian.smf.client.music.search\0.2")


/**
 * Provides service ("org.symbian.smf.client.music.playlist")
 *
 * Interface to a remote playlist service. This class
 * provides some basic functionality to allow applications
 * to interact with playlists in some music related service provider (e.g. last.fm).
 *
 * Note that branding information for the particular service implementation
 * is available from getProvider() API. See also:
 * SmfProvider::serviceName(), SmfProvider::serviceIcon()
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in object.
 *
 */
class SMFCLIENT_EXPORT SmfPlaylistService : public QObject
	{
	Q_OBJECT
	
public:
	/**
	 * Constructs SmfPlaylistService with base provider info
	 * Seeing as this is a plug-in implementation, these will realistically
	 * be generated by SMF factory of some kind
	 */
	SmfPlaylistService ( SmfProvider* baseProvider = 0 );
	
	/**
	 * Destructor
	 */
	~SmfPlaylistService ( );
	
public:
	/**
	 * Gets the list playlists for the logged-in user asynchronously.
	 * The signal playlistsListAvailable() signal is emitted with
	 * SmfPlaylistList once its arrived .
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void playlists ( int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Gets the list playlists for the given user asynchronously.
	 * The signal playlistsListAvailable() signal is emitted with
	 * SmfPlaylistList once its arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param user User for which to get the playlists
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void playlistsOf ( SmfMusicProfile* user, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
public slots:
	/**
	 * Upload currently playing track to a playlist. Signal
	 * playlistUpdated() can be checked for success value
	 * @param plst The playlist to be added in
	 * @param tracks The list of tracks to uploaded
	 */
	int addToPlaylist ( SmfPlaylist plst, SmfTrackInfoList* tracks );
	
	/**
	 * Upload currently playing playlist . Signal
	 * playlistUpdated() can be checked for success value
	 * @param plst The playlist to be uploaded
	 */
	int postCurrentPlayingPlaylist ( SmfPlaylist plst );
	
	/**
	 * Request for a custom operation. The signal customDataAvailable() is emitted 
	 * when the result is available.
	 * @param operationId OperationId
	 * @param customData Custom data to be sent
	 * Note:-Interpretation of operationId and customData is upto the concerned
	 * plugin and client application. service provider should provide some
	 * serializing-deserializing utilities for these custom data
	 */
	void customRequest ( const int& operationId, QByteArray* customData );
		
signals:
	/**
	 * Notification of availability of list of playlists requested.
	 * Note if number of list is large, then it can download the list page by page.
	 * In that case this signal is emitted multiple times.
	 * @param resultPage Page number info
	 */
	void playlistsListAvailable ( SmfPlaylistList*, 
			SmfError error, SmfResultPage resultPage );
	/**
	 * Signals remote updation of playlist with success value
	 */
	void playlistUpdated ( SmfError success );
	
	/**
	 * Signals availability of the result of the customRequest
	 * @param operationId The ID to be shared between clients and the plugin
	 * @param customData data
	 */
	void customDataAvailable( const int& operationId, QByteArray* customData );

private:
	/**
	 * Gets the base provider info
	 */
	SmfProvider* getProvider() const;
	
private:
	SmfProvider* m_baseProvider;
	SmfPlaylistServicePrivate* m_private;
	friend class SmfPlaylistServicePrivate;
	
	};

SMF_SERVICE_NAME(SmfPlaylistService, "org.symbian.smf.client.music.playlist\0.2")


/**
 * provides service ("org.symbian.smf.client.music.events")
 */
class SMFCLIENT_EXPORT SmfMusicEvents : public QObject
	{
	Q_OBJECT
	
public:
	/**
	 * Constructs SmfMusicEvents with base provider info
	 * Seeing as this is a plug-in implementation, these will realistically
	 *  be generated by SMF factory of some kind
	 */
	SmfMusicEvents ( SmfProvider* baseProvider = 0 );
	
	/**
	 * Destructor
	 */
	~SmfMusicEvents ( );
	
public:
	/**
	 * Gets list of events in a particular venue asynchronously.
	 * eventsAvailable() signal is emitted with SmfEventsList once its arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void events ( SmfLocation venue, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Gets list of venues of a particular location asynchronously.
	 * venuesAvailable() signal is emitted with SmfLocationList once its arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void venues ( SmfLocation location, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
public slots:
	/**
	 * Updates events. Might not be supported by all service provider.
	 * eventsUpdated() signal can be checked for success value.
	 * @param SmfEventsList List of events to be posted
	 */
	void postEvents ( SmfEventList events );
	
	/**
	 * Request for a custom operation. The signal customDataAvailable() is emitted 
	 * when the result is available.
	 * @param operationId OperationId
	 * @param customData Custom data to be sent
	 * Note:-Interpretation of operationId and customData is upto the concerned
	 * plugin and client application. service provider should provide some
	 * serializing-deserializing utilities for these custom data
	 */
	void customRequest ( const int& operationId, QByteArray* customData );
	
signals:
	/**
	 * Notification on arrival of event lists
	 * Note if number of list is large, then it can download the list page by page.
	 * In that case this signal is emitted multiple times.
	 * @param resultPage Page number info
	 */
	void eventsAvailable ( SmfEventList* list, 
			SmfError error, SmfResultPage resultPage );
	
	/**
	 * Notification on arrival of venues lists
	 * Note if number of list is large, then it can download the list page by page.
	 * In that case this signal is emitted multiple times.
	 * @param resultPage Page number info
	 */
	void venuesAvailable ( SmfLocationList* list, 
			SmfError error, SmfResultPage resultPage );
	
	/**
	 * Notification of the success of request to post an event
	 */
	void eventsUpdated ( SmfError success );
	
	/**
	 * Signals availability of the result of the customRequest
	 * @param operationId The ID to be shared between clients and the plugin
	 * @param customData data
	 */
	void customDataAvailable( const int& operationId, QByteArray* customData );
	
private:
	/**
	 * Gets the base provider info
	 */
	SmfProvider* getProvider() const;
	
private:
	SmfProvider* m_baseProvider;
	SmfMusicEventsPrivate* m_private;
	friend class SmfMusicEventsPrivate;
	
	};

SMF_SERVICE_NAME(SmfMusicEvents, "org.symbian.smf.client.music.events\0.2")


/**
 * provides service ("org.symbian.smf.client.music.lyrics")
 */
class SMFCLIENT_EXPORT SmfLyricsService : public QObject
	{
	Q_OBJECT
	
public:
	/**
	 * Constructs SmfLyricsService with base provider info.
	 */
	SmfLyricsService ( SmfProvider* baseProvider = 0 );
	
	/**
	 * Destructor
	 */
	~SmfLyricsService ( );
	
public:
	/**
	 * Get the lyrics lists asynchrnously, it fetches texts without time info.
	 * lyricsAvailable() notification comes SmfLyricsList with when the data is available
	 * @param track Track for which lyrics needs to be fetched.
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void lyrics ( SmfTrackInfo track, int pageNum = SMF_FIRST_PAGE,
			int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Get the lyrics lists asynchrnously, it fetches texts with time info.
	 * Subtitle search filter can be applied
	 * subtitleAvailable() notification comes SmfSubtitleList with when the data is available
	 * @param track Track for which subtitle needs to be fetched.
	 * @param filter Subtitle search filter
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void subtitles ( SmfTrackInfo track, SmfSubtitleSearchFilter filter,
			int pageNum = SMF_FIRST_PAGE, int perPage = SMF_ITEMS_PER_PAGE);
	
	/**
	 * Request for a custom operation. The signal customDataAvailable() is emitted 
	 * when the result is available.
	 * @param operationId OperationId
	 * @param customData Custom data to be sent
	 * Note:-Interpretation of operationId and customData is upto the concerned
	 * plugin and client application. service provider should provide some
	 * serializing-deserializing utilities for these custom data
	 */
	void customRequest ( const int& operationId, QByteArray* customData );

signals:
	/**
	 * Notification on arrival of lyrics
	 * Note if the list is large, then it can download the list page by page.
	 * In that case this signal is emitted multiple times.
	 * @param resultPage Page number info
	 */
	void lyricsAvailable ( SmfLyricsList* list, 
			SmfError error, SmfResultPage resultPage );
	
	/**
	 * Notification on arrival of subtitle based on filter.
	 * Note if the list is large, then it can download the list page by page.
	 * In that case this signal is emitted multiple times.
	 * @param resultPage Page number info
	 */
	void subtitleAvailable ( SmfSubtitleList* list, 
			SmfError error, SmfResultPage resultPage );
	
	/**
	 * Signals availability of the result of the customRequest
	 * @param operationId The ID to be shared between clients and the plugin
	 * @param customData data
	 */
	void customDataAvailable( const int& operationId, QByteArray* customData );
	
private:
	/**
	 * Gets the base provider info
	 */
	SmfProvider* getProvider() const;
	
private:
	SmfProvider* m_baseProvider;
	SmfLyricsServicePrivate* m_private;
	friend class SmfLyricsServicePrivate;
	
	};

SMF_SERVICE_NAME(SmfLyricsService, "org.symbian.smf.client.music.lyrics\0.2")

#endif // SMFMUSIC_H
