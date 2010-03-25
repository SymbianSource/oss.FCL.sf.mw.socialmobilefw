/**
* Copyright (c) 2010 Sasken Communication Technologies Ltd.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "{License}"
* which accompanies  this distribution, and is available
* at the URL "{LicenseUrl}".
*
* Initial Contributors:
* Chandradeep Gandhi, Sasken Communication Technologies Ltd - Initial contribution
*
* Contributors:
* Manasij Roy
* Description:
* Interface spefication for smf service provider
*
*/

#ifndef SMFGROUP_H_
#define SMFGROUP_H_

#include "SmfClientGlobal.h"
class SmfContact;
//List of SmfContacts
typedef QList<SmfContact> SmfContactList;
/**
 * class for a group in social network 
 */
class SMFCLIENT_EXPORT SmfGroup : public QObject
	{
	Q_OBJECT
public:
	
	/**
	 * Constructs a group with list of contacts
	 */
	SmfGroup(SmfContactList* list=0);
	/**
	 *Returns list of members in the group
	 *@return list of members 
	 */
	SmfContactList* members()
		{
		return m_members;
		}
	
	/**
	 * Sets the group members
	 * @param members list of members to be added to the group 
	 */
	void setMembers(SmfContactList& members);
private:
	SmfContactList* m_members;
	};
/**
 * Externalization
 */
QDataStream &operator<<(QDataStream &, const SmfGroup &);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfGroup &);
#endif /* SMFGROUP_H_ */
