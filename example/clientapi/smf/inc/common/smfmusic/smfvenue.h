/**
 * @file	smfvenue.h
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
 * The venue class represents a venue and its related information
 * 
 * Note: This class has dependencies on QtMobility project
 */

#ifndef SMFVENUE_H_
#define SMFVENUE_H_

#include <QUrl>
#include <qgeopositioninfo.h> // Qt mobility class
#include <qdatastream.h>
#include "SmfClientGlobal.h"

/**
 * Popular venues.
 * Consists of venue name, city, street, pin, country,url and geo info.
 */
class SMFCLIENT_EXPORT SmfVenue : QObject
	{
	Q_OBJECT
	
public:
	
	/**
	 * Gets the venue name
	 */
	QString getName();
	
	/**
	 * Sets the venue name
	 */
	void setName(QString& name);
	
	/**
	 * Gets the city name
	 */
	QString getCity();
	
	/**
	 * Sets the city name
	 */
	void setCity(QString& name);
	
	/**
	 * Gets the Street name
	 */
	QString getStreet();
	
	/**
	 * Sets the Street name
	 */
	void setStreet(QString& name);
	
	/**
	 * Gets the Pin
	 */
	QString getPin();
	
	/**
	 * Sets the Pin
	 */
	void setPin(QString& name);
	
	/**
	 * Gets the Country name
	 */
	QString getCountry();
	
	/**
	 * Sets the Country name
	 */
	void setCountry(QString& name);
	
	/**
	 * Gets the geo info
	 */
	QtMobility::QGeoPositionInfo getGeo();
	
	/**
	 * Sets the geo info
	 */
	void setGeo(QtMobility::QGeoPositionInfo& geo);
	
	/**
	 * Gets the url
	 */
	QUrl getUrl();
	
	/**
	 * Sets the url
	 */
	void setUrl(QUrl& url);

	
private:
	QString m_name;
	QString m_city;
	QString m_street;
	QString m_pin;
	QString m_country;
	QtMobility::QGeoPositionInfo m_geo;
	QUrl m_url;
	};

/**
* Externalization
*/
QDataStream &operator<<(QDataStream &, const SmfVenue&);
/**
 * Internalization
 */
QDataStream &operator>>(QDataStream &, SmfVenue&);
#endif /* SMFVENUE_H_ */
