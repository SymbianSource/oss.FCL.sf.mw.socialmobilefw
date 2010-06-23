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
 * The SmfGallery class 
 *
 */

#ifndef SMFGALLERY_H
#define SMFGALLERY_H

#include "smfglobal.h"
#include "smfprovider.h"
#include "smfclientglobal.h"
#include "smfpicture.h"
#include "smfpicturealbum.h"
#include "smfcomment.h"
#include "smfcontact.h"

#include <QObject>
#include <QDateTime>
#include <QStringList>

#include <qmobilityglobal.h>



class SmfGalleryPrivate;
/**
 * @ingroup smf_client_group 
 * Interface to a remote gallery service. This class
 * provides some basic gallery functionality to allow applications
 * to interact with a picture gallery in a social network.
 *
 * Note that branding information for the particular service implementation
 * is available from getProvider() API. See also:
 * SmfProvider::serviceName(), SmfProvider::serviceIcon()
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in object.
 * Interface name for SmfGallery org.symbian.smf.client.gallery
 */
class SMFCLIENT_EXPORT SmfGallery : public QObject
{
  Q_OBJECT

public:
  /**
   * Constructs SmfGallery.
   * @param baseProvider The base provider info
   * Seeing as this is a plug-in implementation, these will realistically
   be generated by SMF factory of some kind
   */

  SmfGallery(SmfProvider* baseprovider);
  ~SmfGallery();

	public slots:
	/**
	 * Get the album listing asynchronously.
	 * The albumsAvailable() signal is emitted with SmfPictureAlbumList once the albums have arrived.
	 * When the list is big user can specify the page number and per page item data.
	 * If not supplied by the user default values are used.
	 * @param names the subject or any keywords to be used to filter albums with that name
	 * @param user the user whose albums are requested 
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
	 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
	 */
	void albums(QStringList names, SmfContact* user, int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE);
	
	
  /**
   * Get the picture listing asynchronously.
   * The picturesAvailable() signal is emitted with SmfPictureList once the pictures have arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
	 * @param albums album(s) whose pictures are being requested
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
	void pictures(SmfPictureAlbumList &albums, int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE);

  /**
   * Returns a user title/caption for the picture
   */
  QString description(SmfPicture& picture);
  /**
   * Gets the base provider info
   */
   SmfProvider* getProvider() ;
	//friend so that it can directly emit SmfPostProvider's signal
	friend class SmfGalleryPrivate;
public slots:
	/**
	 * Upload an image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param image the image to be uploaded
	 * @param album the optional destination album name 
	 */
	void upload(SmfPicture* image, SmfPictureAlbum* album=NULL) ;

	/**
	 * Upload an list image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param images the list image to be uploaded
	 * @param album the optional destination album name 
	 */
	void upload(SmfPictureList* images, SmfPictureAlbum* album=NULL) ;

  /**
   * Posts a comment for an image. uploadFinished() signal is emitted
   * with success of the post once comment is posted.
   * @param image Image to comment on
   * @param comment Comment to post
   */
   void postComment(SmfPicture image, SmfComment comment) ;
   /**
	* Request for a custom operation.
	* @param operationId OperationId
	* @param customData Custom data to be sent
	* Note:-Interpretation of operationId and customData is upto the concerned
	* plugin and client application. service provider should provide some
	* serializing-deserializing utilities for these custom data
	*/
   void customRequest(const int& operationId,QByteArray* customData);
signals:
	
	/*
	 * Notification on arrival of list of SmfPictureAlbum as a result of call to @ref albums().
	 * @param pics Picture list
	 * @param error Error string
	 * @param resultPage Page number info
	 */
	void albumsAvailable(SmfPictureAlbumList* albums, SmfError error, SmfResultPage resultPage);
	
	/*
	 * Notification on arrival of list of SmfPicture as a result of request.
	 * Note if number of friends is large, then it can download the list page by page.
	 * In that case this signal is emitted multiple times.
	 * through pictures().
	 * @param pics Picture list
	 * @param error Error string
	 * @param resultPage Page number info
	 */
	void picturesAvailable(SmfPictureList* pics, SmfError error, SmfResultPage resultPage);

  /**
   * Notification of the success of the uploading of image/comment
   * @param error The upload success result of each individual pictures
   */
  void uploadFinished(QList<SmfError> error);
	/**
	 * Emitted when custom data is available
	 * @param operationId Requested operation id
	 * @param customData Custom data received, interpretation is not the responsibility of Smf
	 */
	void customDataAvailable(int operationId, QByteArray* customData);

	/** friend so that it can directly emit SmfGallery's signal*/
	friend class SmfGalleryPrivate;


private:
	SmfProvider* m_baseProvider;
	//private impl wrapper
	SmfGalleryPrivate* m_private;

};

SMF_SERVICE_NAME(SmfGallery, "org.symbian.smf.client.gallery\0.2")

#endif // SMFGALLERY_H

