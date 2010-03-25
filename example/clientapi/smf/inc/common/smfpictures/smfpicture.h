/**
 * @file	smfpicture.h
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
 * The picture class represents an instance of a picture
 */

#ifndef SMFPICTURE_H_
#define SMFPICTURE_H_

#include <QDateTime>
#include <QStringList>
#include <QUrl>
#include <qdatastream.h>
#include "SmfClientGlobal.h"
/**
 * SmfPictureVisibility enumeration
 */
enum SmfPictureVisibility
	{
	SMFVisibilityFriend,
	SMFVisibilityPersonal,
	SMFVisibilityFamily,
	SMFVisibilityGroup,
	SMFVisibilityPublic
	};
/**
 * The picture class represents an instance of a picture
 */
class SMFCLIENT_EXPORT SmfPicture : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfPicture( QObject *aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfPicture( );
	
	
	/**
	 * Method to get the id of the picture
	 * @param aId The ID value 
	 */
	void getId( QString &aId );
	
	/**
	 * Method to get a picture owner
	 * @param aOwner The owner of the picture
	 */
	void getOwner( QString &aOwner );
	
	/**
	 * Method to get a picture title
	 * @param aTitle The title of the picture
	 */
	void getTitle( QString &aTitle );
	
	/**
	 * Method to get a picture description
	 * @param aDescription The description of the picture
	 */
	void getDescription( QString &aDescription );
	
	/**
	 * Method to get a visibility of a picture for public
	 * @param aVisibility The visibility mode of 
	 * this picture for others
	 */
	void getVisibility( SmfPictureVisibility &aVisibility );
	
	/**
	 * Method to get the date of posting the picture
	 * @param aPostedOn The posted date of the picture
	 */
	void getPostedDate( QDateTime &aPostedOn );
	
	/**
	 * Method to get the comments for the picture
	 * @param aComments The comments for the picture
	 */
	void getComments( QStringList &aComments );
	
	/**
	 * Method to get the tags for the picture
	 * @param aTags The tags for the picture
	 */
	void getTags( QStringList &aTags );
	
	/**
	 * Method to get the url of the picture
	 * @param aUrl The url of the picture
	 */
	void getUrl( QUrl &aUrl );
	
	/**
	 * Method to get the picture data as a byte array
	 * @param aData The picture as a byte array
	 */
	void getPicture( QByteArray &aData );
		
	/**
	 * Method to set a picture owner
	 * @param aOwner The owner of the picture
	 */
	void setOwner( const QString &aOwner );
	
	/**
	 * Method to set a picture title
	 * @param aTitle The title of the picture
	 */
	void setTitle( const QString &aTitle );
	
	/**
	 * Method to set a picture description
	 * @param aDescription The description of the picture
	 */
	void setDescription( const QString &aDescription );
	
	/**
	 * Method to set a visibility of a picture for public
	 * @param aVisibility aVisibility The visibility mode of 
	 * this picture for others
	 */
	void setVisibility( const SmfPictureVisibility &aVisibility );
	
	/**
	 * Method to set the date of posting the picture
	 * @param aPostedOn The posted date of the picture
	 */
	void setPostedDate( const QDateTime &aPostedOn );
	
	/**
	 * Method to set the comments for the picture
	 * @param aComment The comment for the picture
	 */
	void setComment( const QString &aComment );
	
	/**
	 * Method to set the tags for the picture
	 * @param aTag The tag for the picture
	 */
	void setTag( const QString &aTag );
	
	/**
	 * Method to set the picture data as a byte array
	 * @param aData The picture as a byte array
	 */
	void setPicture( const QByteArray &aData );
	
private:
	QString m_photoid;		// unique ID of the picture, service provider specific
	QString m_owner;	// owner of the picture
	QString m_title;	// picture title
	QString m_descrition;// description
	bool m_ispublic;// visibility for public, Set to 0 for no, 1 for yes.
	bool m_isfriend;// visilibility for friends Set to 0 for no, 1 for yes.
	bool m_isfamily;// visilibility for family Set to 0 for no, 1 for yes.
	QDateTime m_postedon;// date posted
	QStringList m_comments;// comments
	QStringList m_tags;	// tags
	QUrl m_url;			// url
	QByteArray* m_picture;	// picture data as bytearray
	QString m_caption;	// caption
	
	};
/**
* Externalization
*/
QDataStream &operator<<(QDataStream &, const SmfPicture&);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfPicture&);
#endif /* SMFPICTURE_H_ */
