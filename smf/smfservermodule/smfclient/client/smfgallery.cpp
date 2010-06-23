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

#include "smfgallery.h"
#include "smfgallery_p.h"


  SmfGallery::SmfGallery(SmfProvider* baseprovider):m_baseProvider(baseprovider)
	  {
	  //creating private impl wrapper
	  m_private = new SmfGalleryPrivate(this);
	  }
  SmfGallery::~SmfGallery()
	  {
	  if(m_private)
		  {
		  delete m_private;
		  m_private = NULL;
		  }
	  }

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
void SmfGallery::albums(QStringList names, SmfContact* user, int pageNum,int perPage)
	  {
		m_private->albums(names,user, pageNum, perPage);	
	  }

  /**
 * Get the picture listing asynchronously.
 * The picturesAvailable() signal is emitted with SmfPictureList once the pictures have arrived.
 * When the list is big user can specify the page number and per page item data.
 * If not supplied by the user default values are used.
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
void SmfGallery::pictures(SmfPictureAlbumList &albums, int pageNum,int perPage)
	  {
	m_private->pictures( albums, pageNum, perPage);
	  }

  //APIs to get/set base provider info (SmfProvider)

  /**
   * Gets the base provider info
   */
   SmfProvider* SmfGallery::getProvider() 
	   {
	   return m_baseProvider;
	   }

	/**
	 * Upload an image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param image the image to be uploaded
	 */
void SmfGallery::upload(SmfPicture* image, SmfPictureAlbum* album)
	   {
	m_private->upload(image,album);
	   }

	/**
	 * Upload an list image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param images the list image to be uploaded
	 */

void SmfGallery::upload(SmfPictureList* images, SmfPictureAlbum* album) 
	   {
	m_private->upload(images,album);
	   }

  /**
   * Posts a comment for an image. uploadFinished() signal is emitted
   * with success of the post once comment is posted.
   * @param image Image to comment on
   * @param comment Comment to post
   */
   void SmfGallery::postComment(SmfPicture image, SmfComment comment) 
	   {
	   m_private->postComment(image,comment);
	   }
   
QString SmfGallery::description(SmfPicture& picture)
   {
		Q_UNUSED(picture);
   }
/**
 * Request for a custom operation.
 * @param operationId OperationId
 * @param customData Custom data to be sent
 * Note:-Interpretation of operationId and customData is upto the concerned
 * plugin and client application. service provider should provide some
 * serializing-deserializing utilities for these custom data
 */
void SmfGallery::customRequest(const int& operationId,QByteArray* customData)
	{
	/*to be implemented*/
	Q_UNUSED(operationId);
	Q_UNUSED(customData);
	}
