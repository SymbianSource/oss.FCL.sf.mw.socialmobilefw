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
 * The Provider Base class is the class that has to be contained as a member 
 * in all Plug-in Interfaces.
 *
 */

#ifndef SMFPROVIDERBASE_H_
#define SMFPROVIDERBASE_H_

#include <QImage>
#include <QUrl>
#include <QBuffer>
#include <smfglobal.h>
#include <qdatastream.h>
#include <QSharedData>

class SmfProviderBasePrivate;

/**
 * @ingroup smf_plugin_group
 * The Provider Base class is the class that has to be contained as a member 
 * in all Plug-in Interfaces.
 * 
 * All of plug-ins should contain this class as a member and should also 
 * contain a public method to get instance of this class. 
 */
class SmfProviderBase : public QObject
	{
	Q_OBJECT

public:
	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfProviderBase( QObject* aParent = 0 );
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfProviderBase( const SmfProviderBase &aOther );
	
	/**
	 * Destructor
	 */
	~SmfProviderBase( );

	/**
	 * Method to get the Localisable name of the service.
	 * @return The Localisable name of the service.
	 */
	QString serviceName( ) const;
	
	/**
	 * Method to get the Logo of the service
	 * @return The Logo of the service
	 */
	QImage serviceIcon( ) const;
	
	/**
	 * Method to get the Readable service description
	 * @return The Readable service description
	 */
	QString description( ) const;
	
	/**
	 * Method to get the Website of the service
	 * @return The Website of the service
	 */
	QUrl serviceUrl( ) const;
	
	/**
	 * Method to get the URL of the Application providing this service
	 * @return The URL of the Application providing this service
	 */
	QUrl applicationUrl( ) const;
	
	/**
	 * Method to get the Icon of the application
	 * @return The Icon of the application
	 */
	QImage applicationIcon( ) const;
	
	/**
	 * Method to get the Plugin specific ID
	 * @return The Plugin specific ID
	 */
	quint32 pluginId( ) const;
	
	/**
	 * Method to get the ID of the authentication application 
	 * for this service
	 * @param aProgram The authentication application name
	 * @param aArguments List of arguments required for authentication app
	 * @param aMode Strting mode for authentication application
	 * @return The ID of the authentication application 
	 */
	quint32 authenticationApp( QString &aProgram, QStringList & aArguments, 
			QIODevice::OpenModeFlag aMode = QIODevice::ReadWrite ) const;
	
	/**
	 * Method to get the unique registration ID provided by the 
	 * Smf for authorised plugins
	 * @return The unique registration ID/token provided by the Smf for 
	 * authorised plugins
	 */
	QString smfRegistrationId( ) const;
	
	/**
	 * Method to get the data usage of each plugin
	 * @return The data usage structure
	 */
	SmfPluginDataUsage getDataUsage( ) const;
	
	/**
	 * Method to get the reference count for loading.
	 * The reference count increased with each load request and decreased 
	 * with each unload request
	 * @return The reference count value for loading of the plugins
	 */
	uint getLoadRefCount( ) const;
	
private:
	QSharedDataPointer<SmfProviderBasePrivate> d;

	friend QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfProviderBase &aProviderBase );

	friend QDataStream &operator>>( QDataStream &aDataStream, 
			SmfProviderBase &aProviderBase );
	
	};


/**
 * Method for Externalization. Writes the SmfProviderBase object to 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be written
 * @param aProviderBase The SmfProviderBase object to be externalized
 * @return reference to the written stream
 */
QDataStream &operator<<( QDataStream &aDataStream, 
		const SmfProviderBase &aProviderBase );

/**
 * Method for Internalization. Reads a SmfProviderBase object from 
 * the stream and returns a reference to the stream.
 * @param aDataStream Stream to be read
 * @param aProviderBase The SmfProviderBase object to be internalized
 * @return reference to the stream
 */
QDataStream &operator>>( QDataStream &aDataStream, 
		SmfProviderBase &aProviderBase);

#endif /* SMFPROVIDERBASE_H_ */
