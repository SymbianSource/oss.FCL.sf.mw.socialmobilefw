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

#include <smfmusicprofile.h>
#include <smfmusicprofile_p.h>

/**
 * Constructor with default argument
 */
SmfMusicProfile::SmfMusicProfile( )
	{
	d = new SmfMusicProfilePrivate;
	}

/**
 * Copy Constructor
 * @param aOther The reference object
 */
SmfMusicProfile::SmfMusicProfile( const SmfMusicProfile &aOther )
	:d ( aOther.d )
	{
	}

/**
 * Overloaded = operator
 * @param aOther The reference object
 * @return The target reference value
 */
SmfMusicProfile& SmfMusicProfile::operator=( const SmfMusicProfile &aOther )
	{
	d->m_usage = aOther.d->m_usage;
	d->m_interest = aOther.d->m_interest;
	d->m_events = aOther.d->m_events;
	d->m_profileId = aOther.d->m_profileId;
	return *this;
	}

/**
 * Destructor
 */
SmfMusicProfile::~SmfMusicProfile( )
	{
	}

/**
 * Method to get the user's used tracks
 * @return The users track list
 */
QList<SmfTrackInfo> SmfMusicProfile::musicUsageInfo( ) const
	{
	return d->m_usage;
	}

/**
 * Method to get the user's interested tracks
 * @return The users interested track list
 */
QList<SmfTrackInfo> SmfMusicProfile::interestInfo( ) const
	{
	return d->m_interest;
	}

/**
 * Method to get the user events as list of SmfEvents
 * @return The list of events
 */
QList<SmfEvent> SmfMusicProfile::userEvents( ) const
	{
	return d->m_events;
	}

/**
 * Method to get the id of the music profile
 * @return The ID value 
 */
QString SmfMusicProfile::id( ) const
	{
	return d->m_profileId;
	}

/**
 * Method to set the user's used tracks
 * @param aUsage The users new track list
 */
void SmfMusicProfile::setMusicUsageInfo( const QList<SmfTrackInfo>& aUsage )
	{
	d->m_usage = aUsage;
	}

/**
 * Method to set the user's interested tracks
 * @param aInterest The users new interested track list
 */
void SmfMusicProfile::setInterestInfo( const QList<SmfTrackInfo>& aInterest )
	{
	d->m_interest = aInterest;
	}

/**
 * Method to set the user events as list of SmfEvents
 * @param aList The list of events
 */
void SmfMusicProfile::setUserEvents( const QList<SmfEvent> &aList )
	{
	d->m_events = aList;
	}

/**
 * Method to set the id of the music profile
 * @param aId The ID value 
 */
void SmfMusicProfile::setId( const QString &aId )
	{
	d->m_profileId = aId;
	}


/**
 * Method for Externalization. Writes the SmfMusicProfile object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aProfile The SmfMusicProfile object to be externalized
 * @return reference to the written stream
 */
 QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfMusicProfile &aProfile )
	{
	// Serialize d->m_usage
	aDataStream<<aProfile.d->m_usage;
	
	// Serialize d->m_interest
	aDataStream<<aProfile.d->m_interest;
	
	// Serialize d->m_events
	aDataStream<<aProfile.d->m_events;
	
	// Serialize d->m_profileId
	aDataStream<<aProfile.d->m_profileId;
	
	return aDataStream;
	}

/**
 * Method for Internalization. Reads a SmfMusicProfile object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aProfile The SmfMusicProfile object to be internalized
 * @return reference to the stream
 */
 QDataStream &operator>>( QDataStream &aDataStream, 
		SmfMusicProfile &aProfile)
	{
	// Deserialize d->m_usage
	aDataStream>>aProfile.d->m_usage;
	
	// Deserialize d->m_interest
	aDataStream>>aProfile.d->m_interest;
	
	// Deserialize d->m_events
	aDataStream>>aProfile.d->m_events;
	
	// Deserialize d->m_profileId
	aDataStream>>aProfile.d->m_profileId;
	
	return aDataStream;
	}

