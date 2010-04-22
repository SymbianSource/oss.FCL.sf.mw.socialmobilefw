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
 * Private class implemented for implicit sharing of SmfTrackInfo class
 *
 */

#ifndef SMFTRACKINFO_P_H_
#define SMFTRACKINFO_P_H_

#include <QTime>
#include <smfmusicrating.h>
#include <smfcomment.h>
#include <QStringList>
#include <QSharedData>

class SmfTrackInfoPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfTrackInfoPrivate( int aMaxRating = 0, int aMinRating = 0 );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfTrackInfoPrivate( const SmfTrackInfoPrivate &aOther ) ;
	/**
	 * Destructor
	 */
	~SmfTrackInfoPrivate( );
  
	QString m_trackId;		// track id
	QString m_title;		// title
	SmfAlbum m_album;		// album 
	SmfArtists m_artists;	// artist name
	QString m_genre;		// genre information
	QStringList m_tags;		// tag information
	QDateTime m_year;			// release year
	SmfMusicRating m_rating;// rating
	QList<SmfComment> m_comments;	// comments
	QTime m_duration;		// duration
	
};

#endif /* SMFTRACKINFO_P_H_ */
