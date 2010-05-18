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
 * Private class implemented for implicit sharing of SmfMusicFingerPrint class
 *
 */

#ifndef SMFMUSICFINGERPRINT_P_H_
#define SMFMUSICFINGERPRINT_P_H_

#include <QSharedData>

class SmfMusicFingerPrintPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfMusicFingerPrintPrivate( )
		{
		}
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfMusicFingerPrintPrivate( const SmfMusicFingerPrintPrivate &aOther ) : 
		QSharedData ( aOther ), 
		m_fingerPrintData ( aOther.m_fingerPrintData )	{ }
	
	/**
	 * Destructor
	 */
	~SmfMusicFingerPrintPrivate( )
		{
		}
  
	QByteArray m_fingerPrintData;	// Finger print data
	
};


#endif /* SMFMUSICFINGERPRINT_P_H_ */
