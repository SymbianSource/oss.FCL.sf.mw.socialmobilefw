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
 * The Transport Manager class initializes the transport component of Smf 
 * framework
 * Note: This class has dependencies on QtMobility project
 *
 */

// Include files
#include "smftransportmanager.h"
#include "smftransportmanagerutil.h"

// Static data initialisation
SmfTransportManager* SmfTransportManager::m_myInstance = NULL;


/**
 * Method to get the instance of SmfTransportManager class
 * @return The instance of SmfTransportManager class
 */
SmfTransportManager* SmfTransportManager::getInstance ( )
	{
	if(NULL == m_myInstance)
		m_myInstance = new SmfTransportManager( );
	return m_myInstance;
	}

/**
 * Constructor with default argument
 * @param aParent The parent object
 */
SmfTransportManager::SmfTransportManager ( )
		: m_netwConfigMngr(this), m_systemInfo(this)
	{
	// Register for monitoring changes in IAPs (network configurations)
	connect(&m_netwConfigMngr, SIGNAL(configurationAdded(const QNetworkConfiguration &)), 
			this, SLOT(configurationAdded(const QNetworkConfiguration &)));
	
	connect(&m_netwConfigMngr, SIGNAL(configurationChanged(const QNetworkConfiguration &)), 
			this, SLOT(configurationChanged(const QNetworkConfiguration &)));
	
	connect(&m_netwConfigMngr, SIGNAL(configurationRemoved(const QNetworkConfiguration &)), 
			this, SLOT(configurationRemoved(const QNetworkConfiguration &)));
	}


/**
 * Destructor
 */
SmfTransportManager::~SmfTransportManager ( )
	{
	if(m_myInstance)
		delete m_myInstance;
	}


/**
 * Method to initialize the transport component before 
 * executing a web query
 * @return SmfTransportInitializeResult
 * @see smfglobal.h
 */
SmfTransportInitializeResult SmfTransportManager::initializeTransport ( )
	{
	SmfTransportInitializeResult retVal = SmfTransportInitNetworkNotAvailable;
	
	if(getNetworkAvailabilty())
		{
		retVal = SmfTransportInitNoError;
		
		SmfNetworkStatus status;
		getCurrentNetworkStatus(status);
		
		switch(status)
			{
			// homenetwork
			case SmfNetworkConnectedHome:
			case SmfNetworkConnected:
				break;

			// roaming
			case SmfNetworkConnectedRoaming:
///////////// check with CSM for usage while roaming
//// if enabled do nothing(i.e, retVal will be SmfTransportInitNoError)
//// else retVal = SmfTransportInitRoamingNetworkUsageNotEnabled;
/*				if(!iSmfServer->getNetworkUsageWhileRoamingSetting())
					retVal = SmfTransportInitRoamingNetworkUsageNotEnabled;*/
				break;
						
			// unknown state
			case SmfNetworkStateNotKnown:
			// network not available
			case SmfNetworkNotConnected:
			default :
				retVal = SmfTransportInitNetworkNotAvailable;
			}
		}
	return retVal;
	}


/* Method to get network availability.
 * @return True if online, else false.
 */
bool SmfTransportManager::getNetworkAvailabilty ( )
	{
	return m_netwConfigMngr.isOnline();
	}


/*
 * Method that checks if the phone is in home network or in roaming.
 * @param aStatus [out] An output parameter indicating the current network 
 * status as SmfNetworkStatus
 * @see smfglobal.h
 */
void SmfTransportManager::getCurrentNetworkStatus ( 
		SmfNetworkStatus &aStatus )
	{
	QSystemNetworkInfo::NetworkStatus status = m_systemInfo.networkStatus ( QSystemNetworkInfo::GsmMode );
	
	switch(status)
		{
		// homenetwork
		case QSystemNetworkInfo::HomeNetwork:
			aStatus = SmfNetworkConnectedHome;
			break;
		
		// connected
		case QSystemNetworkInfo::Connected:
			aStatus = SmfNetworkConnected;
			break;
		
		// roaming	
		case QSystemNetworkInfo::Roaming:
			aStatus = SmfNetworkConnectedRoaming;
			break;
			
		// unknown state			
		case QSystemNetworkInfo::Searching:
		case QSystemNetworkInfo::Busy:
			aStatus = SmfNetworkStateNotKnown;
			break;

		// network not available
		case QSystemNetworkInfo::UndefinedStatus:
		case QSystemNetworkInfo::NoNetworkAvailable:
		case QSystemNetworkInfo::EmergencyOnly:
		case QSystemNetworkInfo::Denied:
		default :
			aStatus = SmfNetworkNotConnected;
		}
	}



/**
 * This slot is called whenever a new network configuration is added to the system.
 * @param aConfig The new configuration
 */
void SmfTransportManager::configurationAdded ( 
		const QNetworkConfiguration &aConfig )
	{
	Q_UNUSED(aConfig);
	SmfTransportManagerUtil::getInstance()->configurationAdded(SmfTransportOpIAPChanged);
	}

/**
 * This slot is called when the state of the aConfig changes.
 * @param aConfig The changed configuration
 */
void SmfTransportManager::configurationChanged ( 
		const QNetworkConfiguration &aConfig )
	{
	if( aConfig == m_netwConfigMngr.defaultConfiguration() )
		SmfTransportManagerUtil::getInstance()->configurationChanged(SmfTransportOpIAPChanged);
	}

/**
 * This slot is called when a configuration is about to be removed from the system.
 * The removed configuration is invalid but retains name and identifier.
 * @param aConfig The to be removed configuration
 */
void SmfTransportManager::configurationRemoved ( 
		const QNetworkConfiguration &aConfig )
	{
	if( aConfig == m_netwConfigMngr.defaultConfiguration() )
		SmfTransportManagerUtil::getInstance()->configurationRemoved(SmfTransportOpIAPChanged);
	}

