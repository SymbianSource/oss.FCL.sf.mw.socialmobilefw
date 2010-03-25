/**
 * @file	smflyrics.h
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
 * The lyrics class represents an instance of a music track's lyrics
 */

#ifndef SMFLYRICS_H_
#define SMFLYRICS_H_

#include <QObject>
#include <qdatastream.h>
#include <QDateTime>
#include "SmfClientGlobal.h"
/**
 * The lyrics class represents an instance of a music track's lyrics
 */
class SMFCLIENT_EXPORT SmfLyrics : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfLyrics( QObject *aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfLyrics( );

	/**
	 * Method to get the lyrics
	 * @param aLyrics The lyrics content
	 */
	void getLyrics( QByteArray &aLyrics );
	
	/**
	 * Method to get the language
	 * @param aLanguage The language
	 */
	void getLanguage( QString &aLanguage );
	
	/**
	 * Method to get the frame rate
	 * @return the frame rate
	 */
	double getFrameRate( );
	
	/**
	 * Method to get the duration
	 * @return the duration
	 */
	double getDuration( );
	
	/**
	 * Method to get the release year
	 * @param aRelYear The release year
	 */
	void getReleaseYear( QDateTime &aRelYear );
	
	/**
	 * Method to get the id of the lyrics
	 * @param aId The ID value 
	 */
	void getId( QString &aId );

private:
	QByteArray m_lyrics;	// lyrics data
	QString m_language;		// language
	double m_frameRate;		// frame rate
	double m_duration;		// duration
	QDateTime m_releaseYr;	// release year
	QString m_lyricsId;		// lyrics id
	
	};
/**
* Externalization
*/
QDataStream &operator<<(QDataStream &, const SmfLyrics&);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfLyrics&);
#endif /* SMFLYRICS_H_ */
