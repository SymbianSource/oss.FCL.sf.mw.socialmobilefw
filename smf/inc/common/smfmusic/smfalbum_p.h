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
 * Private class implemented for implicit sharing of SmfAlbum class
 *
 */

#ifndef SMFALBUM_P_H_
#define SMFALBUM_P_H_

#include <smfartists.h>
#include <QStringList>
#include <QSharedData>

class SmfAlbumPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfAlbumPrivate( ) { 
		m_name.clear(); 
		m_albumId.clear();
		
	}
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfAlbumPrivate( const SmfAlbumPrivate &aOther ) : 
		QSharedData ( aOther ), 
		m_name ( aOther.m_name ), 
		m_image ( aOther.m_image ),
		m_artists ( aOther.m_artists ),
		m_albumId ( aOther.m_albumId ) 	{ }
	
	/**
	 * Destructor
	 */
	~SmfAlbumPrivate( );
  
	QString		m_name;		// album name
	QImage 		m_image;	// album image
	SmfArtists m_artists;	// album's artists
	QString 	m_albumId;
	
};

#endif /* SMFALBUM_P_H_ */
