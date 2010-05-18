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

#include <smfalbum.h>
#include <smfalbum_p.h>


/**
 * Constructor with default argument
 */
SmfAlbum::SmfAlbum( )
	{
	d = new SmfAlbumPrivate;
	}

/**
 * Copy Constructor
 * @param aOther The reference object
 */
SmfAlbum::SmfAlbum( const SmfAlbum &aOther )
	: d( aOther.d )
	{
	}

/**
 * Overloaded = operator
 * @param aOther The reference object
 * @return The target reference value
 */
SmfAlbum& SmfAlbum::operator=( const SmfAlbum &aOther )
	{
	d->m_name = aOther.d->m_name;
	d->m_image = aOther.d->m_image;
	d->m_artists = aOther.d->m_artists;
	d->m_albumId = aOther.d->m_albumId;
	return *this;
	}

/**
 * Destructor
 */
SmfAlbum::~SmfAlbum( )
	{
	}

/**
 * Method to get the album name
 * @return The album name
 */
QString SmfAlbum::name( ) const
	{
	return d->m_name;
	}

/**
 * Method to get the album's image
 * @return The album's image
 */
QImage SmfAlbum::image( ) const
	{
	return d->m_image;
	}

/**
 * Method to get the artist names
 * @return The list of artists in the album
 */
SmfArtists SmfAlbum::artists( ) const
	{
	return d->m_artists;
	}

/**
 * Method to get the id of the album
 * @return The ID value 
 */
QString SmfAlbum::id( ) const
	{
	return d->m_albumId;
	}

/**
 * Method to set the album name
 * @param aName The album name
 */
void SmfAlbum::setName( const QString &aName )
	{
	d->m_name = aName;
	}

/**
 * Method to set the album's image
 * @param aImage The album's image
 */
void SmfAlbum::setImage( const QImage &aImage )
	{
	d->m_image = aImage;
	}

/**
 * Method to set the artist names
 * @param aArtists The list of artists in the album
 */
void SmfAlbum::setArtists( const SmfArtists &aArtists )
	{
	d->m_artists = aArtists;
	}

/**
 * Method to set the id of the album
 * @param aId The ID value 
 */
void SmfAlbum::setId( const QString &aId )
	{
	d->m_albumId = aId;
	}

/**
 * Method for Externalization. Writes the SmfAlbum object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aAlbum The SmfAlbum object to be externalized
 * @return reference to the written stream
 */
 QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfAlbum &aAlbum )
	{
	// Serialize d->m_name
	aDataStream<<aAlbum.d->m_name;
	
	// Serialize d->m_image
	aDataStream<<aAlbum.d->m_image;
	
	// Serialize d->m_artists
	aDataStream<<aAlbum.d->m_artists;
	
	// Serialize d->m_albumId
	aDataStream<<aAlbum.d->m_albumId;
	
	return aDataStream;
	}

/**
 * Method for Internalization. Reads a SmfAlbum object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aAlbum The SmfAlbum object to be internalized
 * @return reference to the stream
 */
 QDataStream &operator>>( QDataStream &aDataStream, 
		SmfAlbum &aAlbum)
	{
	// Deserialize d->m_name
	aDataStream>>aAlbum.d->m_name;
	
	// Deserialize d->m_image
	aDataStream>>aAlbum.d->m_image;
	
	// Deserialize d->m_artists
	aDataStream>>aAlbum.d->m_artists;
	
	// Deserialize d->m_albumId
	aDataStream>>aAlbum.d->m_albumId;
	
	return aDataStream;
	}
