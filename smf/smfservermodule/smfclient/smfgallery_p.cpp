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

#include <qdebug.h>
#include <smfprovider.h>
#include <smfcontact.h>
#include <smfcomment.h>

#include "smfgallery.h"
#include "smfgallery_p.h"
#ifdef Q_OS_SYMBIAN
	#include "smfclientsymbian.h"
#else
	#include "SmfClientQt.h"
#endif

SmfGalleryPrivate::SmfGalleryPrivate(SmfGallery* gallery)
		: m_gallery(gallery)
	{
#ifdef Q_OS_SYMBIAN
	//private impl for symbian
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
 * Get the album listing asynchronously. The albumsAvailable() signal is 
 * emitted with SmfPictureAlbumList once the albums have arrived.
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
	SmfProvider* m_baseProvider = m_gallery->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<names;
	if(user)
		{
		write<<m_argFlag;
		write<<*user;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	m_argFlag = 1;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
	
	QString intfName(galleryInterface);
	int maxalloc = MaxSmfPictureAlbumSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfPictureGetAlbums, maxalloc);
	}


/**
 * Get the picture listing asynchronously. The picturesAvailable() signal is 
 * emitted with SmfPictureList once the pictures have arrived.
 * When the list is big user can specify the page number and per page item data.
 * If not supplied by the user default values are used.
 * @param albums album(s) whose pictures are being requested
 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
 * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
 */
void SmfGalleryPrivate::pictures(SmfPictureAlbumList &albums,int pageNum,int perPage)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_gallery->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<albums;
	write<<m_argFlag;
	write<<pageNum;
	write<<m_argFlag;
	write<<perPage;
		
	QString intfName(galleryInterface);
	int maxalloc = MaxSmfPictureSize*perPage;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
		  SmfPictureGetPictures, maxalloc);
	}

/**
 * Returns a user title/caption for the picture
 */
void SmfGalleryPrivate::description ( SmfPicture& picture )
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_gallery->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<picture;
	
	QString intfName(galleryInterface);
	// ToDo :- Setting max limit of description string as 1000
	int maxalloc = 1000;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
		  SmfPictureDescription, maxalloc);
	}

/**
 * Upload an image.Implemented as slot to connect to UI controls more easily
 * uploadFinished() signal is emitted with the success value of the upload
 * @param image the image to be uploaded
 * @param album the optional destination album name 
 */
void SmfGalleryPrivate::upload(SmfPicture* image, SmfPictureAlbum* album)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_gallery->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	if(image)
		{
		m_argFlag = 1;
		write<<m_argFlag;
		write<<*image;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}

	if(album)
		{
		m_argFlag = 1;
		write<<m_argFlag;
		write<<*album;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
	
	QString intfName(galleryInterface);
	int maxalloc = 100; // ToDo:- limit set as 100
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
		  SmfPictureUpload, maxalloc);
	}

/**
 * Upload an list image.Implemented as slot to connect to UI controls more easily
 * uploadFinished() signal is emitted with the success value of the upload
 * @param images the list image to be uploaded
 * @param album the optional destination album name 
 */
void SmfGalleryPrivate::upload(SmfPictureList* images, SmfPictureAlbum* album)
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray
	m_serializedDataToServer.clear();
	SmfProvider* m_baseProvider = m_gallery->getProvider();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	if(images)
		{
		m_argFlag = 1;
		write<<m_argFlag;
		write<<*images;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}

	if(album)
		{
		m_argFlag = 1;
		write<<m_argFlag;
		write<<*album;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}
		
	QString intfName(galleryInterface);
	int maxalloc = 1000; // ToDo:- limit set as 1000
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfPictureMultiUpload, maxalloc);
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
	SmfProvider* m_baseProvider = m_gallery->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<image;
	write<<m_argFlag;
	write<<comment;
		
	QString intfName(galleryInterface);
	int maxalloc = 100; // ToDo:- limit set as 100
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfPicturePostComment, maxalloc);
	}

