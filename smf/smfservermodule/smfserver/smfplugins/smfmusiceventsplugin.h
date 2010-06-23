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
 * Interface specification for music events
 * Note: This class has dependencies on QtMobility project
 *
 */


#ifndef SMFMUSICEVENTSPLUGIN_H_
#define SMFMUSICEVENTSPLUGIN_H_

#include <smfpluginbase.h>
#include <smfevent.h>
#include <smflocation.h>
#include <qgeopositioninfo.h> // Qt mobility class

using namespace QtMobility;

/**
 * @ingroup smf_plugin_group
 * Interface specification for music events
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in.
 * 
 * Note: This class has dependencies on QtMobility project
 */
class SmfMusicEventsPlugin : public SmfPluginBase
	{
public:
	
	/**
	 * Destructor
	 */
	virtual ~SmfMusicEventsPlugin( ) {}
	
	/**
	 * Method to get the events based on location
	 * @param aRequest [out] The request data to be sent to network
	 * @param aLocation Location of the event
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError events( SmfPluginRequestData &aRequest,
			const QGeoPositionInfo &aLocation,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to get the venues based on location
	 * @param aRequest [out] The request data to be sent to network
	 * @param aLocation Location of the venue
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError venues( SmfPluginRequestData &aRequest,
			const QGeoPositionInfo &aLocation,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to get the events based on venues
	 * @param aRequest [out] The request data to be sent to network
	 * @param aVenue Venue of the event
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError events( SmfPluginRequestData &aRequest,
			const SmfLocation &aVenue,
			const int aPageNum = SMF_FIRST_PAGE, 
			const int aItemsPerPage = SMF_ITEMS_PER_PAGE ) = 0;
	
	/**
	 * Method to post events
	 * @param aRequest [out] The request data to be sent to network
	 * @param aEventList The list of events to be posted
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError postEvents( SmfPluginRequestData &aRequest,
			const QList<SmfEvent> &aEventList ) = 0;
	
	/**
	 * Customised method for SmfMusicEventsPlugin interface
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

Q_DECLARE_INTERFACE( SmfMusicEventsPlugin, "org.symbian.smf.plugin.music.events/v0.2" );

#endif /* SMFMUSICEVENTSPLUGIN_H_ */
