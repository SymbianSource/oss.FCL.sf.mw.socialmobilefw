/**
 * @file	smfcomment.h
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
 * The comment class represents a comment on a picture or a music track
 */

#ifndef SMFCOMMENT_H_
#define SMFCOMMENT_H_

#include <QDateTime>
#include <qdatastream.h>
#include "SmfClientGlobal.h"
/**
 * The comment class represents a comment on a picture or a music track
 */
class SMFCLIENT_EXPORT SmfComment : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object 
	 */
	SmfComment( QObject *aParent = 0 );
	/**
	 * Destructor
	 */
	~SmfComment( );
	

	/**
	 * Method to get the comment text
	 * @param aText The comment text
	 */
	void getText( QString &aText );
	
	/**
	 * Method to get the comment time stamp
	 * @param aTimeStamp The comment time stamp value
	 */
	void getTimeStamp( QDateTime &aTimeStamp );
	
	/**
	 * Method to get the id of the comment
	 * @param aId The ID value 
	 */
	void getId( QString &aId );
	
	/**
	 * Method to set the comment text
	 * @param aText The comment text to be set
	 */
	void setText( const QString &aText );
	
	/**
	 * Method to set the comment time stamp
	 * @param aTimeStamp The comment time stamp value to be set
	 */
	void setTimeStamp( const QDateTime &aTimeStamp );
	
private:
	QString m_text;			// comment text
	QDateTime m_timeStamp;	// comment time stamp
	QString m_commetnId;
	
	};
/**
 * Externalization
 */
QDataStream &operator<<(QDataStream &, const SmfComment &);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfComment &);
#endif /* SMFCOMMENT_H_ */
