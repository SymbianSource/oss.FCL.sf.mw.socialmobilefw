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
 * The SmfAlbum class represents a music album
 *
 */
#ifndef SMFALBUM_H_
#define SMFALBUM_H_

#include <QImage>
#include <qdatastream.h>
#include <QSharedData>
#include <smfclientglobal.h>
#include <QMetaType>

class SmfAlbumPrivate;
class SmfArtists;

/**
 * @ingroup smf_common_group
 * The SmfAlbum class represents a music album
 */
class SMFCOMMON_EXPORT SmfAlbum
	{
public:
	/**
	 * Constructor with default argument
	 */
	SmfAlbum( );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfAlbum( const SmfAlbum &aOther );
	
	/**
	 * Overloaded = operator
	 * @param aOther The reference object
	 * @return The target reference value
	 */
	SmfAlbum& operator=( const SmfAlbum &aOther );
	
	/**
	 * Destructor
	 */
	~SmfAlbum( );
	
	/**
	 * Method to get the album name
	 * @return The album name
	 */
	QString name( ) const;
	
	/**
	 * Method to get the album's image
	 * @return The album's image
	 */
	QImage image( ) const;
	
	/**
	 * Method to get the artist names
	 * @return The list of artists in the album
	 */
	SmfArtists artists( ) const;
	
	/**
	 * Method to get the id of the album
	 * @return The ID value 
	 */
	QString id( ) const;
	
	/**
	 * Method to set the album name
	 * @param aName The album name
	 */
	void setName( const QString &aName );
	
	/**
	 * Method to set the album's image
	 * @param aImage The album's image
	 */
	void setImage( const QImage &aImage );
	
	/**
	 * Method to set the artist names
	 * @param aArtists The list of artists in the album
	 */
	void setArtists( const SmfArtists &aArtists );
	
	/**
	 * Method to set the id of the album
	 * @param aId The ID value 
	 */
	void setId( const QString &aId );
	
private:
	QSharedDataPointer<SmfAlbumPrivate> d;
	
	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfAlbum &aAlbum );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfAlbum &aAlbum );
	
	};


/**
 * Method for Externalization. Writes the SmfAlbum object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aAlbum The SmfAlbum object to be externalized
 * @return reference to the written stream
 */
SMFCOMMON_EXPORT QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfAlbum &aAlbum );

/**
 * Method for Internalization. Reads a SmfAlbum object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aAlbum The SmfAlbum object to be internalized
 * @return reference to the stream
 */
SMFCOMMON_EXPORT QDataStream &operator>>( QDataStream &aDataStream, 
		SmfAlbum &aAlbum);

// Make the class SmfAlbum known to QMetaType, so that as to register it.
Q_DECLARE_METATYPE(SmfAlbum)
Q_DECLARE_METATYPE(QList<SmfAlbum>)

#endif /* SMFALBUM_H_ */
