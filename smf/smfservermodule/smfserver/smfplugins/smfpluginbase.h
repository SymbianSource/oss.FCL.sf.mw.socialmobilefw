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
 * The SmfPluginBase class is the base class for all plugins.
 *
 */

#ifndef SMFPLUGINBASE_H_
#define SMFPLUGINBASE_H_

#include <smfproviderbase.h>
#include <smfglobal.h>

class SmfPluginUtil;

/**
 * @ingroup smf_plugin_group
 * The SmfPluginBase class is the base class for all plugins 
 */
class SmfPluginBase
	{
public:
	/**
	 * Destructor
	 */
	virtual ~SmfPluginBase( ) {}
	
	/**
	 * The first method to be called in the plugin that implements this interface.
	 * If this method is not called, plugin may not behave as expected.
	 * Plugins are expected to save the aUtil handle and use as and when required.
	 * @param aUtil The instance of SmfPluginUtil
	 */
	virtual void initialize( SmfPluginUtil *aUtil ) = 0;

	/**
	 * Method to get the provider information
	 * @return Instance of SmfProviderBase
	 */
	virtual SmfProviderBase* getProviderInfo( ) = 0;
	
	/**
	 * Method to get the result for a network request.
	 * @param aTransportResult The result of transport operation
	 * @param aReply The QByteArray instance containing the network response.
	 * The plugins should delete this instance once they have read the 
	 * data from it.
	 * @param aResult [out] An output parameter to the plugin manager.If the 
	 * return value is SmfSendRequestAgain, QVariant will be of type 
	 * SmfPluginRequestData.
	 * 
	 * For SmfContactFetcherPlugin: If last operation was friends() or followers() 
	 * or search() or searchInGroup() or searchNear(), aResult will be of type 
	 * QList<SmfContact>. If last operation was groups(), aResult will be of 
	 * type QList<SmfGroup>
	 * For SmfPostProviderPlugin: If last operation was retrieve(), aResult will be 
	 * of type QList<SmfPost>. If last operation was post() or updatePost() or 
	 * postDirected() or commentOnAPost() or postAppearence() or sharePost(), 
	 * aResult will be of type bool
	 * For SmfLyricsServicePlugin: If last operation was lyrics(), aResult will 
	 * be of type QList<SmfLyrics>. If last operation was subtitles(), aResult will 
	 * be of type QList<SmfSubtitle>.
	 * For SmfMusicEventsPlugin: If last operation was events(), aResult will 
	 * be of type QList<SmfEvent>. If last operation was venues(), aResult 
	 * will be of type QList<SmfLocation>. If last operation was postEvents(), 
	 * aResult will be of type bool
	 * For SmfMusicSearchPlugin: If last operation was recommendations() or 
	 * tracks() or trackInfo(), aResult will be of type QList<SmfTrackInfo>. 
	 * If last operation was stores(), aResult will be of type 
	 * QList<SmfProvider>. If last operation was postCurrentPlaying(), 
	 * aResult will be of type bool.
	 * For SmfMusicServicePlugin: If last operation was userInfo(), aResult 
	 * will be of type SmfMusicProfile. If last operation was searchUser(), 
	 * aResult will be of type QList<SmfMusicProfile>.
	 * For SmfPlaylistServicePlugin: If last operation was playlists() or 
	 * playlistsOf(), aResult will be of type QList<SmfPlaylist>. If last 
	 * operation was addToPlaylist() or postCurrentPlayingPlaylist(), 
	 * aResult will be of type bool.
	 * For SmfGalleryPlugin: If last operation was pictures(), aResult will 
	 * be of type QList<SmfPicture>. If last operation was description(), 
	 * aResult will be of type QString. If last operation was upload() or 
	 * postComment(), aResult will be of type bool.
	 * 
	 * @param aRetType [out] SmfPluginRetType
	 * @param aPageResult [out] The SmfResultPage structure variable
	 */
	virtual SmfPluginError responseAvailable( 
			const SmfTransportResult &aTransportResult, 
			QByteArray *aReply, 
			QVariant* aResult, 
			SmfPluginRetType &aRetType,
			SmfResultPage &aPageResult ) = 0;
	
	};

Q_DECLARE_INTERFACE( SmfPluginBase, "org.symbian.smf.plugin.smfpluginbase/v1.0" );

#endif /* SMFPLUGINBASE_H_ */
