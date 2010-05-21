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
* Description:
* SMF Server implementation for platforms other than Symbian.
* Uses  QLocalServer-QLocalSocket classes
*
*/

#include "smfserverqt_p.h"

#include <QLocalServer>
#include <QLocalSocket>

//
// SmfServerQt
//

SmfServerQt::SmfServerQt()
{
}

SmfServerQt::~SmfServerQt()
{
}

bool SmfServerQt::start()
{
    return false;
}

int SmfServerQt::sessionListCount() const
{
    return 0;
}

void SmfServerQt::writeLog(QString log) const
{
    Q_UNUSED(log);
}

/**
 * Called by the SmfServer when client authorization finishes.
 * @param success success of the authorization
 */
void SmfServerQt::clientAuthorizationFinished(bool success)
{
    Q_UNUSED(success);
}

void SmfServerQt::newClientConnected()
{
}

void  SmfServerQt::removeFromList()
{
}

int SmfServerQt::findAndServiceclient(int requestID,QByteArray* parsedData,SmfError error)
{
    Q_UNUSED(requestID);
    Q_UNUSED(parsedData);
    Q_UNUSED(error);
    return 0;
}

//
// SmfServerQtSession
//

SmfServerQtSession::SmfServerQtSession(QLocalSocket *clientConnection, SmfServerQt *server)
{
    Q_UNUSED(server);
    Q_UNUSED(clientConnection);
}

SmfServerQtSession::~SmfServerQtSession()
{
}

void SmfServerQtSession::readDataFromClient()
{
}

void SmfServerQtSession::clientAuthorizationFinished(bool success)
{
    Q_UNUSED(success);
}
