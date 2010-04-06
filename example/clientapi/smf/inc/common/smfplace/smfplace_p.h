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
 * Private class implemented for implicit sharing of SmfPlace class
 *
 */

#ifndef SMFPLACE_P_H_
#define SMFPLACE_P_H_

#include <QUrl>
#include <qgeopositioninfo.h> // Qt mobility class
#include <QSharedData>

using namespace QtMobility; // Qt mobility - namespace

class SmfPlacePrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfPlacePrivate( ) { 
		m_name.clear();
		m_city.clear();
		m_street.clear();
		m_zipcode.clear();
		m_country.clear();
		m_url.clear();
		m_placeId.clear(); }
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfPlacePrivate( const SmfPlacePrivate &aOther ) : 
		QSharedData ( aOther ), 
		m_name ( aOther.m_name ),
		m_city ( aOther.m_city ),
		m_street ( aOther.m_street ),
		m_zipcode ( aOther.m_zipcode ),
		m_country ( aOther.m_country ),
		m_geo ( aOther.m_geo ),
		m_url ( aOther.m_url ),
		m_placeId ( aOther.m_placeId ) 	{ }
	
	/**
	 * Destructor
	 */
	~SmfPlacePrivate( );
  
	QString m_name;						// place name
	QString m_city;						// city of place
	QString m_street;					// street of place
	QString m_zipcode;					// zip code of place
	QString m_country;					// country of place
	QtMobility::QGeoPositionInfo m_geo;	// place geo position information
	QUrl m_url;							// url indicating the place
	QString m_placeId;					// place id
	
};

#endif /* SMFPLACE_P_H_ */
