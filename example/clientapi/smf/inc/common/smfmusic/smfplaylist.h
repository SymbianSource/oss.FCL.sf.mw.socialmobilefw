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

#ifndef SMFPLAYLIST_H_
#define SMFPLAYLIST_H_

#include <smftrackinfo.h>
#include <qdatastream.h>
#include <QSharedData>
#include <smfclientglobal.h>

class SmfPlaylistPrivate;

/**
 * @ingroup smf_common_group
 * The playlist class represents an instance of a playlist
 */
class SMFCLIENT_EXPORT SmfPlaylist : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfPlaylist( QObject *aParent = 0 );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfPlaylist( const SmfPlaylist &aOther );
	
	/**
	 * Destructor
	 */
	~SmfPlaylist( );
	
	/**
	 * Method to get the list of tracks in the playlist
	 * @return The list of tracks in the playlist
	 */
	QList<SmfTrackInfo> trackList( ) const;
	
	/**
	 * Method to get the playlist title
	 * @return The title of the playlist
	 */
	QString playListTitle( ) const;
	
	/**
	 * Method to get the creation date of the playlist
	 * @return The date and time of creation of the playlist
	 */
	QDateTime creationDate( ) const;
	
	/**
	 * Method to get the id of the playlist
	 * @return The ID value 
	 */
	QString id( ) const;
	
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
	QSharedDataPointer<SmfPlaylistPrivate> d;
	
	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfPlaylist &aPlaylist );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfPlaylist &aPlaylist );
	
	};


/**
 * Method for Externalization. Writes the SmfPlaylist object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aPlaylist The SmfPlaylist object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfPlaylist &aPlaylist );

/**
 * Method for Internalization. Reads a SmfPlaylist object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aPlaylist The SmfPlaylist object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfPlaylist &aPlaylist);

#endif /* SMFPLAYLIST_H_ */
