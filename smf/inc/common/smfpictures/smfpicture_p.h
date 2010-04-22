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
 * Private class implemented for implicit sharing of SmfPicture class
 *
 */

#ifndef SMFPICTURE_P_H_
#define SMFPICTURE_P_H_


#include <QDateTime>
#include <QStringList>
#include <QUrl>
#include <QSharedData>
#include <smfclientglobal.h>
#include <smfpicture.h>

class SmfPicturePrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfPicturePrivate( );
	/**
	 * Constructor
	 */
	SmfPicturePrivate( const QImage &aImage );
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfPicturePrivate( const SmfPicturePrivate &aOther ) ;
	/**
	 * Destructor
	 */
	~SmfPicturePrivate( );
  
	QString m_photoId;		// unique ID of the picture, service provider specific
	QString m_owner;	// owner of the picture
	QString m_title;	// picture title
	QString m_description;// description
	SmfPictureVisibility m_picVisibility; // picture visibility
	QDateTime m_postedOn;// date posted
	QStringList m_comments;// comments
	QStringList m_tags;	// tags
	QUrl m_url;			// url
	QImage m_picture;	// picture data as bytearray
	QString m_caption;	// caption
	
};

#endif /* SMFPICTURE_P_H_ */
