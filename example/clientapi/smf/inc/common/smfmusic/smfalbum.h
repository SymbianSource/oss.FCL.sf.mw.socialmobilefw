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

class SmfAlbumPrivate;
class SmfArtists;

/**
 * @ingroup smf_common_group
 * The SmfAlbum class represents a music album
 */
class SMFCLIENT_EXPORT SmfAlbum : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfAlbum( QObject *aParent = 0 );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfAlbum( const SmfAlbum &aOther );
	
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
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfAlbum &aAlbum );

/**
 * Method for Internalization. Reads a SmfAlbum object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aAlbum The SmfAlbum object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfAlbum &aAlbum);


#endif /* SMFALBUM_H_ */
