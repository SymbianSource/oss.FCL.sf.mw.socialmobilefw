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
 * Private class implemented for implicit sharing of SmfPlaylist class
 *
 */

#ifndef SMFPLAYLIST_P_H_
#define SMFPLAYLIST_P_H_

#include <smftrackinfo.h>
#include <QSharedData>

class SmfPlaylistPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfPlaylistPrivate( ) { 
		m_trackList.clear(); 
		m_title.clear();
		m_playlistId.clear();
	}
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfPlaylistPrivate( const SmfPlaylistPrivate &aOther ) : 
		QSharedData ( aOther ), 
		m_trackList ( aOther.m_trackList ), 
		m_title ( aOther.m_title ),
		m_creationDate ( aOther.m_creationDate ),
		m_playlistId ( aOther.m_playlistId ) 	{ }
	
	/**
	 * Destructor
	 */
	~SmfPlaylistPrivate( );
  
	QList<SmfTrackInfo> m_trackList;// list of tracks
	QString m_title;				// playlist name
	QDateTime m_creationDate;		// creation date
	QString m_playlistId;
	
};

#endif /* SMFPLAYLIST_P_H_ */
