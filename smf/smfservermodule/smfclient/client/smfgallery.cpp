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
   * Get the picture listing asynchronously.
   * The picturesAvailable() signal is emitted with SmfPictureList once the pictures have arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  void SmfGallery::pictures(int pageNum,int perPage)
	  {
	  m_private->pictures(pageNum,perPage);
	  }

  /**
   * Returns a user title/caption for the picture
   */
  QString SmfGallery::description(SmfPicture& picture)
	  {
	  m_private->description(picture);
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
   void SmfGallery::upload(SmfPicture* image) 
	   {
	   m_private->upload(image);
	   }

	/**
	 * Upload an list image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param images the list image to be uploaded
	 */
   void SmfGallery::upload(SmfPictureList* images) 
	   {
	   m_private->upload(images);
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

