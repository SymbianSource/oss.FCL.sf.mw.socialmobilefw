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

#ifndef SMFMUSICPRIVATE_H_
#define SMFMUSICPRIVATE_H_

#include "smfmusic.h"
#include "smfglobal.h"
#include "smfobserver.h"
#include "smfcomment.h"
#ifdef Q_OS_SYMBIAN
#include "smfclientsymbian.h"
#else
#include "smfclientqt.h"
#endif
class SmfMusicService;
class SmfMusicSearch;
class SmfPlaylistService;
class SmfMusicEvents;
class SmfLyricsService;

class SmfMusicServicePrivate : public smfObserver
	{
	Q_OBJECT
public:
	/**
	 * Constructor
	 */
	SmfMusicServicePrivate(SmfMusicService* aMusicService);
	~SmfMusicServicePrivate();
	/**
	* Gets self profile information asynchronously.
	* userInfoAvailable() signal is emitted with SmfMusicProfile when the info is arrived
	*/
	void userinfo() ;
	
	/**
	* Asynchronously searches information about other service users for a particular venue
	* searchInfoAvailable() signal is emitted with SmfMusicProfileList when the info is arrived.
	* When the list is big user can specify the page number and per page item data.
	* If not supplied by the user default values are used.
	* @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	* @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	*/
	void searchUser(SmfLocation venue,int pageNum,int perPage) ;
	
	/** from smfobserver */
   void resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error);
private:
	  SmfProvider* m_baseProvider;
	  //serialized byte array of provider+other info to be sent to the server
	  //The order:- SmfProvider then params in order of their appearance in fn
	  QByteArray m_serializedDataToServer;
	  //serialized xtra info, order of serialization follows order of param
	  QByteArray m_xtraInfoSerialized;
	#ifdef Q_OS_SYMBIAN
	  CSmfClientSymbian* m_SmfClientPrivate;
	  friend class CSmfClientSymbian;
	  int m_xtraInfoFlag;
	  int m_pageInfoFlag;
	#else
	  SmfClientQt* m_SmfClientPrivate;
	  friend class SmfClientQt;
	#endif
	  bool m_connected;
	  SmfMusicService* m_musicService;
	  SmfMusicProfile* m_profile;
	  SmfMusicProfileList m_profileList;
	};
//SmfMusicSearch
class SmfMusicSearchPrivate : public smfObserver
	{
	Q_OBJECT
public:
	/**
	 * Constructor
	 */
	SmfMusicSearchPrivate(SmfMusicSearch* aMusicSearch);
	~SmfMusicSearchPrivate();
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
	  void recommendations(SmfTrackInfo track,int pageNum,int perPage)  ; // basic list of track objects

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
	  void tracks(SmfTrackInfo track,int pageNum,int perPage)  ; // basic list of track objects

	  /**
	   * Searches for a track having similar finger print asynchronously.
	   * The signal trackSearchAvailable() is emitted with SmfTrackInfoList
	   * once its arrived.
	   * @param signature The search criteria,signature to be searched for
	   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	   */
	  void trackInfo(SmfMusicFingerPrint signature,int pageNum,int perPage)  ; // search by fingerprint object

	  /**
	   * Search information about where to buy this song from asynchronously.
	   * The signal storeSearchAvailable() is emitted with SmfProviderList once its arrived.
	   * @param track The search criteria for stores
	   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	   */
	  void stores(SmfTrackInfo track,int pageNum,int perPage)  ;
		/**
		 * Posts currently playing track.
		 * Success can be checked by checking the signal postFinished()
		 * @param track Track to post
		 */
	  void postCurrentPlaying(SmfTrackInfo track) ;
	  void postRating(SmfTrackInfo track, SmfMusicRating rate) ;
	  void postComments(SmfTrackInfo track, SmfComment comment) ;
		/** from smfobserver */
	   void resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error);
private:
	  SmfProvider* m_baseProvider;
	  //serialized byte array of provider+other info to be sent to the server
	  //The order:- SmfProvider then params in order of their appearance in fn
	  QByteArray m_serializedDataToServer;
	  //serialized xtra info, order of serialization follows order of param
	  QByteArray m_xtraInfoSerialized;
	#ifdef Q_OS_SYMBIAN
	  CSmfClientSymbian* m_SmfClientPrivate;
	  friend class CSmfClientSymbian;
	  int m_xtraInfoFlag;
	  int m_pageInfoFlag;
	#else
	  SmfClientQt* m_SmfClientPrivate;
	  friend class SmfClientQt;
	#endif
	  bool m_connected;
	  SmfMusicSearch* m_musicSearch;
	  SmfMusicProfile* m_profile;
	  SmfProviderList* m_providers;
	  SmfTrackInfoList* m_trackInfoList;
	  SmfMusicProfileList m_profileList;
	};
class SmfPlaylistServicePrivate : public smfObserver
{
  Q_OBJECT

public:
  /**
   * Constructs SmfPlaylistService with base provider info
   * Seeing as this is a plug-in implementation, these will realistically
   * be generated by SMF factory of some kind
   */
  SmfPlaylistServicePrivate(SmfPlaylistService* aPlayLstSrvc);
  ~SmfPlaylistServicePrivate();

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
  void playlists(int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE)  ; // basic list of playlist objects

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
  void playlistsOf(SmfMusicProfile* user,int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE)  ;
  /**
	 * Upload currently playing track to a playlist. Signal
	 * playlistUpdated() can be checked for success value
	 * @param plst The playlist to be added in
	 * @param tracks The list of tracks to uploaded
	 */
  int addToPlaylist(SmfPlaylist plst, SmfTrackInfoList* tracks)  ;

