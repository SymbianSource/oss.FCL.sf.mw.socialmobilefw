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
 * Interface specification for plugins that implement gallery related services
 *
 */

#ifndef SMFGALLERYPLUGIN_H_
#define SMFGALLERYPLUGIN_H_

#include <smfpluginbase.h>
#include <smfpicture.h>
#include <smfcomment.h>
#include <smfpicturealbum.h>

/**
 * @ingroup smf_plugin_group
 * Interface specification for plugins that implement gallery related services
 * like getting pictures, their description, uploading, posting comments 
 * on pictures etc
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 *
 * A sample call flow between SmfPluginMgr and  SmfGalleryPlugin is shown here
 * 
 * @msc
   hscale = "2";  
  SmfServer,SmfTransportMgr,SmfPluginMgr,SmfGalleryPlugin;
  ...;
  SmfPluginMgr=>SmfTransportMgr [ label = "connect(SIGNAL(finished()), SLOT(replyFinished()))" ] ;
  ...;
  SmfServer=> SmfPluginMgr[ label = "loadAndCheckPlugin()" ] ;
  SmfPluginMgr=>SmfPluginMgr [ label = "SmfGalleryPlugin smfglpl= load()" ] ;
  SmfPluginMgr=>SmfGalleryPlugin [ label = "QNetworkRequest  req = smfglpl.pictures()" ] ;
  SmfPluginMgr=>SmfTransportMgr [ label = "nwmgr.get( req )" ] ;
  ...;
  SmfTransportMgr=>SmfPluginMgr [ label = "replyFinished( reply )" ] ;
  SmfPluginMgr=>SmfGalleryPlugin [ label = "status = responseAvailable( reply, &data )" ] ;
  SmfPluginMgr=>SmfServer [ label = "if(status ==complete) dataStream <<  data;" ] ;
  @endmsc
 * 
 */
class SmfGalleryPlugin : public SmfPluginBase
	{
public:
	/**
	 * Destructor
	 */
	virtual ~SmfGalleryPlugin( ) {}

	/**
	 * Method to get a list of albums
	 * @param aRequest [out] The request data to be sent to network
	 * @param aNames The subject or any keywords to be used to filter albums with that name
	 * @param aUser The user whose albums are requested
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError albums( SmfPluginRequestData &aRequest, 
			const QStringList &aNames, 
			const SmfContact *aUser, 
			const int aPageNum = SMF_FIRST_PAGE,
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to get a list of pictures
	 * @param aRequest [out] The request data to be sent to network
	 * @param aAlbums The album(s) whose pictures are being requested
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError pictures( SmfPluginRequestData &aRequest, 
			const SmfPictureAlbumList &aAlbums, 
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to get a description
	 * @param aRequest [out] The request data to be sent to network
	 * @param aImage The image abot which the description is required
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError description( SmfPluginRequestData &aRequest,
			const SmfPicture &aImage ) = 0;
	
	/**
	 * Method to upload a picture
	 * @param aRequest [out] The request data to be sent to network
	 * @param aImage The image to be uploaded
	 * @param aAlbum the optional destination album name
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError upload( SmfPluginRequestData &aRequest,
			const SmfPicture &aImage,
			const SmfPictureAlbum* aAlbum = NULL ) = 0;
	
	/**
	 * Method to upload a list of pictures
	 * @param aRequest [out] The request data to be sent to network
	 * @param aImages The list of images to be uploaded
	 * @param aAlbum the optional destination album name
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError upload( SmfPluginRequestData &aRequest,
			const QList<SmfPicture> &aImages, 
			const SmfPictureAlbum* aAlbum = NULL ) = 0;
	
	/**
	 * Method to post comment on a picture is available
	 * @param aRequest [out] The request data to be sent to network
	 * @param aImage The image on which comment is to be posted
	 * @param aComment The comment to be posted
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError postComment( SmfPluginRequestData &aRequest,
			const SmfPicture &aImage, 
			const SmfComment &aComment ) = 0;
		
	/**
	 * Customised method for SmfGalleryPlugin interface
	 * @param aRequest [out] The request data to be sent to network
	 * @param aOperation The operation type (should be known between 
	 * the client interface and the plugin)
	 * @param aData The data required to form the request (The type 
	 * of data should be known between client and the plugin)
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError customRequest( SmfPluginRequestData &aRequest, 
			const int &aOperation, QByteArray *aData ) = 0;
		
	};

Q_DECLARE_INTERFACE( SmfGalleryPlugin, "org.symbian.smf.plugin.gallery/v0.2" );

#endif /* SMFGALLERYPLUGIN_H_ */
