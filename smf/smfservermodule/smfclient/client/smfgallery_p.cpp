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
 * Private implementation for SmfGallery class 
 *
 */
#include "smfgallery_p.h"
//logging headers
#ifdef WRITE_LOG
#include <QTextStream>
#include <QFile>
#endif
#ifdef Q_OS_SYMBIAN
#include "smfclientsymbian.h"
#else
#include "smfclientqt.h"
#endif

  SmfGalleryPrivate::SmfGalleryPrivate(SmfGallery* gallery)
  : m_gallery(gallery)
  	{
  	  //private impl for symbian
  	#ifdef Q_OS_SYMBIAN
  	m_SmfClientPrivate = CSmfClientSymbian::NewL(this);
  	#endif
  	}
  SmfGalleryPrivate::~SmfGalleryPrivate()
	  {
	  if(m_SmfClientPrivate)
		  {
		  delete m_SmfClientPrivate;
		  m_SmfClientPrivate = NULL;
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
  void SmfGalleryPrivate::pictures(int pageNum,int perPage)
	  {
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  
	  m_baseProvider = m_gallery->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  //serialize end
	  QString intfName(galleryInterface);
	  
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPictureGetPictures);
	  }

  /**
   * Returns a user title/caption for the picture
   */
  QString SmfGalleryPrivate::description(SmfPicture& picture)
	  {
	  //We need to pass Opcode and SmfProvider+SmfPicture serialized into bytearray 
	  
	  m_baseProvider = m_gallery->getProvider();
	  //serialize start
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  write<<picture;
	  //serialize end
	  QString intfName(galleryInterface);
	  
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPictureDescription);
	  //TODO:-to be implemented
	  return QString();
	  }
  /**
	 * Upload an image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param image the image to be uploaded
	 */
   void SmfGalleryPrivate::upload(SmfPicture* image)
	   {
		  //We need to pass Opcode and SmfProvider+SmfPicture serialized into bytearray 
		  
		  m_baseProvider = m_gallery->getProvider();
		  //serialize start
		  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
		  write<<*(m_baseProvider);
		  write<<*(image);
		  //serialize end
		  QString intfName(galleryInterface);
		  
		  //call private impl's send method
		  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPictureUpload);
		}

	/**
	 * Upload an list image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param images the list image to be uploaded
	 */
   void SmfGalleryPrivate::upload(SmfPictureList* images)
	   {
		  //We need to pass Opcode and SmfProvider+SmfPictureList serialized into bytearray 
		  
		  m_baseProvider = m_gallery->getProvider();
		  //serialize start
		  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
		  write<<*(m_baseProvider);
		  write<<*(images);
		  //serialize end
		  QString intfName(galleryInterface);
		  
		  //call private impl's send method
		  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPictureMultiUpload);
		}

  /**
   * Posts a comment for an image. uploadFinished() signal is emitted
   * with success of the post once comment is posted.
   * @param image Image to comment on
   * @param comment Comment to post
   */
   void SmfGalleryPrivate::postComment(SmfPicture image, SmfComment comment)
	   {
		  //We need to pass Opcode and SmfProvider+SmfPicture+SmfComment serialized into bytearray 
		  
		  m_baseProvider = m_gallery->getProvider();
		  //serialize start
		  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
		  write<<*(m_baseProvider);
		  write<<image;
		  write<<comment;
		  //serialize end
		  QString intfName(galleryInterface);
		  
		  //call private impl's send method
		  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPicturePostComment);
		}
   //from smfobserver
   void SmfGalleryPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error)
	   {
		writeLog("SmfPostProviderPrivate::resultsAvailable");
		
		//note:- "result" is serialized and we need to de-serialize it as per opcode
		//TODO:- order of serialization Error value followed by data
		
		QDataStream reader(&result,QIODevice::ReadOnly);
		
		//Now de-serialize it based on opcode
		switch(opcode)
			{
			case SmfPictureGetPicturesComplete:
				{
					if(m_picList)
						{
						delete m_picList;
						m_picList = NULL;
						}

					m_picList = new SmfPictureList;
					reader>>*(m_picList);
					writeLog("m_picList.count=");
					writeLog(QString::number(m_picList->count()));
//					foreach(SmfPicture pic,*m_picList)
//						{

//						}
					//not incorporating paging now
					SmfResultPage page;

					emit m_gallery->picturesAvailable(m_picList,error,page);
				
				}
				break;
			case SmfPictureUploadComplete:
			case SmfPicturePostCommentComplete:
				{
					bool success;
					reader>>success;
					writeLog("success=");
					if(success)
						writeLog("true");
					QList<bool> successList;
					successList.append(success);
					emit m_gallery->uploadFinished(successList);
				
				}
				break;
			case SmfPictureMultiUpload:
				{
				QList<bool> successList;
				reader>>successList;
				emit m_gallery->uploadFinished(successList);
			
			}
				break;
	//		default:
	//			//should panic?
	//			
			}
	   }
   
