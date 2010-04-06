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
 * The post class represents an instance of a post to a SN site
 *
 */

#ifndef SMFPOST_H_
#define SMFPOST_H_

#include <QImage>
#include <QUrl>
#include <qdatastream.h>
#include <QSharedData>
#include <QTextDocument>
#include <smfclientglobal.h>

class SmfPostPrivate;


/**
 * @ingroup smf_common_group
 * The post class represents an instance of a post to a SN site
 */
class SMFCLIENT_EXPORT SmfPost : public QTextDocument
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param text Text
	 * @param aParent The parent object
	 */
	SmfPost( QString text,QObject *aParent = 0 );
	
	/**
	 * Constructor
	 * @param aText The post's text
	 * @param aImage The post's image
	 * @param aUrl The post's url
	 */
	SmfPost( QString aTitle, QString aDesc, QImage aImage, QUrl aUrl );

	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfPost( const SmfPost &aOther );
	
	/**
	 * Destructor
	 */
	~SmfPost( );
	
	/**
	 * Method to get the title of the post
	 * @return The post's title
	 */
	QString title( ) const;
	
	/**
	 * Method to get the description of the post
	 * @return The post's description
	 */
	QString description( ) const;
	
	/**
	 * Method to get the image of the post
	 * @return The post's image
	 */
	QImage image( ) const;
	
	/**
	 * Method to get the url of the post
	 * @return The post's url
	 */
	QUrl url( ) const;
	
	/**
	 * Method to get the id of the post
	 * @return The ID value 
	 */
	QString id( ) const;
	
	/**
	 * Method to set the title of the post
	 * @param aTitle The post's new title
	 */
	void setTitle( const QString &aTitle );
	
	/**
	 * Method to set the description of the post
	 * @param aDesc The post's new description
	 */
	void setDescription( const QString &aDesc );
	
	/**
	 * Method to set the image of the post
	 * @param aPic The post's image
	 */
	void setImage( const QImage& aPic );
	
	/**
	 * Method to set the url of the post
	 * @param aUrl The post's url
	 */
	void setUrl( QUrl& aUrl );
	
private:
	QSharedDataPointer<SmfPostPrivate> d;

	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfPost &aPost );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfPost &aPost );
	
	};


/**
 * Method for Externalization. Writes the SmfPost object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aPost The SmfPost object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfPost &aPost );

/**
 * Method for Internalization. Reads a SmfPost object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aPost The SmfPost object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfPost &aPost);

#endif /* SMFPOST_H_ */
