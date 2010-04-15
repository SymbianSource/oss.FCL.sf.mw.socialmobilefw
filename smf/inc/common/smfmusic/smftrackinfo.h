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
 * The track info class represents information about a music track
 *
 */

#ifndef SMFTRACKINFO_H_
#define SMFTRACKINFO_H_

#include <QTime>
#include <qdatastream.h>
#include <smfmusicrating.h>
#include <smfartists.h>
#include <smfalbum.h>
#include <QStringList>
#include <QSharedData>
#include <smfclientglobal.h>

class SmfTrackInfoPrivate;

/**
 * @ingroup smf_common_group
 * The track info class represents information about a music track
 */
class SMFCLIENT_EXPORT SmfTrackInfo : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 * (parent should be of type SmfMusicServicePlugin)
	 */
	SmfTrackInfo(QObject *aParent = 0 );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfTrackInfo( const SmfTrackInfo &aOther );
	
	/**
	 * Destructor
	 */
	~SmfTrackInfo( );
	
	/**
	 * Method to get the id of the track
	 * @return The ID value 
	 */
	QString id( ) const;
	
	/**
	 * Method to get the track title
	 * @return The title of the track
	 */
	QString title( ) const;
	
	/**
	 * Method to get the track album
	 * @return The album of the track
	 */
	SmfAlbum album( ) const;
	
	/**
	 * Method to get the track's artists
	 * @return The artists of the track
	 */
	SmfArtists artists( ) const;
	
	/**
	 * Method to get the genre of the track
	 * @return The genre of the track
	 */
	QString genre( ) const;
	
	/**
	 * Method to get the track's tag
	 * @return The tag of the track
	 */
	QStringList tags( ) const;
	
	/**
	 * Method to get the release year of the track
	 * @return The release year of the track
	 */
	QDateTime year( ) const;
	
	/**
	 * Method to get the track's rating
	 * @return The rating of the track
	 */
	SmfMusicRating rating( ) const;
	
	/**
	 * Method to get the comments on the track
	 * @return The comments on the track
	 */
	QStringList comments( ) const;
	
	/**
	 * Method to get the track's duration
	 * @return The duration of the track
	 */
	QTime duration( ) const;
	
	/**
	 * Method to set the track title
	 * @param aTitle The new title of the track
	 */
	void setTitle( const QString &aTitle );
	
	/**
	 * Method to set the track album
	 * @param aAlbum The new album of the track
	 */
	void setAlbum( const SmfAlbum &aAlbum );
	
	/**
	 * Method to set the track's artists
	 * @param aArtists The new artists of the track
	 */
	void setArtists( const SmfArtists &aArtists );
	
	/**
	 * Method to set the genre of the track
	 * @param aGenre The new genre of the track
	 */
	void setGenre( const QString &aGenre );
	
	/**
	 * Method to set the track's tag
	 * @param aTag The tag of the track
	 */
	void setTags( const QStringList &aTags );
	
	/**
	 * Method to set the release year of the track
	 * @param aYear The new release year of the track
	 */
	void setYear( const QDateTime &aYear );
	
	/**
	 * Method to set the track's rating
	 * @param aRating The new rating of the track
	 */
	void setRating( const SmfMusicRating &aRating );
	
	/**
	 * Method to set the comments on the track
	 * @param aComment The new comment of the track
	 */
	void setComment( const QStringList &aComments );
	
	/**
	 * Method to set the duration of the track
	 * @param aDuration The new duration of the track
	 */
	void setDuration( const QTime &aDuration );

private:
	QSharedDataPointer<SmfTrackInfoPrivate> d;
	
	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfTrackInfo &aTrackInfo );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfTrackInfo &aTrackInfo );
	
	};


/**
 * Method for Externalization. Writes the SmfTrackInfo object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aTrackInfo The SmfTrackInfo object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfTrackInfo &aTrackInfo );

/**
 * Method for Internalization. Reads a SmfTrackInfo object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aTrackInfo The SmfTrackInfo object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfTrackInfo &aTrackInfo);

// Make the class SmfTrackInfo known to QMetaType, so that as to register it.
Q_DECLARE_METATYPE(SmfTrackInfo)

#endif /* SMFTRACKINFO_H_ */
