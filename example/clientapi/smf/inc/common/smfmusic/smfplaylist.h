/**
 * @file	smfplaylist.h
 * @author  Nalina Hariharan,Manasij Roy Sasken Communication Technologies Ltd - Initial contribution
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2010 Sasken Communication Technologies Ltd. 
 * All rights reserved.
 * This component and the accompanying materials are made available 
 * under the terms of the "{License}" 
 * which accompanies  this distribution, and is available 
 * at the URL "{LicenseUrl}".
 * 
 * @section DESCRIPTION
 *
 * The playlist class represents an instance of a playlist
 */

#ifndef SMFPLAYLIST_H_
#define SMFPLAYLIST_H_

#include <smftrackinfo.h>
#include <qdatastream.h>

/**
 * The playlist class represents an instance of a playlist
 */
class SmfPlaylist : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfPlaylist( QObject *aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfPlaylist( );
	
	/**
	 * Method to get the list of tracks in the playlist
	 * @param aList The list of tracks in the playlist
	 */
	void getTrackList( QList<SmfTrackInfo>& aList );
	
	/**
	 * Method to get the playlist title
	 * @param aTitle The title of the playlist
	 */
	void getPlayListTitle( QString &aTitle );
	
	/**
	 * Method to get the creation date of the playlist
	 * @param aDate The date and time of creation of the playlist
	 */
	void getCreationDate( QDateTime &aDate );
	
	/**
	 * Method to get the id of the playlist
	 * @param aId The ID value 
	 */
	void getId( QString &aId );
	
	/**
	 * Method to set the list of tracks in the playlist
	 * @param aList The new list of tracks in the playlist
	 */
	void setTrackList( const QList<SmfTrackInfo> &aList );
	
	/**
	 * Method to set the playlist title
	 * @param aTitle The new title of the playlist
	 */
	void setPlayListTitle( const QString &aTitle );
	
	/**
	 * Method to set the creation date of the playlist
	 * @param aDate The new date and time of creation of the playlist
	 */
	void setCreationDate( const QDateTime &aDate );
	
private:
	QList<SmfTrackInfo> m_trackList;	// list of tracks
	QString m_title;				// playlist name
	QDateTime m_creationDate;		// creation date
	QString m_playlistId;
	
	};
 /**
  * Externalization
  */
 QDataStream &operator<<(QDataStream &, const SmfPlaylist &);
 /**
  * Internalization
  */
 QDataStream &operator>>(QDataStream &, SmfPlaylist &);
 
#endif /* SMFPLAYLIST_H_ */
