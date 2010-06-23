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
#include "SmfClientQt.h"
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
 * Get the album listing asynchronously.
 * The albumsAvailable() signal is emitted with SmfPictureAlbumList once the albums have arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
 * @param names the subject or any keywords to be used to filter albums with that name
 * @param user the user whose albums are requested 
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
void SmfGalleryPrivate::albums(QStringList names, SmfContact* user, int pageNum, int perPage)
	  {
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_gallery->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  //TODO:- use different opcode incase post retreival is for other user
	  //serialize SmfContact if post retrieval is for other user
	  if(user)
		  {
		  //now serialize SmfContact 
		  m_xtraInfoSerialized.clear();
		  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
		  Xtrawrite<<*(user);
		  m_xtraInfoFlag = m_xtraInfoSerialized.size();
		  write<<m_xtraInfoFlag;
		  }
	  else
		  {
		  m_xtraInfoFlag = 0;
		  write<<m_xtraInfoFlag;
		  }	
	  QString intfName(galleryInterface);
	  int maxalloc = MaxSmfPictureAlbumSize*perPage;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPictureGetAlbums,maxalloc,m_xtraInfoSerialized);
	  }

  /**
 * Get the picture listing asynchronously.
 * The picturesAvailable() signal is emitted with SmfPictureList once the pictures have arrived.
 * When the list is big user can specify the page number and per page item data.
 * If not supplied by the user default values are used.
 * @param albums album(s) whose pictures are being requested
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
void SmfGalleryPrivate::pictures(SmfPictureAlbumList albums,int pageNum,int perPage)
	  {
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_gallery->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 1;
	  write<<m_pageInfoFlag;
	  write<<pageNum;
	  write<<perPage;
	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  Xtrawrite<<albums;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(galleryInterface);
	  int maxalloc = MaxSmfPictureSize*perPage;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPictureGetPictures,maxalloc,m_xtraInfoSerialized);
	  }

  /**
	 * Upload an image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param image the image to be uploaded
	 */
void SmfGalleryPrivate::upload(SmfPicture* image, SmfPictureAlbum* album)
	   {
	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_gallery->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;

	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  Xtrawrite<<image;
	  Xtrawrite<<album;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(galleryInterface);
	  int maxalloc = 100;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPictureUpload,maxalloc,m_xtraInfoSerialized);
	  }

	/**
	 * Upload an list image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param images the list image to be uploaded
	 */
void SmfGalleryPrivate::upload(SmfPictureList* images, SmfPictureAlbum* album)
	{

	  //We need to pass Opcode and SmfProvider serialized into bytearray 
	  m_baseProvider = m_gallery->getProvider();
	  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	  write<<*(m_baseProvider);
	  m_pageInfoFlag = 0;
	  write<<m_pageInfoFlag;

	  m_xtraInfoSerialized.clear();
	  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
	  Xtrawrite<<images;
	  Xtrawrite<<album;
	  m_xtraInfoFlag = m_xtraInfoSerialized.size();
	  write<<m_xtraInfoFlag;

	  QString intfName(galleryInterface);
	  int maxalloc = 1000;
	  //call private impl's send method
	  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPictureMultiUpload,maxalloc,m_xtraInfoSerialized);
	  
	}

  /**
   * Posts a comment for an image. uploadFinished() signal is emitted
   * with success of the post once comment is posted.
   * @param image Image to comment on
   * @param comment Comment to post
   */
   void SmfGalleryPrivate::postComment(SmfPicture image, SmfComment comment)
	   {

		  //We need to pass Opcode and SmfProvider serialized into bytearray 
		  m_baseProvider = m_gallery->getProvider();
		  QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
		  write<<*(m_baseProvider);
		  m_pageInfoFlag = 0;
		  write<<m_pageInfoFlag;

		  m_xtraInfoSerialized.clear();
		  QDataStream Xtrawrite(&m_xtraInfoSerialized,QIODevice::WriteOnly);
		  Xtrawrite<<image;
		  Xtrawrite<<comment;
		  m_xtraInfoFlag = m_xtraInfoSerialized.size();
		  write<<m_xtraInfoFlag;

		  QString intfName(galleryInterface);
		  int maxalloc = 100;
		  //call private impl's send method
		  m_SmfClientPrivate->sendRequest(m_serializedDataToServer,intfName,SmfPicturePostComment,maxalloc,m_xtraInfoSerialized);
		  
		}
   //from smfobserver
   void SmfGalleryPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error)
	   {
	writeLog("SmfPostProviderPrivate::resultsAvailable");
	
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//Order of serialization Error value followed by data
	
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
					//not incorporating paging now
					SmfResultPage page;
		
					emit m_gallery->picturesAvailable(m_picList,error,page);
				
				}
				break;
			case SmfPictureUpload:
			case SmfPicturePostComment:
				{
					
					int errInt;
					reader>>errInt;
					writeLog("success=");
					QList<SmfError> successList;
					SmfError success = (SmfError)errInt; 
					successList.append(success);
					emit m_gallery->uploadFinished(successList);
				
				}
				break;
			case SmfPictureMultiUpload:
				{
				QList<int> successListInt;
				QList<SmfError> successList;
				reader>>successListInt;
				foreach(int success,successListInt)
					{
					successList.append((SmfError)success);
					}
				emit m_gallery->uploadFinished(successList);
			
				}
				break;
				default:
					User::Panic(_L("Gallerprivate="),opcode);
					
		}
}
   
