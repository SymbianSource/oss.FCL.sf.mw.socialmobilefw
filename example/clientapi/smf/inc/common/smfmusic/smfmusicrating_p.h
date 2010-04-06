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

class SmfMusicRatingPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfMusicRatingPrivate( int aMax, int aMin ) { 
		m_rating = 0;
		m_max = aMax;
		m_min = aMin;
	}
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfMusicRatingPrivate( const SmfMusicRatingPrivate &aOther ) : 
		QSharedData ( aOther ), 
		m_rating ( aOther.m_rating ),
		m_max ( aOther.m_max ),
		m_min ( aOther.m_min )  { }
	
	/**
	 * Destructor
	 */
	~SmfMusicRatingPrivate( );
  
	int m_rating;	// rating
	int m_max;		// max rating
	int m_min;		// min rating
	
};

#endif /* SMFMUSICRATING_P_H_ */
