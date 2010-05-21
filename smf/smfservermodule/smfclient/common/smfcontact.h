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
#include "smfclientglobal.h"
#include <qdatastream.h>
#include <QSharedData>
#include <QVariant>
//#include <smfclientglobal.h>
#include "smfcontact_p.h"

using namespace QtMobility;


//TODO:- For the time being we'll just store a string SmfContact 
/**
 * @ingroup smf_common_group
 * The contact class represents a social contact
 * 
 * Note: This class has dependencies on QtMobility project
 */
//QList<SmfContact> gives error for serialization if its derived from QObject
class  SMFCLIENT_EXPORT SmfContact //: public QObject
	{
	//Q_OBJECT

public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfContact( QObject *aParent = 0 );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfContact( const SmfContact &aOther );
	
	/**
	 * Destructor
	 */
	~SmfContact( );
//Q_DECLARE_LATIN1_LITERAL(SubTypeAddress, "Address");
//Q_DECLARE_LATIN1_LITERAL(SubTypeAnniversary, "Anniversary");
//Q_DECLARE_LATIN1_LITERAL(SubTypeAvatar, "Avatar");
//Q_DECLARE_LATIN1_LITERAL(SubTypeBirthday, "Birthday");
//Q_DECLARE_LATIN1_LITERAL(SubTypeContactId, "ContactId");
//Q_DECLARE_LATIN1_LITERAL(SubTypeEmailAddress, "EmailAddress");
//Q_DECLARE_LATIN1_LITERAL(SubTypeGender, "Gender");
//Q_DECLARE_LATIN1_LITERAL(SubTypeGeolocation, "Geolocation");
//Q_DECLARE_LATIN1_LITERAL(SubTypeGuid, "Guid");
//Q_DECLARE_LATIN1_LITERAL(SubTypeName, "Name");
//Q_DECLARE_LATIN1_LITERAL(SubTypeNickname, "Nickname");
//Q_DECLARE_LATIN1_LITERAL(SubTypeNote, "Note");
//Q_DECLARE_LATIN1_LITERAL(SubTypeOnlineAccount, "OnlineAccount");
//Q_DECLARE_LATIN1_LITERAL(SubTypeOrganization, "Organization");
//Q_DECLARE_LATIN1_LITERAL(SubTypePhoneNumber, "PhoneNumber");
//Q_DECLARE_LATIN1_LITERAL(SubTypeTimestamp, "Timestamp");
//Q_DECLARE_LATIN1_LITERAL(SubTypeType, "Type");
//Q_DECLARE_LATIN1_LITERAL(SubTypeUrl, "Url");
public slots:
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
	
	/**
	 * Method to set value for a subtype
	 */
	void setValue(const QString& aSubType,QVariant& value);
	void writeLog(QString log) const;
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
SMFCLIENT_EXPORT QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfContact &aContact );

/**
 * Method for Internalization. Reads a SmfContact object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aContact The SmfContact object to be internalized
 * @return reference to the stream
 */
SMFCLIENT_EXPORT QDataStream &operator>>( QDataStream &aDataStream, 
		SmfContact &aContact );
typedef QList<SmfContact> SmfContactList;
/**
 * Serialization support for Qt mobility contact
 */
SMFCLIENT_EXPORT QDataStream &operator<<( QDataStream &aDataStream, 
		const QContactName &aContact );

SMFCLIENT_EXPORT QDataStream &operator>>( QDataStream &aDataStream, 
		QContactName &aContact );
// Make the class SmfContact known to QMetaType, so that as to register it.
Q_DECLARE_METATYPE(SmfContact)
Q_DECLARE_METATYPE(QList<SmfContact>)

//make qt mobility related classes known to moc
Q_DECLARE_METATYPE(QContactAddress)
Q_DECLARE_METATYPE(QContactAnniversary)
Q_DECLARE_METATYPE(QContactAvatar)
Q_DECLARE_METATYPE(QContactBirthday)
Q_DECLARE_METATYPE(QContactId)
Q_DECLARE_METATYPE(QContactEmailAddress)
Q_DECLARE_METATYPE(QContactGender)
Q_DECLARE_METATYPE(QContactGuid)
Q_DECLARE_METATYPE(QContactName)
Q_DECLARE_METATYPE(QContactNickname)
Q_DECLARE_METATYPE(QContactNote)
Q_DECLARE_METATYPE(QContactOnlineAccount)
Q_DECLARE_METATYPE(QContactOrganization)
Q_DECLARE_METATYPE(QContactPhoneNumber)
Q_DECLARE_METATYPE(QContactTimestamp)
Q_DECLARE_METATYPE(QContactType)
Q_DECLARE_METATYPE(QContactUrl)

#endif /* SMFCONTACT_H_ */
