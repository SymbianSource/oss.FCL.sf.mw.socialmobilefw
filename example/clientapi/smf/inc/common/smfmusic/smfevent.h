/**
 * @file	smfevent.h
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
 * The event class represents a music event
 */

#ifndef SMFEVENT_H_
#define SMFEVENT_H_

#include <smfvenue.h>
#include <QStringList>
#include <qdatastream.h>
#include "SmfClientGlobal.h"
/**
 * The event class represents a music event
 */
class SMFCLIENT_EXPORT SmfEvent : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfEvent( QObject *aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfEvent( );
	
	/**
	 * Method to get the event name
	 * @param aName The event name
	 */
	void getTitle( QString &aName );
	
	/**
	 * Method to get the event date and time
	 */
	void getEventDateTime( QDateTime &aDateTime );
	
	/**
	 * Method to get the artist names
	 * @param aArtist The list of artists in the event
	 */
	void getArtists( QStringList &aArtist );
	
	/**
	 * Method to get the venue of the event
	 * @param aVenue The venue of the event
	 */
	void getVenue( SmfVenue &aVenue );
	
	/**
	 * Method to get the URL for getting tickets for the event
	 * @param aUrl The Url for getting ticket for the event
	 */
	void getTicketUrl( QUrl &aUrl );
	
	/**
	 * Method to get the id of the event
	 * @param aId The ID value 
	 */
	void getId( QString &aId );
	
	/**
	 * Method to set the event name
	 * @param aName The new event name
	 */
	void setTitle( const QString& aName );
	
	/**
	 * Method to set the event date and time
	 * @param aDateTime The new date and time of the event
	 * 
	 */
	void setEventDateTime( const QDateTime& aDateTime );
	
	/**
	 * Method to set the artist names
	 * @param aArtist The new list of artists in the event
	 */
	void setArtists( const QStringList& aArtist );
	
	/**
	 * Method to set the venue name
	 * @param aVenue The new venue of the event
	 */
	void setVenue( const SmfVenue& aVenue );
	
	/**
	 * Method to set the URL for getting tickets for the event
	 * @param aUrl The new Url for getting ticket for the event
	 */
	void setTicketUrl( const QUrl &aUrl );

private:
	QStringList m_name;			// event name
	QDateTime 	m_dateTime;		// event date and time
	QStringList m_artistName;	// event artist names
	SmfVenue 	m_venue;		// venue of the event
	QUrl 		m_url;			// ticket url
	QString 	m_eventId;
	
	};
/**
* Externalization
*/
QDataStream &operator<<(QDataStream &, const SmfEvent&);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfEvent&);

#endif /* SMFEVENT_H_ */
