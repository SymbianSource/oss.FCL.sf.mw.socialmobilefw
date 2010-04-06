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
 * The music profile class represents a user's profile in music site
 *
 */

#ifndef SMFMUSICPROFILE_H_
#define SMFMUSICPROFILE_H_

#include <smftrackinfo.h>
#include <smfevent.h>
#include <qdatastream.h>
#include <QSharedData>
#include <smfclientglobal.h>

class SmfMusicProfilePrivate;

/**
 * @ingroup smf_common_group
 * The music profile class represents a user's profile in music site
 */
class SMFCLIENT_EXPORT SmfMusicProfile : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfMusicProfile( QObject *aParent = 0 );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfMusicProfile( const SmfMusicProfile &aOther );
	
	/**
	 * Destructor
	 */
	~SmfMusicProfile( );
	
	/**
	 * Method to get the user's used tracks
	 * @return The users track list
	 */
	QList<SmfTrackInfo> musicUsageInfo( ) const;
	
	/**
	 * Method to get the user's interested tracks
	 * @return The users interested track list
	 */
	QList<SmfTrackInfo> interestInfo( ) const;
	
	/**
	 * Method to get the user events as list of SmfEvents
	 * @return The list of events
	 */
	QList<SmfEvent> userEvents( ) const;
	
	/**
	 * Method to get the id of the music profile
	 * @return The ID value 
	 */
	QString id( ) const;
	
	/**
	 * Method to set the user's used tracks
	 * @param aUsage The users new track list
	 */
	void setMusicUsageInfo( const QList<SmfTrackInfo>& aUsage );
	
	/**
	 * Method to set the user's interested tracks
	 * @param aInterest The users new interested track list
	 */
	void setInterestInfo( const QList<SmfTrackInfo>& aInterest );

private:
	QSharedDataPointer<SmfMusicProfilePrivate> d;

	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfMusicProfile &aProfile );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfMusicProfile &aProfile );
	
	};


/**
 * Method for Externalization. Writes the SmfMusicProfile object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aProfile The SmfMusicProfile object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfMusicProfile &aProfile );

/**
 * Method for Internalization. Reads a SmfMusicProfile object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aProfile The SmfMusicProfile object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfMusicProfile &aProfile);

#endif /* SMFMUSICPROFILE_H_ */
