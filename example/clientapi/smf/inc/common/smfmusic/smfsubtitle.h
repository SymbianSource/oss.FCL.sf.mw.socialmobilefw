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
 * The subtitle class represents information about a track's subtitle
 *
 */

#ifndef SMFSUBTITLE_H_
#define SMFSUBTITLE_H_

#include <QObject>
#include <qdatastream.h>
#include <QDateTime>
#include <QSharedData>
#include <smfclientglobal.h>

enum SmfSubtitleSearchFilterFields
	{
	SubtitleLanguage = 0,
	SubtitleFrameRate,		// value = 1
	SubtitleDuration,		// value = 2
	SubtitleReleaseYear,	// value = 3
	SubtitleAll = SubtitleLanguage | SubtitleFrameRate | 
			SubtitleDuration | SubtitleReleaseYear
	};
typedef QMap<SmfSubtitleSearchFilterFields,QString> SmfSubtitleSearchFilter;

class SmfSubtitlePrivate;

/**
 * @ingroup smf_common_group
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
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfSubtitle( const SmfSubtitle &aOther );
	
	/**
	 * Destructor
	 */
	~SmfSubtitle( );
	
	/**
	 * Method to get the subtitle as a bytearray
	 * @return The subtitle content
	 */
	QByteArray subtitle( ) const;
	
	/**
	 * Method to get the language
	 * @return The language
	 */
	QString language( ) const;
	
	/**
	 * Method to get the frame rate
	 * @return the frame rate
	 */
	double frameRate( ) const;
	
	/**
	 * Method to get the duration
	 * @return the duration
	 */
	double duration( ) const;
	
	/**
	 * Method to get the release year
	 * @return The release year
	 */
	QDateTime releaseYear( ) const;
	
	/**
	 * Method to get the id of the subtitle
	 * @return The ID value 
	 */
	QString id( ) const;
	
private:
	QSharedDataPointer<SmfSubtitlePrivate> d;
	
	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfSubtitle &aSubtitle );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfSubtitle &aSubtitle );
	
	};


/**
 * Method for Externalization. Writes the SmfSubtitle object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aSubtitle The SmfSubtitle object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfSubtitle &aSubtitle );

/**
 * Method for Internalization. Reads a SmfSubtitle object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aSubtitle The SmfSubtitle object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfSubtitle &aSubtitle);

#endif /* SMFSUBTITLE_H_ */
