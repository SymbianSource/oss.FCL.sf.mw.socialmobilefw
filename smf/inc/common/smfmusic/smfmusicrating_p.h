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
 * Private class implemented for implicit sharing of SmfMusicRating class
 *
 */

#ifndef SMFMUSICRATING_P_H_
#define SMFMUSICRATING_P_H_

#include <QSharedData>
#include <smfmusicrating.h>

class SmfMusicRatingPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfMusicRatingPrivate( ) ;
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfMusicRatingPrivate( const SmfMusicRatingPrivate &aOther );
	/**
	 * Destructor
	 */
	~SmfMusicRatingPrivate( );
  
	qint32 m_rating;	// rating
	qint32 m_max;		// max rating
	qint32 m_min;		// min rating
	
};

#endif /* SMFMUSICRATING_P_H_ */
