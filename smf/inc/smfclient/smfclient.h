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
 * The SmfEvent class represents an event
 *
 */

#ifndef SMFCLIENT_H
#define SMFCLIENT_H

#include <QObject>
#include <QImage>
#include <QUrl>

#include "../common/SmfClientGlobal.h"

class SmfProvider;

/**
 * @ingroup smf_client_group 
 * Interface for a base service provider. Other service provider classes contains 
 * implementation of this base class so that each has access to service provider 
 * information.
 * All of the functionality described here should be implemented by a service
 * specific plug-in object.
 */
class SMFCLIENT_EXPORT SmfClient
{
  /*Q_OBJECT*/

public:

  /**
   * @param name of the service 
   * @return service provider list
   */
  QList<SmfProvider>* GetServices(const QString& serviceName); 
  QList<SmfProvider>* GetServices(const QString& serviceName, const QString& providerName);
  QList<SmfProvider>* GetServices(const QString& serviceName, const SmfProvider& provider);  
};


#endif // SMFCLIENT_H