	/**
	 * Upload currently playing playlist . Signal
	 * playlistUpdated() can be checked for success value
	 * @param plst The playlist to be uploaded
	 */
  int postCurrentPlayingPlaylist(SmfPlaylist plst)  ;
  /** from smfobserver */
  void resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error);
private:
	SmfProvider* m_baseProvider;
	//serialized byte array of provider+other info to be sent to the server
	//The order:- SmfProvider then params in order of their appearance in fn
	QByteArray m_serializedDataToServer;
	//serialized xtra info, order of serialization follows order of param
	QByteArray m_xtraInfoSerialized;
	#ifdef Q_OS_SYMBIAN
	CSmfClientSymbian* m_SmfClientPrivate;
	friend class CSmfClientSymbian;
	int m_xtraInfoFlag;
	int m_pageInfoFlag;
	#else
	SmfClientQt* m_SmfClientPrivate;
	friend class SmfClientQt;
	#endif
	bool m_connected;
	SmfPlaylistService* m_playlstSrvc;
	SmfPlaylistList* m_playlistList;
};

class SmfMusicEventsPrivate : public smfObserver
{
  Q_OBJECT

public:
  /**
   * Constructs SmfMusicEvents with base provider info
   * Seeing as this is a plug-in implementation, these will realistically
   *  be generated by SMF factory of some kind
   */
  SmfMusicEventsPrivate(SmfMusicEvents* aMusicEvent);
  ~SmfMusicEventsPrivate();

public:

  /**
   * Gets list of events in a particular location asynchronously.
   * eventsAvailable() signal is emitted with SmfEventsList once its arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  void events(QContactGeoLocation location,int pageNum,int perPage)  ;

  /**
   * Gets list of venues of a particular location asynchronously.
   * venuesAvailable() signal is emitted with SmfLocationList once its arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  void venues(QContactGeoLocation location,int pageNum,int perPage)  ; // basic list of venue objects

  /**
   * Gets list of events in a particular venue asynchronously.
   * eventsAvailable() signal is emitted with SmfEventsList once its arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  void events(SmfLocation venue,int pageNum,int perPage)  ; // basic list of events objects

/**
	 * Updates events. Might not be supported by all service provider.
	 * eventsUpdated() signal can be checked for success value.
	 * @param SmfEventsList List of events to be posted
	 */
	 void postEvents(SmfEventList events);
	  /** from smfobserver */
	  void resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error);
	private:
		SmfProvider* m_baseProvider;
		//serialized byte array of provider+other info to be sent to the server
		//The order:- SmfProvider then params in order of their appearance in fn
		QByteArray m_serializedDataToServer;
		//serialized xtra info, order of serialization follows order of param
		QByteArray m_xtraInfoSerialized;
		#ifdef Q_OS_SYMBIAN
		CSmfClientSymbian* m_SmfClientPrivate;
		friend class CSmfClientSymbian;
		int m_xtraInfoFlag;
		int m_pageInfoFlag;
		#else
		SmfClientQt* m_SmfClientPrivate;
		friend class SmfClientQt;
		#endif
		bool m_connected;
		SmfMusicEvents* m_musicEvent;
		SmfEventList* m_events;
		SmfLocationList* m_venues;
};

/**
* provides service ("org.symbian.smf.client.music.lyrics")
*/
class SmfLyricsServicePrivate : public smfObserver
{
  Q_OBJECT

public:
  /**
   * Constructs SmfLyricsService with base provider info.
   * Seeing as this is a plug-in implementation, these will realistically
   * be generated by SMF factory of some kind
   */

  SmfLyricsServicePrivate(SmfLyricsService* aLyricsSrvc);
  ~SmfLyricsServicePrivate();

public:

  /**
   * Get the lyrics lists asynchrnously, it fetches texts without time info.
   * lyricsAvailable() notification comes SmfLyricsList with when the data is available
   * @param track Track for which lyrics needs to be fetched.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  void lyrics(SmfTrackInfo track,int pageNum,int perPage)  ;

  /**
   * Get the lyrics lists asynchrnously, it fetches texts with time info.
   * Subtitle search filter can be applied
   * subtitleAvailable() notification comes SmfSubtitleList with when the data is available
   * @param track Track for which subtitle needs to be fetched.
   * @param filter Subtitle search filter
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  void subtitles(SmfTrackInfo track, SmfSubtitleSearchFilter filter,int pageNum,int perPage)  ; // texts with time information
  /** from smfobserver */
  void resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error);
private:
	SmfProvider* m_baseProvider;
	//serialized byte array of provider+other info to be sent to the server
	//The order:- SmfProvider then params in order of their appearance in fn
	QByteArray m_serializedDataToServer;
	//serialized xtra info, order of serialization follows order of param
	QByteArray m_xtraInfoSerialized;
	#ifdef Q_OS_SYMBIAN
	CSmfClientSymbian* m_SmfClientPrivate;
	friend class CSmfClientSymbian;
	int m_xtraInfoFlag;
	int m_pageInfoFlag;
	#else
	SmfClientQt* m_SmfClientPrivate;
	friend class SmfClientQt;
	#endif
	bool m_connected;
	SmfLyricsService* m_lyricsSrvc;
	SmfLyricsList* m_lyricsList;
	SmfSubtitleList* m_subList;
};
#endif
