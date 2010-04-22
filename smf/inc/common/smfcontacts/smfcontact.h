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
 * The contact class represents a social contact
 * Note: This class has dependencies on QtMobility project
 *
 */

#ifndef SMFCONTACT_H_
#define SMFCONTACT_H_

#include "qtcontacts.h"
#include <qdatastream.h>
#include <QSharedData>
#include <smfclientglobal.h>

using namespace QtMobility;

class SmfContactPrivate;

/**
 * @ingroup smf_common_group
 * The contact class represents a social contact
 * 
 * Note: This class has dependencies on QtMobility project
 */
class SMFCLIENT_EXPORT SmfContact
	{

public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfContact();
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfContact( const SmfContact &aOther );
	
	/**
	 * Destructor
	 */
	~SmfContact( );
	
	/**
	 * Method to get the available sub fields for contacts.
	 * The following are the available sub types of a contact
	 *   QtMobility::QContactAddress		Address;
	 *   QtMobility::QContactAnniversary	Anniversary;
	 *   QtMobility::QContactAvatar			Avatar;
	 *   QtMobility::QContactBirthday		Birthday;
	 *   QtMobility::QContactId				ContactId;
	 *   QtMobility::QContactEmailAddress	EmailAddress;
	 *   QtMobility::QContactGender			Gender;
	 *   QtMobility::QContactGeolocation	Geolocation;
	 *   QtMobility::QContactGuid			Guid;
	 *   QtMobility::QContactName			Name;
	 *   QtMobility::QContactNickname		Nickname;
	 *   QtMobility::QContactNote			Note;
	 *   QtMobility::QContactOnlineAccount	OnlineAccount;
	 *   QtMobility::QContactOrganization	Organization;
	 *   QtMobility::QContactPhoneNumber	PhoneNumber;
	 *   QtMobility::QContactTimestamp		Timestamp;
	 *   QtMobility::QContactType			Type;
	 *   QtMobility::QContactUrl			Url;
	 * @return The Available sub fields for this contact
	 */
	QStringList subTypes( ) const;
	
	/**
	 * Method to convert an SmfContact to a QContact
	 * Changes made to the returned QContact will not be reflected 
	 * in its parent SmfContact object
	 * @param aContact QContact corresponding to SmfContact 
	 */
	void convert( QContact &aContact ) const;
	
	/**
	 * Method to get the value of a sub field for this contact
	 * @param aSubType The sub field type 
	 * @return The value of the sub field subType
	 * @see subTypes()
	 */
	QVariant value( const QString& aSubType ) const;
	
private:
	QSharedDataPointer<SmfContactPrivate> d;
	
	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfContact &aContact );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfContact &aContact );
	
	};


/**
 * Method for Externalization. Writes the SmfContact object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aContact The SmfContact object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfContact &aContact );

/**
 * Method for Internalization. Reads a SmfContact object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aContact The SmfContact object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfContact &aContact );

// Make the class SmfContact known to QMetaType, so that as to register it.
Q_DECLARE_METATYPE(SmfContact)

#endif /* SMFCONTACT_H_ */
