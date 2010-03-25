/**
 * @file	smfmusicrating.h
 * @author  Nalina Hariharan, Sasken Communication Technologies Ltd - Initial contribution
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (c) 2010 Sasken Communication Technologies Ltd. 
 * All rights reserved.
 * This component and the accompanying materials are made available 
 * under the terms of the "{License}" 
 * which accompanies  this distribution, and is available 
 * at the URL "{LicenseUrl}".
 * 
 * @section DESCRIPTION
 *
 * The music rating class represents an instance of rating 
 * about a music track 
 */

#ifndef SMFMUSICRATING_H_
#define SMFMUSICRATING_H_

#include <QObject>
#include "SmfClientGlobal.h"
class SmfTrackInfo;
/**
 * Rating value, services should define their own scale
 */
class SMFCLIENT_EXPORT SmfMusicRating : QObject
	{
	Q_OBJECT
	
public:
	/**
	 * Constructs the rating for the given track
	 */
	SmfMusicRating(SmfTrackInfo* track);
	/**
	 * Gets the rating
	 */
	int getRating();
	
	/**
	 * Gets the max for the rating scale
	 */
	int getMax();
	
	/**
	 * Gets the min for the rating scale
	 */
	int getMin();
	
	/**
	 * Gets the rating
	 */
	void setRating(int rating);
	
	/**
	 * Sets the max for the rating scale
	 */
	void setMax(int max);
	
	/**
	 * Sets the min for the rating scale
	 */
	void setMin(int min);

private:
	int m_rating;
	int m_max;
	int m_min;
	};
/**
* Externalization
*/
QDataStream &operator<<(QDataStream &, const SmfMusicRating&);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfMusicRating&);
#endif /* SMFMUSICRATING_H_ */
