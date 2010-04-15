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
#include <qtcontacts.h>
#include <smfevent.h>
#include <smfplace.h>

// Forward declaration
class SmfPluginManagerUtil;

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
	Q_OBJECT
public:
	/**
	 * Constructor with default argument
	 * @param aUtil The SmfPluginManagerUtil instance. The plugins can 
	 * call the method getAuthKeys() of this class, with its pluginID to 
	 * get the OAuth keys, keys are returned only if this plugin is 
	 * authorised by Smf framework
	 */
	SmfMusicEventsPlugin( SmfPluginManagerUtil* aUtil );
	
	/**
	 * Destructor
	 */
	~SmfMusicEventsPlugin( );
	
	/**
	 * Method to get the events based on location
	 * @param aRequest [out] The request data to be sent to network
	 * @param aLocation Location of the event
	 * @param aPageNum The page to be extracted
	 * @param aItemsPerPage Number of items per page
	 * @return SmfPluginError Plugin error if any, else SmfPluginErrNone
	 */
	virtual SmfPluginError events( SmfPluginRequestData &aRequest,
			const QtMobility::QContactGeolocation &aLocation,
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
			const QtMobility::QContactGeolocation &aLocation,
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
			const SmfPlace &aVenue,
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
	
	};

Q_DECLARE_INTERFACE( SmfMusicEventsPlugin, "org.symbian.smf.plugin.music.events/v1.0" );

#endif /* SMFMUSICEVENTSPLUGIN_H_ */
