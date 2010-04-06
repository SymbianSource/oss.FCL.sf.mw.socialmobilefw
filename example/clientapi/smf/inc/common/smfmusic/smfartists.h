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
 * The SmfArtists class represents the artists in a track or an album
 *
 */

#ifndef SMFARTISTS_H_
#define SMFARTISTS_H_

#include <QImage>
#include <QUrl>
#include <qdatastream.h>
#include <QSharedData>
#include <smfclientglobal.h>

class SmfArtistsPrivate;

/**
 * @ingroup smf_common_group
 * The SmfArtists class represents the artists in a track or an album
 */
class SMFCLIENT_EXPORT SmfArtists : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfArtists( QObject *aParent = 0 );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfArtists( const SmfArtists &aOther );
	
	/**
	 * Destructor
	 */
	~SmfArtists( );
	
	/**
	 * Method to get the artists names
	 * @return The list of artists
	 */
	QStringList names( ) const;
	
	/**
	 * Method to get the image of the artists
	 * @return The image of te artists
	 */
	QImage image( ) const;
	
	/**
	 * Method to get the url of the artists
	 * @return The url of the artists
	 */
	QUrl url( ) const;
		
private:
	QSharedDataPointer<SmfArtistsPrivate> d;
	
	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfArtists &aArtists );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfArtists &aArtists );
	
	};


/**
 * Method for Externalization. Writes the SmfArtists object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aArtists The SmfArtists object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfArtists &aArtists );

/**
 * Method for Internalization. Reads a SmfArtists object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aArtists The SmfArtists object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfArtists &aArtists);


#endif /* SMFARTISTS_H_ */
