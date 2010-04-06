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

class SmfPicturePrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfPicturePrivate( ) { 
		m_photoId.clear();
		m_owner.clear();
		m_title.clear();
		m_description.clear();
		m_comments.clear();
		m_tags.clear();
		m_url.clear();
		m_caption.clear();
	}
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfPicturePrivate( const SmfPicturePrivate &aOther ) : 
		QSharedData ( aOther ), 
		m_photoId ( aOther.m_photoId ),
		m_owner ( aOther.m_owner ),
		m_title ( aOther.m_title ),
		m_description ( aOther.m_description ),
		m_picVisibility ( aOther.m_picVisibility ),
		m_postedOn ( aOther.m_postedOn ),
		m_comments ( aOther.m_comments ),
		m_tags ( aOther.m_tags ),
		m_url ( aOther.m_url ),
		m_picture ( aOther.m_picture ),
		m_caption ( aOther.m_caption ) 	{ }
	
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
