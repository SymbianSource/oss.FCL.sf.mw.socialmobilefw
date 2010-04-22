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
 * Private class implemented for implicit sharing of SmfPost class
 *
 */

#ifndef SMFPOST_P_H_
#define SMFPOST_P_H_

#include <QSharedData>

class SmfPostPrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfPostPrivate( ) ;
	
	/**
	 * Constructor
	 */
	SmfPostPrivate( QString aTitle, QString aDesc, QImage aImage, QUrl aUrl );

	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfPostPrivate( const SmfPostPrivate &aOther );
	
	/**
	 * Destructor
	 */
	~SmfPostPrivate( );
  
	QString m_title;	// title
	QString m_desc;		// description
	QImage m_image;		// image
	QUrl m_url;			// url
	QString m_postId;	// post's unique Id
	
};

#endif /* SMFPOST_P_H_ */
