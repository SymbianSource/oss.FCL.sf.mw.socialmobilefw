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
 * The SmfEvent class represents an event
 *
 */

#ifndef SMFEVENT_H_
#define SMFEVENT_H_

#include <smfplace.h>
#include <smfartists.h>
#include <QStringList>
#include <qdatastream.h>
#include <QSharedData>
#include <smfclientglobal.h>

class SmfEventPrivate;

/**
 * @ingroup smf_common_group
 * The event class represents an event
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
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfEvent( const SmfEvent &aOther );
	
	/**
	 * Destructor
	 */
	~SmfEvent( );
	
	/**
	 * Method to get the event name
	 * @return The event name
	 */
	QString title( ) const;
	
	/**
	 * Method to get the event date and time
	 * @return The date and time of the event
	 */
	QDateTime eventDateTime( ) const;
	
	/**
	 * Method to get the event duration
	 * @return The duration of the event
	 */
	QTime duration( ) const;
	
	/**
	 * Method to get the artist names
	 * @return The list of artists in the event
	 */
	SmfArtists artists( ) const;
	
	/**
	 * Method to get the venue of the event
	 * @return The venue of the event
	 */
	SmfPlace venue( ) const;
	
	/**
	 * Method to get the URL for getting tickets for the event
	 * @return The Url for getting ticket for the event
	 */
	QUrl ticketUrl( ) const;
	
	/**
	 * Method to get the id of the event
	 * @return The ID value 
	 */
	QString id( ) const;
	
	/**
	 * Method to set the event name
	 * @param aName The new event name
	 */
	void setTitle( const QString &aName );
	
	/**
	 * Method to set the event date and time
	 * @param aDateTime The new date and time of the event
	 * 
	 */
	void setEventDateTime( const QDateTime &aDateTime );
	
	/**
	 * Method to set the event duration
	 * @param aDuration The new duration of the event
	 * 
	 */
	void setDuration( const QTime &aDuration );
	
	/**
	 * Method to set the artist
	 * @param aArtists The new artists in the event
	 */
	void setArtists( const SmfArtists &aArtists );
	
	/**
	 * Method to set the venue name
	 * @param aVenue The new venue of the event
	 */
	void setVenue( const SmfPlace &aVenue );
	
	/**
	 * Method to set the URL for getting tickets for the event
	 * @param aUrl The new Url for getting ticket for the event
	 */
	void setTicketUrl( const QUrl &aUrl );
	
private:
	QSharedDataPointer<SmfEventPrivate> d;
	
	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfEvent &aEvent );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfEvent &aEvent );
	
	};


/**
 * Method for Externalization. Writes the SmfEvent object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aEvent The SmfEvent object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfEvent &aEvent );

/**
 * Method for Internalization. Reads a SmfEvent object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aEvent The SmfEvent object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfEvent &aEvent);

#endif /* SMFEVENT_H_ */
