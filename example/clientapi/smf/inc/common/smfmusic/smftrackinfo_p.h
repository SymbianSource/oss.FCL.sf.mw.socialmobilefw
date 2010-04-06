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
#include <QStringList>
#include <QSharedData>

class SmfTrackInfoPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfTrackInfoPrivate( int aMaxRating, int aMinRating ) : m_rating(aMaxRating, aMinRating) 
		{ 
		m_trackId.clear();
		m_title.clear();
		m_albumTitle.clear();
		m_artistName.clear();
		m_genre.clear();
		m_tag.clear();
		m_comment.clear();
	}
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfTrackInfoPrivate( const SmfTrackInfoPrivate &aOther ) : 
		QSharedData ( aOther ), 
		m_trackId ( aOther.m_trackId ),
		m_title ( aOther.m_title ),
		m_albumTitle ( aOther.m_albumTitle ),
		m_artistName ( aOther.m_artistName ),
		m_genre ( aOther.m_genre ),
		m_tag ( aOther.m_tag ),
		m_year ( aOther.m_year ),
		m_rating ( aOther.m_rating ),
		m_comment ( aOther.m_comment ),
		m_duration ( aOther.m_duration ) 	{ }
	
	/**
	 * Destructor
	 */
	~SmfTrackInfoPrivate( );
  
	QString m_trackId;		// track id
	QString m_title;		// title
	QString m_albumTitle;	// album name
	QString m_artistName;	// artist name
	QString m_genre;		// genre information
	QStringList m_tag;			// tag information
	QDateTime m_year;			// release year
	SmfMusicRating m_rating;// rating
	QStringList m_comment;		// comments
	QTime m_duration;		// duration
	
};

#endif /* SMFTRACKINFO_P_H_ */
