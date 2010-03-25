/**
 * @file	smftrackinfo.h
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
 * The track info class represents information about a music track
 */

#ifndef SMFTRACKINFO_H_
#define SMFTRACKINFO_H_

#include <QTime>
#include <qdatastream.h>
#include <smfmusicrating.h>
#include "SmfClientGlobal.h"
/**
 * Music track info as track id,title, album, artist, genre,
 * tag, director,release year, rating, comment info 
 */
class SMFCLIENT_EXPORT SmfTrackInfo : public QObject
	{
	Q_OBJECT
public:
	 /**
	  * Constructs track info 
	  */
	SmfTrackInfo(QObject* parent=0);
	/**
	 * Gets track id
	 */
	void getId(QString& id);
	
	/**
	 * Gets title of the track
	 */
	void getTitle(QString& title);
	
	/**
	 * Gets album name
	 */
	void getAlbum(QString& album);
	
	/**
	 * Gets track artist
	 */
	void getArtist(QString& artist);
	
	/**
	 * Gets track genre
	 */
	void getGenre(QString& genre);
	
	/**
	 * Gets track tag
	 */
	void getTag(QString& tag);
	
	/**
	 * Gets director name
	 */
	void getDirector(QString& director);
	
	/**
	 * Gets release year
	 */
	void getYear(QString& year);
	
	/**
	 * Gets track rating 
	 */
	void getRating(SmfMusicRating& rating);
	
	/**
	 * Gets comments
	 */
	void getComment(QStringList& comment);
	
	
	/**
	 * Sets id
	 */
	void setId(QString& id);
	
	/**
	 * Sets title
	 */
	void setTitle(QString& title);
	
	/**
	 * Sets album name
	 */
	void setAlbum(QString& album);
	
	/**
	 * Sets artist name
	 */
	void setArtist(QString& artist);
	
	/**
	 * Sets genre
	 */
	void setGenre(QString& genre);
	
	/**
	 * Sets tag
	 */
	void setTag(QString& tag);
	
	/**
	 * Sets director name
	 */
	void setDirector(QString& director);
	
	/**
	 * Sets release year
	 */
	void setYear(QString& year);
	
	/**
	 * Sets rating
	 */
	void setRating(SmfMusicRating& rating);
	
	/**
	 * Sets comment
	 */
	void setComment(QStringList& comment);

private:
	QString m_id;
	QString m_title;
	QString m_albumTitle;
	QString m_artistName;
	QString m_genre;
	QString m_tag;
	QString m_director;
	QString m_year;
	SmfMusicRating m_rating;
	QString m_comment;
	};
/**
* Externalization
*/
QDataStream &operator<<(QDataStream &, const SmfTrackInfo&);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfTrackInfo&);
#endif /* SMFTRACKINFO_H_ */
