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
 * The SmfMusicFingerPrint class represents a music finger print used in searches
 *
 */

#ifndef SMFMUSICFINGERPRINT_H_
#define SMFMUSICFINGERPRINT_H_

#include <qdatastream.h>
#include <QSharedData>
#include <smfclientglobal.h>

class SmfMusicFingerPrintPrivate;

/**
 * @ingroup smf_common_group
 * The musicfingerprint class represents a music finger print used in searches
 */
class SMFCLIENT_EXPORT SmfMusicFingerPrint : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfMusicFingerPrint( QObject *aParent = 0 );
	
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfMusicFingerPrint( const SmfMusicFingerPrint &aOther );
	
	/**
	 * Destructor
	 */
	~SmfMusicFingerPrint( );
	
	/**
	 * GEt the music finger print data
	 * @return The music finger print data
	 */
	QByteArray musicFingerPrint ( ) const;
	
private:
	QSharedDataPointer<SmfMusicFingerPrintPrivate> d;
	
	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfMusicFingerPrint &aMFP );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfMusicFingerPrint &aMFP );
	
	};


/**
 * Method for Externalization. Writes the SmfMusicFingerPrint object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aMFP The SmfMusicFingerPrint object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfMusicFingerPrint &aMFP );

/**
 * Method for Internalization. Reads a SmfMusicFingerPrint object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aMFP The SmfMusicFingerPrint object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfMusicFingerPrint &aMFP);

#endif /* SMFMUSICFINGERPRINT_H_ */