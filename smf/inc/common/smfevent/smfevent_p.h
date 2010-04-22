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
 * Private class implemented for implicit sharing of SmfEvent class
 *
 */

#ifndef SMFEVENT_P_H_
#define SMFEVENT_P_H_


#include <smfplace.h>
#include <smfartists.h>
#include <QStringList>
#include <QSharedData>

class SmfEventPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfEventPrivate( );
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfEventPrivate( const SmfEventPrivate &aOther );
	/**
	 * Destructor
	 */
	~SmfEventPrivate( );
  
	QString		m_name;			// event name
	QDateTime 	m_dateTime;		// event date and time
	QTime		m_duration;		// event duration
	SmfArtists m_artists;		// event artist names
	SmfPlace 	m_venue;		// venue of the event
	QUrl 		m_url;			// ticket url
	QString 	m_eventId;	// event Id
	
};

#endif /* SMFEVENT_P_H_ */
