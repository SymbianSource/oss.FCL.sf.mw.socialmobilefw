/**
 * @file	smfsubtitlesearchfilter.h
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
 * The subtitle search filter class represents the filter options 
 * for searching tracks
 */

#ifndef SMFSUBTITLESEARCHFILTER_H_
#define SMFSUBTITLESEARCHFILTER_H_

#include <QDateTime>
#include "SmfClientGlobal.h"
/**
 * The subtitle search filter class represents the filter options 
 * for searching tracks
 */
class SMFCLIENT_EXPORT SmfSubtitleSearchFilter : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfSubtitleSearchFilter( QObject *aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfSubtitleSearchFilter( );
	
	/**
	 * Method to get the language
	 * @param aLang The language
	 */
	void getLanguage( QString &aLang );
	
	/**
	 * Method to get the frame rate
	 * @return The frame rate
	 */
	double getFrameRate( );
	
	/**
	 * Method to get the duration
	 * @param aTime The duration
	 */
	void getDuration( QTime &aTime );
	
	/**
	 * Method to get the release year
	 * @param aDateTime The release year
	 */
	void getReleaseYear( QDateTime &aDateTime );

	/**
	 * Method to set the language
	 * @param aLang The new language
	 */
	void setLanguage( const QString& aLang );
	
	/**
	 * Method to set the frame rate
	 * @param aFr The new frame rate
	 */
	void setFrameRate( const double aFr );
	
	/**
	 * Method to set the duration
	 * @param aDuration The new duration
	 */
	void setDuration( const QTime& aDuration );
	
	/**
	 * Method to set the release year
	 * @param aDateTime The new release year
	 */
	void setReleaseYear( const QDateTime &aDateTime );
	
private:
	QString m_language;		// language
	double m_frameRate;		// frame rate
	QTime m_duration;		// duration
	QDateTime m_releaseYr;	// release year
	};

#endif /* SMFSUBTITLESEARCHFILTER_H_ */
