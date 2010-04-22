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
 * Private class implemented for implicit sharing of SmfSubtitle class
 *
 */

#ifndef SMFSUBTITLE_P_H_
#define SMFSUBTITLE_P_H_

#include <QDateTime>
#include <QSharedData>

class SmfSubtitlePrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfSubtitlePrivate( ) ;
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfSubtitlePrivate( const SmfSubtitlePrivate &aOther );
	
	/**
	 * Destructor
	 */
	~SmfSubtitlePrivate( );
  
	QByteArray m_subtitle;	// subtitle data
	QString m_language;		// language
	double m_frameRate;		// frame rate
	double m_duration;		// duration
	QDateTime m_releaseYr;	// release year
	QString m_subtitleId;	// subtitle id
	
};

#endif /* SMFSUBTITLE_P_H_ */
