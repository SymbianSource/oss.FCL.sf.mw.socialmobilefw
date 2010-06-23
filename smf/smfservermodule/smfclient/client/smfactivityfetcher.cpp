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
 * Description:
 * Represents activity in terms similar to standard activity stream http://activitystrea.ms/
 */

#include "smfactivityfetcher.h"
#include "smfactivityfetcher_p.h"

SmfActivityFetcher::SmfActivityFetcher(SmfProvider *baseprovider)
:m_baseProvider(baseprovider)
{
	  //creating private impl wrapper
	  m_private = new SmfActivityFetcherPrivate(this);
}

SmfActivityFetcher::~SmfActivityFetcher()
{
	
}

void SmfActivityFetcher::selfActivities(int pageNum, int perPage)
{
	m_private->selfActivities(pageNum,perPage);
}

SmfProvider *SmfActivityFetcher::getProvider()
{
	return m_baseProvider;
}

void SmfActivityFetcher::filtered(QList<SmfActivityObjectType> filters, int pageNum, int perPage)
{
	m_private->filtered(filters,pageNum,perPage);
}

void SmfActivityFetcher::friendsActivities(const SmfContact & aFriend, int pageNum, int perPage)
{
	m_private->friendsActivities(aFriend,pageNum,perPage);
}
