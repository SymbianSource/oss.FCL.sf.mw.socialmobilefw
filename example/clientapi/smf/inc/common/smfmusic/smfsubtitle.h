/**
 * @file	smfsubtitle.h
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
 * The subtitle class represents information about a track's subtitle
 */

#ifndef SMFSUBTITLE_H_
#define SMFSUBTITLE_H_

#include <QObject>
#include <qdatastream.h>
#include <QDateTime>
#include "SmfClientGlobal.h"
/**
 * The subtitle class represents information about a track's subtitle
 */
class SMFCLIENT_EXPORT SmfSubtitle : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfSubtitle( QObject *aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfSubtitle( );
	

	/**
	 * Method to get the subtitle as a bytearray
	 * @param aSubTitle The subtitle content
	 */
	void getSubtitle( QByteArray &aSubTitle );
	
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
	 * Method to get the id of the subtitle
	 * @param aId The ID value 
	 */
	void getId( QString &aId );
	
private:
	QByteArray m_subtitle;	// subtitle data
	QString m_language;		// language
	double m_frameRate;		// frame rate
	double m_duration;		// duration
	QDateTime m_releaseYr;	// release year
	QString m_subtitleId;	// subtitle id
	};
/**
 * Externalization
 */
QDataStream &operator<<(QDataStream &, const SmfSubtitle&);
	/**
	 * Internalization
	 */
QDataStream &operator>>(QDataStream &, SmfSubtitle&);
#endif /* SMFSUBTITLE_H_ */
