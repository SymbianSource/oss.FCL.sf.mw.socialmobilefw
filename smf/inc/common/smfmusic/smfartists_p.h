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
 * Private class implemented for implicit sharing of SmfArtists class
 *
 */

#ifndef SMFARTISTS_P_H_
#define SMFARTISTS_P_H_

#include <QImage>
#include <QUrl>
#include <QStringList>
#include <QSharedData>

class SmfArtistsPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfArtistsPrivate( );
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfArtistsPrivate( const SmfArtistsPrivate &aOther );
	/**
	 * Destructor
	 */
	~SmfArtistsPrivate( );
  
	QStringList	m_names;	// artist names
	QImage 		m_image;	// image
	QUrl 		m_url;		// url
};


#endif /* SMFARTISTS_P_H_ */
