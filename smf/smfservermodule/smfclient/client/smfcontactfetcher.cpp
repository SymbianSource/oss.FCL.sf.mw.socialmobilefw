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
 * The SmfContactFetcher class is for fetching SmfContact related info
 *
 */

#include "smfcontactfetcher.h"
#include "smfcontactfetcher_p.h"

SmfContactFetcher::SmfContactFetcher(SmfProvider* baseProvider)
  {
  m_baseProvider = baseProvider;
  //creating private impl wrapper
  m_private = new SmfContactFetcherPrivate(this);
  }
SmfContactFetcher::SmfContactFetcher(SmfProvider* baseProvider, SmfContact* contact)
{
	m_baseProvider = baseProvider;
	m_frndContact = contact;
	//creating private impl wrapper
	m_private = new SmfContactFetcherPrivate(this);
}
SmfContactFetcher::~SmfContactFetcher()
{
	if(m_baseProvider)
	{
		delete m_baseProvider;
		m_baseProvider = NULL;
	}
	if(m_frndContact)
	{
		delete m_frndContact;
		m_frndContact = NULL;
	}
	//TODO:- Request to unload the plugin
}
bool  SmfContactFetcher::friends(int pageNum,int perPage)
{
m_private->friends(pageNum,perPage);
}
bool SmfContactFetcher::followers(int pageNum,int perPage)
{
	m_private->followers(pageNum,perPage);
}
void  SmfContactFetcher::search(SmfContact* contact,int pageNum,int perPage) 
{
	m_private->search(contact,pageNum,perPage);
}
bool  SmfContactFetcher::searchNear(SmfLocation* location,SmfLocationSearchBoundary proximity,int pageNum,int perPage) 
{
	//To be implemented
}
bool SmfContactFetcher::groups(int pageNum,int perPage) 
{
	m_private->groups(pageNum,perPage);
}
bool  SmfContactFetcher::searchInGroup(SmfGroup group,int pageNum,int perPage) 
{
	m_private->searchInGroup(group,pageNum,perPage);
}
void SmfContactFetcher::customRequest(const int& operationId,QByteArray* customData)
{
	//To be implemented
}
SmfProvider*  SmfContactFetcher::getProvider() 
{
	return m_baseProvider;
}
