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
 * Interface specification for music services
 *
 */


#ifndef SMFMUSICSERVICEPLUGIN_H_
#define SMFMUSICSERVICEPLUGIN_H_

#include <smfpluginbase.h>
#include <smfmusicprofile.h>
#include <smflocation.h>

/**
 * @ingroup smf_plugin_group
 * Interface specification for music services. This class provides basic 
 * functionality to allow application to search for a user or check for
 * user information etc.
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 */
class SmfMusicServicePlugin : public SmfPluginBase
	{
public:
	
	/**
	 * Destructor
	 */
	virtual ~SmfMusicServicePlugin( ) {}
	
	/**
	 * Method to get self profile information
	 * @param aRequest [out] The request data to be sent to network
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError userInfo( SmfPluginRequestData &aRequest ) = 0;
	
	/**
	 * Method to search information about other service users for a 
	 * particular place
	 * @param aRequest [out] The request data to be sent to network
	 * @param aPlace The place which is the search criteria
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError searchUser( SmfPluginRequestData &aRequest,
			const SmfLocation &aPlace,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Customised method for SmfMusicServicePlugin interface
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

Q_DECLARE_INTERFACE( SmfMusicServicePlugin, "org.symbian.smf.plugin.music.service/v1.0" );

#endif /* SMFMUSICSERVICEPLUGIN_H_ */
