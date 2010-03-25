/**
 * @file	smfgalleryplugin.h
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
 * Interface specification for plugins that implement gallery related services
 */

#ifndef SMFGALLERYPLUGIN_H_
#define SMFGALLERYPLUGIN_H_

#include <smfproviderbase.h>
#include <smfpicture.h>
#include <smfcomment.h>

/**
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
class SmfGalleryPlugin : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfGalleryPlugin( QObject* aParent = 0 );
	
	/**
	 * Destructor
	 */
	~SmfGalleryPlugin( );
	
	/**
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	virtual SmfProviderBase* getProviderInfo( ) = 0;
	
	/**
	 * Method to get a list of pictures
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError pictures( SmfPluginRequestData *aRequest, 
			const int aPageNum = 0, 
			const int aItemsPerPage = 10) = 0;
	
	/**
	 * Method to get a description
	 * @param aImage The image abot which the description is required
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError description( const SmfPicture &aImage,
			SmfPluginRequestData *aRequest ) = 0;
	
	/**
	 * Method to upload a picture
	 * @param aImage The image to be uploaded
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError upload( const SmfPicture &aImage,
			SmfPluginRequestData *aRequest ) = 0;
	
	/**
	 * Method to upload a list of pictures
	 * @param aImages The list of images to be uploaded
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError upload( const QList<SmfPicture> &aImages,
			SmfPluginRequestData *aRequest ) = 0;
	
	/**
	 * Method to post comment on a picture
	 * is available
	 * @param aImage The image on which comment is to be posted
	 * @param aComment The comment to be posted
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError postComment( const SmfPicture &aImage, 
			const SmfComment &aComment,
			SmfPluginRequestData *aRequest ) = 0;
	
	/**
	 * Method to get the result for a network request.
	 * @param aTransportResult The result of transport operation
	 * @param aReply The QNetworkReply instance for the request
	 * @param aResult [out] An output parameter to the plugin manager.If the 
	 * return value is SmfSendRequestAgain, QVariant will be of type 
	 * SmfPluginRequestData. 
	 * If last operation was pictures(), aResult will be of type QList<SmfPicture>
	 * If last operation was description(), aResult will be of type QString
	 * If last operation was upload() or postComment(), aResult will be of 
	 * type bool
	 * @param aRetType [out] SmfPluginRetType
	 * @param aIsLastPage [out] true if this the last page, else false
	 * @return SmfPluginError 
	 */
	virtual SmfPluginError responseAvailable( 
			const SmfTransportResult aTransportResult, 
			QNetworkReply *aReply, 
			QVariant* aResult, 
			SmfPluginRetType aRetType,
			bool aIsLastPage) = 0;
		
	};

Q_DECLARE_INTERFACE( SmfGalleryPlugin, "org.symbian.smf.plugin.gallery/v1.0" );

#endif /* SMFGALLERYPLUGIN_H_ */
