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
 * Private class implemented for implicit sharing of SmfProviderBase class
 *
 */

#ifndef SMFPROVIDERBASE_P_H_
#define SMFPROVIDERBASE_P_H_

#include <QImage>
#include <QUrl>
#include <QBuffer>
#include <smfglobal.h>
#include <QSharedData>


class SmfProviderBasePrivate : public QSharedData
{
public:
	/**
	 * Constructor
	 */
	SmfProviderBasePrivate( ) { 
		m_serviceName.clear(); 
		m_description.clear();
		m_serviceUrl.clear();
		m_applicationUrl.clear();
		m_pluginId = 0;
		m_authenticationAppId = 0;
		m_registrationId.clear();
		m_loadRefCount = 0;
	}
	
	/**
	 * Copy Consturctor
	 * @param aOther The reference object to be copy constructed
	 */
	SmfProviderBasePrivate( const SmfProviderBasePrivate &aOther ) : 
		QSharedData ( aOther ), 
		m_serviceName ( aOther.m_serviceName ),
		m_serviceIcon ( aOther.m_serviceIcon ),
		m_description ( aOther.m_description ),
		m_serviceUrl ( aOther.m_serviceUrl ),
		m_applicationUrl ( aOther.m_applicationUrl ),
		m_applicationIcon ( aOther.m_applicationIcon ),
		m_pluginId ( aOther.m_pluginId ),
		m_authenticationAppId ( aOther.m_authenticationAppId ),
		m_registrationId ( aOther.m_registrationId ), 
		m_usage ( aOther.m_usage ),
		m_loadRefCount ( aOther.m_loadRefCount )	{ }
	
	/**
	 * Destructor
	 */
	~SmfProviderBasePrivate( );
  
	QString m_serviceName;	// the Localisable name of the service
	QImage m_serviceIcon;	// the Logo of the service
	QString m_description;	// the Readable service description
	QUrl m_serviceUrl;		// the Website of the service
	QUrl m_applicationUrl; 	// URL of the Application providing this service
	QImage m_applicationIcon;	//Icon of the application
	quint32 m_pluginId;		// The Plugin specific ID
	quint32 m_authenticationAppId; // ID of the authentication application for this service
	QString m_registrationId; 	// unique registration ID provided by the 
							// Smf for authorised plugins
	SmfPluginDataUsage m_usage;	//data usage of each plugin
	uint m_loadRefCount;	// reference count increased with each load request 
							// and decreased with each unload request
	
};

#endif /* SMFPROVIDERBASE_P_H_ */
