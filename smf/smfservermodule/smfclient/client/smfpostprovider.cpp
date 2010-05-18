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
 * The SmfEvent class represents an event
 *
 */

#include "smfpostprovider.h"
#include <QVector>
#include <QTextFormat>

	/**
	 * Constructs SmfPostProvider.
	 * @param baseProvider The base provider info
	 * Seeing as this is a plug-in implementation, these will realistically
	 * be generated by SMF factory of some kind
	 */
	SmfPostProvider::SmfPostProvider(SmfProvider* baseProvider)
		{
		  m_baseProvider = baseProvider;
		  //creating private impl wrapper
		  m_private = new SmfPostProviderPrivate(this);
		}
	SmfPostProvider::~SmfPostProvider()
		{
		
		}
	/**
	 * returns maximum no of chars (unicode) that service provider can post without truncation
	 * negative value means no limit
	 */
	qint32 SmfPostProvider::maxCharsInPost() const
	{ 
	
	}
	/**
	 * returns maximum no of items that can be returned in a single query to getPosts
	 * negative value means feature not supported.
	 */
	qint32 SmfPostProvider::maxItems() const
			{ 
		
			}

	/**
	 * returns all the formatting of posts that this service provider supports.
	 * May return 0 items to mean only QString is supported.
	 */
	QVector<QTextFormat> SmfPostProvider::supportedFormats () const
			{
		
			}
	
	/**
	 * returns whether this SP supports Appearence @see SmfAppearenceInfo
	 */
	bool SmfPostProvider::supportsAppearence () const
			{ 
		
			}

	/**
	 * Gets the posts asynchronously. The signal postsAvailable()with SmfPostList is emitted
	 * once the post lists are available
	 * @param user user's contact in this SP, omit for self contact
	 * @param pageNum Page number to download, SMF_FIRST_PAGE denotes fresh query.
     * @param perPage Item per page, default is SMF_ITEMS_PER_PAGE 
	 * @see postsAvailable()
	 */
	void SmfPostProvider::posts(SmfContact* user ,int pageNum,int perPage)
		{ 
		m_private->posts(user,pageNum,perPage);
		}
	
	/**
	 * Updates a post to own area, the success of the post can be checked with signal
	 * updatePostFinished() signal
	 * @param postData data to be posted
	 * @param location location data
	 */
	void SmfPostProvider::post(SmfPost& postData,SmfLocation& location) 
		{
		m_private->post(postData,location);
		} 
	/**
	 * Updates the last post to own area with new data, the success of the post can be checked with signal
	 * updatePostFinished() signal
	 * @param postData edited/new data to be posted
	 * @param location location data
	 */
	void SmfPostProvider::updatePost(SmfPost& postData)
		{ 
		m_private->updatePost(postData);
		} 
	
	/**
	 * Updates a post to a particular Smf contact. the success of the post can be checked with signal
	 * updatePostFinished() signal.
	 * @param postData data to be posted
	 * @param contact contact to which the post is to be directed
	 * @param location location data
	 */
	void SmfPostProvider::postDirected(SmfPost& postData,SmfContact& contact,SmfLocation* location)
		{ 
		m_private->postDirected(postData,contact,location);
		}
	
	/**
	 * Posts appearance info of the user.e.g. appear offline, busy, do-not-disturb
	 * @param appearence user appearance
	 * @see SmfPresenceInfo
	 * @return False on Failure/Not supported 
	 */
	bool SmfPostProvider::postAppearence(SmfAppearenceInfo appearence)
		{ 
		m_private->postAppearence(appearence);
		} 
	/**
	 * Share /a contact's post to user's friends and followers (e.g. retweet in twitter, share on facebook)
	 * emits updatePostFinished() signal when done.
	 * @param postData data to be posted
	 * @param contact contact to which the post belonged
	 * @param bool whether user changed items within the post
	 */
	void SmfPostProvider::sharePost(SmfPost& postData,SmfContact& contact,bool edited)
		{
		m_private->sharePost(postData,contact,edited);
		}
	
	//APIs to get/set base provider info (SmfProvider)
	
	/**
	 * Gets the base provider info
	 */
	SmfProvider* SmfPostProvider::getProvider() const
			{
			return m_baseProvider;
			}

