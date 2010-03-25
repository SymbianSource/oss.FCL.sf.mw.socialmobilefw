/**
 * @file	smfmusicfingerprint.h
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
 * The musicfingerprint class represents a music finger print used in searches
 */

#ifndef SMFMUSICFINGERPRINT_H_
#define SMFMUSICFINGERPRINT_H_

#include <QObject>

/**
 * The musicfingerprint class represents a music finger print used in searches
 */
class SmfMusicFingerPrint : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfMusicFingerPrint( QObject *aParent = 0 );
	/**
	 * Destructor
	 */
	~SmfMusicFingerPrint( );
	
	};
/**
* Externalization
*/
friend QDataStream &operator<<(QDataStream &, const SmfMusicFingerPrint&);
/**
 * Internalization
 */
friend QDataStream &operator>>(QDataStream &, SmfMusicFingerPrint&);
#endif /* SMFMUSICFINGERPRINT_H_ */
