/**
 * @file	smfmusicprofile.h
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
 * The music profile class represents a user's profile in music site
 */

#ifndef SMFMUSICPROFILE_H_
#define SMFMUSICPROFILE_H_

#include <qdatastream.h>
#include "SmfClientGlobal.h"

#include "smfcontact.h"
class SmfTrackInfo;
class SmfEventsList;
typedef QList<SmfTrackInfo> SmfTrackInfoList; 
/**
 * User profile containing music usage and interest info, extends SmfContact.
 * Consists of,-
 * 1. Conatct info
 * 2. Music usage info as SmfTrackInfoList
 * 3. Interest info as SmfTrackInfoList
 * 4. Events SmfEventsList
 */
class SMFCLIENT_EXPORT SmfMusicProfile : public SmfContact
	{
	  Q_OBJECT

	public:
	  // Seeing as this is a plug-in implementation, these will realistically
	  // be generated by SMF factory of some kind
	  SmfMusicProfile(QObject* parent = 0);
	  ~SmfMusicProfile();
	  
	  /**
	   * Gets the music usage info of the user as list of SmfTrackInfo
	   */
	  SmfTrackInfoList* getMusicUsageInfo();
	  
	  /**
	   * Gets the music interest info of the user as list of SmfTrackInfo
	   */
	  SmfTrackInfoList* getInterestInfo();
	  
	  /**
	   * Gets the user events as list of SmfEvents
	   */
	  SmfEventsList* getUserEvents();
	  
	  /**
	   * Sets the music usage info of the user as list of SmfTrackInfo
	   */
	  void setMusicUsageInfo(SmfTrackInfoList* usage);
	  
	  /**
	   * Gets the music interest info of the user as list of SmfTrackInfo
	   */
	  void setInterestInfo(SmfTrackInfoList* interest);

	private:
	  SmfTrackInfoList* usage;
	  SmfTrackInfoList* interest;
	  
	};
/**
* Externalization
*/
QDataStream &operator<<(QDataStream &, const SmfMusicProfile&);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfMusicProfile&);
#endif /* SMFMUSICPROFILE_H_ */
