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
 */
#include "smfclient.h"
#include "smfclient_p.h"


SmfClient::SmfClient()
	{
	writeLog("SmfClient::SmfClient");
	//Create instance of the private wrapper
	m_private = new SmfClientPrivate(this);
	m_providerList = NULL;
	}
SmfClient::~SmfClient()
	{
	if(m_private)
		{
		delete m_private;
		m_private = NULL;
		}
	}

QList<SmfProvider>* SmfClient::GetServices(const QString& serviceName)
	{
	writeLog("SmfClient::GetServices");
	if(m_providerList)
		{
		delete m_providerList;
		m_providerList = NULL;
		}
	writeLog("Before m_private->GetServices");
	//Getting the data synchronously
	m_providerList = m_private->GetServices(serviceName);
	writeLog("After m_private->GetServices");
	return m_providerList;
	}
void SmfClient::writeLog(QString log) const
	{
#ifdef WRITE_LOG
	QFile file("c:\\data\\SmfClientLogs.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
	         ;
    QTextStream out(&file);
    out << log << "\n";
    file.close();
#endif
	}
