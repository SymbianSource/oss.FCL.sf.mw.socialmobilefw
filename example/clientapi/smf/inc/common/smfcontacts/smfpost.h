/**
* Copyright (c) 2010 Sasken Communication Technologies Ltd.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "{License}"
* which accompanies  this distribution, and is available
* at the URL "{LicenseUrl}".
*
* Initial Contributors:
* Chandradeep Gandhi, Sasken Communication Technologies Ltd - Initial contribution
*
* Contributors:
* Manasij Roy
* Description:
* Interface spefication for sfm service provider
*
*/

#ifndef SMFPOST_H_
#define SMFPOST_H_
#include "SmfClientGlobal.h"
/**
 * class for post, later may be we need to add media. 
 * Post consists of a title and description
 */

class SMFCLIENT_EXPORT SmfPost : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructs a post 
	 */
	SmfPost(QObject* provider=0);
	
	/**
	 * Constructs a post with text,image and url
	 */
	SmfPost(QString text,QPixmap image, QString url);
	~SmfPost();
	
	/**
	 * Gets post title
	 */
	QString* getTitle();
	
	/**
	 * Gets post text
	 */
	QString* getDescription();
	
	/**
	 * Sets post title
	 */
	void setTitle(QString* title);
	
	/**
	 * Sets post text
	 */
	void setDescription(QString* desc);

private:
	QString m_text;
	QPixmap m_image;
	QString m_url;
	};
/**
* Externalization
*/
QDataStream &operator<<(QDataStream &, const SmfPost&);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfPost&);
#endif /* SMFPOST_H_ */
