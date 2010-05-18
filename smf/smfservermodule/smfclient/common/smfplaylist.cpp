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
 * The playlist class represents an instance of a playlist
 *
 */

#include <smfplaylist.h>
#include <smfplaylist_p.h>

/**
 * Constructor with default argument
 */
SmfPlaylist::SmfPlaylist( )
	{
	d = new SmfPlaylistPrivate;
	}

/**
 * Copy Constructor
 * @param aOther The reference object
 */
SmfPlaylist::SmfPlaylist( const SmfPlaylist &aOther )
	: d( aOther.d )
	{
	}

/**
 * Overloaded = operator
 * @param aOther The reference object
 * @return The target reference value
 */
SmfPlaylist& SmfPlaylist::operator=( const SmfPlaylist &aOther )
	{
	d->m_trackList = aOther.d->m_trackList;
	d->m_title = aOther.d->m_title;
	d->m_creationDate = aOther.d->m_creationDate;
	d->m_playlistId = aOther.d->m_playlistId;
	return *this;
	}

/**
 * Destructor
 */
SmfPlaylist::~SmfPlaylist( )
	{
	}

/**
 * Method to get the list of tracks in the playlist
 * @return The list of tracks in the playlist
 */
QList<SmfTrackInfo> SmfPlaylist::trackList( ) const
	{
	return d->m_trackList;
	}

/**
 * Method to get the playlist title
 * @return The title of the playlist
 */
QString SmfPlaylist::playListTitle( ) const
	{
	return d->m_title;
	}

/**
 * Method to get the creation date of the playlist
 * @return The date and time of creation of the playlist
 */
QDateTime SmfPlaylist::creationDate( ) const
	{
	return d->m_creationDate;
	}
/**
 * Method to get the id of the playlist
 * @return The ID value 
 */
QString SmfPlaylist::id( ) const
	{
	return d->m_playlistId;
	}

/**
 * Method to set the list of tracks in the playlist
 * @param aList The new list of tracks in the playlist
 */
void SmfPlaylist::setTrackList( const QList<SmfTrackInfo> &aList )
	{
	d->m_trackList = aList;
	}

/**
 * Method to set the playlist title
 * @param aTitle The new title of the playlist
 */
void SmfPlaylist::setPlayListTitle( const QString &aTitle )
	{
	d->m_title = aTitle;
	}

/**
 * Method to set the creation date of the playlist
 * @param aDate The new date and time of creation of the playlist
 */
void SmfPlaylist::setCreationDate( const QDateTime &aDate )
	{
	d->m_creationDate = aDate;
	}

/**
 * Method to set the id of the playlist
 * @param aId The ID value 
 */
void SmfPlaylist::setId( const QString &aId)
	{
	d->m_playlistId = aId;
	}


/**
 * Method for Externalization. Writes the SmfPlaylist object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aPlaylist The SmfPlaylist object to be externalized
 * @return reference to the written stream
 */
 QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfPlaylist &aPlaylist )
	{
	// Serialize d->m_trackList
	aDataStream<<aPlaylist.d->m_trackList;
	
	// Serialize d->m_title
	aDataStream<<aPlaylist.d->m_title;
	
	// Serialize d->m_creationDate
	aDataStream<<aPlaylist.d->m_creationDate;
	
	// Serialize d->m_playlistId
	aDataStream<<aPlaylist.d->m_playlistId;
	
	return aDataStream;
	}

/**
 * Method for Internalization. Reads a SmfPlaylist object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aPlaylist The SmfPlaylist object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfPlaylist &aPlaylist)
	{
	// Deserialize d->m_trackList
	aDataStream>>aPlaylist.d->m_trackList;
	
	// Deserialize d->m_title
	aDataStream>>aPlaylist.d->m_title;
	
	// Deserialize d->m_creationDate
	aDataStream>>aPlaylist.d->m_creationDate;
	
	// Deserialize d->m_playlistId
	aDataStream>>aPlaylist.d->m_playlistId;
	
	return aDataStream;
	}
