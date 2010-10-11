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
 * The SmfClient class provides a set of APIs to get list of service provider
 * who implement the given interface. Calling GetServices of the instance of this
 * class is mandatory before using any other interface.
 *
 */

#ifndef SMFCLIENT_H
#define SMFCLIENT_H

#include <QObject>
#include <qprocess.h>

#include "smfclientglobal.h"
#include "smfglobal.h"

//Forward declarations
class SmfProvider;
class SmfClientPrivate;

enum SMFProviderAuthorizationStatus
	{
	SMFProviderAuthStatusUnknown,	 	//status unknown
	SMFProviderAuthStatusAuthorised, 	//user logged in
	SMFProviderAuthStatusUnauthorised, 	//user logged out/not logged in
	SMFProviderAuthStatusBlocked		//user chosen to block this service
	};

/**
 * @ingroup smf_client_group 
 * Interface for a base service provider. Other service provider classes contains 
 * implementation of this base class so that each has access to service provider 
 * information.
 * All of the functionality described here should be implemented by a service
 * specific plug-in object.
 */
class  SMFCLIENT_EXPORT SmfClient : public QObject
	{
	Q_OBJECT
public:
	/**
	 * Constructs SmfClient
	 */
	SmfClient();
	
	/**
	 * Destructor
	 */
	~SmfClient();
	
	/**
	 * Method to get the list of services for a given service (interface name)
	 * @param serviceName The name of the service 
	 * @return Service provider's list (ownership of memory is transferred 
	 * to the calling party)
	 */
	QList<SmfProvider>* GetServices(const QString& serviceName);
	
	/**
	 * Checks the current status of the service. Application might want to 
	 * launch authorization process by calling loginToService() for the 
	 * service which is not authorised.
	 * @param provider The SmfProvider instance for the requested service
	 * @return Authorization status (loggedin, logged out, blocked, unknown etc)
	 */
	SMFProviderAuthorizationStatus checkAuthorization(const SmfProvider& provider);
	
	/**
	 * Method to launch the authentication application for this service.
	 * This would pop-up service specific login window. Till user logs in 
	 * successfully, any call to SMF APIs for this service Provider would 
	 * return SmfServiceAuthFailed. Symbian application should have 
	 * UserData capability.
	 * @param provider The SmfProvider instance for the requested service
	 * @return SmfError value 
	 */
	SmfError loginToService(SmfProvider* provider);
	
	/**
	 * Method to log-out of (unauthorise) this service. Please note that 
	 * this might not immediately log-out of the service if there are other 
	 * applications using this service. Application need not use this function 
	 * for normal operations - being logged in doesn't cost data charges and would 
	 * avoid disturbing log-in pop-ups. Please note that there is no notification 
	 * when user has logged out of service. Please use checkAuthorization() to 
	 * determine the current status
	 * @param provider The SmfProvider instance for the requested service
	 * @return SmfError value  
	 */
	SmfError logoutOfService(SmfProvider* provider); 
	
	// ToDo :- More overloaded APIs will be provided later
	/**
	 * Get provider details for a particular service provider for a particular service interface
	 * The provider name is always English lowercase with the domain separator 
	 * e.g. "facebook.com", "last.fm","linkedin.com"
	 * for updated list of service provider names please refer SMF wiki in SF
	 * TODO: provide url here.
	 */
	//SmfProvider* GetServices(const QString& serviceName, const QString& providerName);
	//QList<SmfProvider>* GetServices(const QString& serviceName, const SmfProvider& provider);
  
	/**
	 * This method returns the error message for the mentioned argument error code
	 * @param aErrorCode The SmfError code for which the error string is required
	 * @returns a QString object containing the required error message
	 */
	QString errorString ( const SmfError &aErrorCode ) const;

private slots:
	/**
	 * Slot that connects to the started() signal of QProcess
	 */
    void started();
    
	/**
	 * Slot that connects to the stateChanged() signal of QProcess
	 * @param newState The newState of the started Process
	 */
    void stateChanged(QProcess::ProcessState newState);
    
	/**
	 * Slot that connects to the error() signal of QProcess
	 * @param error The error that occured in the started process
	 */
    void error(QProcess::ProcessError error);
    
	/**
	 * Slot that connects to the finished() signal of QProcess. This signal 
	 * is emitted when the process finishes
	 * @param exitCode The exit code of the process
	 * @param exitStatus The exit status of the process
	 */
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    
private:
	/**
	 * Private impl wrapper.
	 */
	SmfClientPrivate* m_private;
	
	/**
	 * List of SmfProvider to be returned to the requested code
	 */
	QList<SmfProvider>* m_providerList;
	QProcess *m_authAppProcess;
  
	};

#endif // SMFCLIENT_H
