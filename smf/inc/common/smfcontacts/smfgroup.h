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
 * The group class represents an instance of a group as per SN site terminolgy
 *
 */

#ifndef SMFGROUP_H_
#define SMFGROUP_H_

#include <smfcontact.h>
#include <qdatastream.h>
#include <QSharedData>
#include <smfclientglobal.h>

class SmfGroupPrivate;


/**
 * @ingroup smf_common_group
 * The group class represents an instance of a group as per SN site terminolgy
 */
class SMFCLIENT_EXPORT SmfGroup
	{

public:
	/**
	 * Constructor with default argument
	 * @param list The list of members in the group
	 */
	SmfGroup( QList<SmfContact>* list = 0 );

	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfGroup( const SmfGroup &aOther );

	/**
	 * Destructor
	 */
	~SmfGroup( );

public:
	/**
	 * Method to get the list of members in the group
	 * @return The list of members in the group
	 */
	QList<SmfContact> members( ) const;

	/**
	 * Method to get the name of the group
	 * @return The name of the group
	 */
	QString name( ) const;

	/**
	 * Method to get the id of the group
	 * @return The ID value
	 */
	QString id( ) const;

private:
	QSharedDataPointer<SmfGroupPrivate> d;

	friend QDataStream &operator<<( QDataStream &aDataStream,
			const SmfGroup &aGroup );

	friend QDataStream &operator>>( QDataStream &aDataStream,
			SmfGroup &aGroup );

	};


/**
 * Method for Externalization. Writes the SmfGroup object to
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aGroup The SmfGroup object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream,
		const SmfGroup &aGroup );

/**
 * Method for Internalization. Reads a SmfGroup object from
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aGroup The SmfGroup object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream,
		SmfGroup &aGroup);

// Make the class SmfGroup known to QMetaType, so that as to register it.
Q_DECLARE_METATYPE(SmfGroup)

#endif /* SMFGROUP_H_ */
