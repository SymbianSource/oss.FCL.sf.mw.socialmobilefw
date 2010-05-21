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

#include "smfprovider.h"
#include "smfclientglobal.h"
#include "smfglobal.h"
#include "smfobserver.h"

class SmfProvider;


#include <QObject>
#include <QDateTime>
#include <QStringList>

#include "smfobserver.h"
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
   * Get the picture listing asynchronously.
   * The picturesAvailable() signal is emitted with SmfPictureList once the pictures have arrived.
   * When the list is big user can specify the page number and per page item data.
   * If not supplied by the user default values are used.
   * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
   * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE
   */
  void pictures(int pageNum=SMF_FIRST_PAGE,int perPage=SMF_ITEMS_PER_PAGE);

  /**
   * Returns a user title/caption for the picture
   */
  QString description(SmfPicture& picture); // A user title or caption, maybe?

  /**
	 * Upload an image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param image the image to be uploaded
	 */
   void upload(SmfPicture* image) ;

	/**
	 * Upload an list image.Implemented as slot to connect to UI controls more easily
	 * uploadFinished() signal is emitted with the success value of the upload
	 * @param images the list image to be uploaded
	 */
   void upload(SmfPictureList* images) ;

  /**
   * Posts a comment for an image. uploadFinished() signal is emitted
   * with success of the post once comment is posted.
   * @param image Image to comment on
   * @param comment Comment to post
   */
   void postComment(SmfPicture image, SmfComment comment) ;
   //from smfobserver
   void resultsAvailable(QByteArray result,SmfRequestTypeID opcode,SmfError error);
private:
	  SmfProvider* m_baseProvider;
	  //serialized byte array of provider+other info to be sent to the server
	  //The order:- SmfProvider then params in order of their appearance in fn
	  QByteArray m_serializedDataToServer;
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
	 void writeLog(QString log) const{};

};







