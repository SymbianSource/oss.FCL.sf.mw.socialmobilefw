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
/**
 * Private Qt wrapper
 * Provided to accomodate diff facilities in diff platforms.
 * Uses Symbian client-server for Symbian
 * QDbus for linux
 * QLocalserver for rest of the platforms
 * Note:- only symbian client-server impl is provided at the moment
 **/
#ifndef SMFGALLERY_P_H
#define SMFGALLERY_P_H

#include "smfprovider.h"
#include "SmfClientGlobal.h"
#include "smfGlobal.h"
#include "SmfObserver.h"

class SmfProvider;


#include <QObject>
#include <QDateTime>
#include <QStringList>

#include "SmfObserver.h"
#include "smfgallery.h"
#ifdef Q_OS_SYMBIAN
class CSmfClientSymbian;
#else
class SmfClientQt;
#endif
class SmfGalleryPrivate : public smfObserver
{
  Q_OBJECT

public:

  SmfGalleryPrivate(SmfGallery* gallery=0);
  ~SmfGalleryPrivate();
  
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
	void pictures(SmfPictureAlbumList albums, int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE);

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
	
	/** from smfobserver */
   void resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error);
	
	
private:
	  SmfProvider* m_baseProvider;
	  //serialized byte array of provider+other info to be sent to the server
	  //The order:- SmfProvider then params in order of their appearance in fn
	  QByteArray m_serializedDataToServer;
	  //serialized xtra info, order of serialization follows order of param
	  QByteArray m_xtraInfoSerialized;
	#ifdef Q_OS_SYMBIAN
	  CSmfClientSymbian* m_SmfClientPrivate;
	  friend class CSmfClientSymbian;
	#else
	  SmfClientQt* m_SmfClientPrivate;
	  friend class SmfClientQt;
	#endif
	  bool m_connected;
	  SmfGallery* m_gallery;
	  SmfPictureList* m_picList;
	  int m_xtraInfoFlag;
	  int m_pageInfoFlag;
	 void writeLog(QString log) const{};

};
#endif






