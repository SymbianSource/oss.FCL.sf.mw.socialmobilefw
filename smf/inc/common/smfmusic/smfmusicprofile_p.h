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
 * Private class implemented for implicit sharing of SmfMusicProfile class
 *
 */

#ifndef SMFMUSICPROFILE_P_H_
#define SMFMUSICPROFILE_P_H_


#include <smftrackinfo.h>
#include <smfevent.h>
#include <QSharedData>

class SmfMusicProfilePrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfMusicProfilePrivate( ) ;
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfMusicProfilePrivate( const SmfMusicProfilePrivate &aOther );
	/**
	 * Destructor
	 */
	~SmfMusicProfilePrivate( );
  
	QList<SmfTrackInfo> m_usage;	// usage tracks
	QList<SmfTrackInfo> m_interest;// interested tracks
	QList<SmfEvent> m_events;		// events list
	QString m_profileId;			// profile Id
	
};

#endif /* SMFMUSICPROFILE_P_H_ */
