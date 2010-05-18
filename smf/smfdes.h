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
 * Intro Page
 *
 * @ref smf_high_level_design
 */

/**
 * @mainpage SMF (Social Mobile Framework Interfaces)
 * @defgroup smf_high_level_design SMF high level design
 *
 * Social Mobile Framework is a application level framework which allows Qt Applications
 * use the common web APIs to perform social web activities.
 *
 * Product name is "SMF"
 *
 * For details please refer to http://developer.symbian.org/wiki/index.php/Social_Mobile_Framework
 *
 * @section smf_interfaces SMF Interfaces
 *
 * There are two sets of interfaces -
 * 		@subsection smf_client_interfaces Client Interfaces
 * 				- Client Applications (e.g. Media Player, Contacts, Game) uses these APIs to access SMF. see @ref smf_client_group
 *
 * 		@subsection smf_plugin_interfaces Plugin Interfaces
 * 				- Service Providers (e.g. Facebook, Flickr, last.fm) implements these APIs to provide service to SMF - see @ref smf_plugin_group
 *
 * Currently, @ref smf_client_interfaces and @ref smf_plugin_interfaces are defined for remote contacts, remote
 * image gallery and remote music services. There are common classes defined for holding data accross clients
 * and plugins, present in inc\common, see - @ref smf_common_group
 *
 *
 *		@section smf_client_usage Client Usage Example
 *		- This shows how to use the SMF client APIs in applications, e.g. Client can define suitable Models using lists returned by these APIs
 *				- how to fetch a list of contacts from a remote server e.g. Facebook, see @ref MyApplication::displayFriends()
 *				- how to get a @ref QContact from a SmfContacts see @ref MyApplication::showlist()
 *				- how to get a list of images from a remote gallery e.g. Flickr.com, see @ref MyApplication::displayGallery()
 *				- how to post privately to a contact see @ref MyApplication::postUpdate()
 *				- how to tell others about the music you are currently listen to, see @ref MyApplication::updateCurrentPlaying()
 *				- how to list all the activities that has happened recently, see @ref MyApplication::getActivities() and @ref MyApplication::showActivities()
 *
 *		@section smf_sample_plugin Sample Plugin From Service provider
 *		- This shows how to write a plugin for extending SMF service. Authentication Application also needs to be provided for these plugins.
 *				- how to create request to be sent to a remote server e.g. Flickr.com, see @ref SamplePlugin::pictures()
 *				- how to implement posting of comment on a picture to Flickr.com,  @ref SamplePlugin::postComment()
 *				- how to parse the response that arrived from Flickr.com, see @ref SamplePlugin::responseAvailable()
 *				- how to use JSON parsing from SmfPluginUtil, see @ref SamplePlugin::responseAvailable()
 *
 *		@section smf_custom_api Custom API in SMF
 *		- SMF strives to provide service API for generic "common" social operations. However this doesn't mean that developers have to
 *		use other propriatery APIs to access service specific "differentiating" features. Service Providers can provide plugins which support
 *		APIs like @ref SmfGalleryPlugin::customRequest(). A separate library would also be provided by service providers to help SMF clients use this custom
 *		API like @ref SmfGallery::customRequest().
 *
 *
 *
 *		@section smf_dependency Dependency
 *		- SMF uses QtMobility for mainly contacts information, see QtMobility project.
 *		- SMF uses QJson as utility library, courtesy Flavio Castelli , see http://gitorious.org/qjson/qjson
 *
 *		@section smf_notes Notes:
 *		- SMF defines the social activities with inspiration from http://activitystrea.ms/
 *		- Other such open standards would soon find their usage in SMF, one such candidate is http://xspf.org/
 *
   */
/**
 * @defgroup smf_common_group Common Data Classes
 */
/**
 * @defgroup smf_client_group Client Classes
 */
/**
 * @defgroup smf_plugin_group Plugin Classes
 */