/**
 * Request for a custom operation.
 * @param operationId OperationId
 * @param customData Custom data to be sent
 * Note:-Interpretation of operationId and customData is upto the concerned
 * plugin and client application. service provider should provide some
 * serializing-deserializing utilities for these custom data
 */
void SmfGalleryPrivate::customRequest ( const int& operationId, QByteArray* customData )
	{
	//We need to pass Opcode and SmfProvider serialized into bytearray 
	SmfProvider* m_baseProvider = m_gallery->getProvider();
	m_serializedDataToServer.clear();
	QDataStream write(&m_serializedDataToServer,QIODevice::WriteOnly);
	write<<*m_baseProvider;
	m_argFlag = 1;
	write<<m_argFlag;
	write<<operationId;
	if(customData)
		{
		write<<m_argFlag;
		write<<*customData;
		}
	else
		{
		m_argFlag = 0;
		write<<m_argFlag;
		}

	QString intfName(galleryInterface);
	//ToDo:- How much size to allocate for custo data? keeping MaxSmfPictureSize for now
	int maxAlloc = MaxSmfPictureSize;
	
	//call private impl's send method
	m_SmfClientPrivate->sendRequest(m_serializedDataToServer, intfName,
			SmfPictureCustomRequest, maxAlloc);
	}


/**
 * To notify availibility of asynchronous requests.
 * @param result Requested result, before using must check error param.
 * @param opcode Requested opcode, for which the result has arrived.
 * @param error Error
 */
void SmfGalleryPrivate::resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error)
	{
	qDebug()<<"SmfGalleryPrivate::resultsAvailable";
	qDebug()<<"opcode = "<<opcode;
	
	//note:- "result" is serialized and we need to de-serialize it as per opcode
	//Order of serialization Error value followed by data
	
	QDataStream reader(&result,QIODevice::ReadOnly);
	
	//Now de-serialize it based on opcode
	switch(opcode)
		{
		case SmfPictureGetAlbums:
			{
			qDebug()<<"fOR ALBUMS LIST";
			SmfPictureAlbumList* m_albums = new SmfPictureAlbumList;
			reader>>*(m_albums);
			qDebug()<<" Albums count = "<<m_albums->count();
			
			SmfResultPage page; // ToDo :-
			
			emit m_gallery->albumsAvailable(m_albums,error,page);
			break;
			}

		case SmfPictureGetPictures:
			{
			SmfPictureList* m_picList = new SmfPictureList;
			reader>>*(m_picList);
			qDebug()<<"m_picList.count = "<<m_picList->count();
			
			//ToDo:- not incorporating paging now
			SmfResultPage page;
			
			emit m_gallery->picturesAvailable(m_picList,error,page);
			break;
			}
			
		case SmfPictureDescription:
			{
			QString str;
			reader>>str;
			qDebug()<<"Pic desc = "<<str;
			
			emit m_gallery->descriptionAvailable(str,error);
			break;
			}
			
		case SmfPictureUpload:
		case SmfPicturePostComment:
			{
			int errInt;
			reader>>errInt;
			qDebug()<<"success = "<<errInt;
			QList<SmfError> successList;
			SmfError success = (SmfError)errInt; 
			successList.append(success);
			emit m_gallery->uploadFinished(successList);
			break;
			}
			
		case SmfPictureMultiUpload:
			{
			QList<int> successListInt;
			QList<SmfError> successList;
			reader>>successListInt;
			foreach(int success,successListInt)
				successList.append((SmfError)success);

			emit m_gallery->uploadFinished(successList);
			break;
			}
			
		case SmfPictureCustomRequest:
   			{
   			int operationId;
   			QByteArray *data = new QByteArray;
   			reader>>operationId;
   			reader>>*data;
   			qDebug()<<"operationId = "<<operationId;
   			qDebug()<<"data size = "<<data->size();
   			emit m_gallery->customDataAvailable(operationId, data);
   			break;
   			}

		default:
			User::Panic(_L("Gallery Private = "),opcode);
		}
	}
   
