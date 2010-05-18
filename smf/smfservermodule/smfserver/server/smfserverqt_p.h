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

#ifndef SMFSERVERQT_P_H
#define SMFSERVERQT_P_H

#include <QObject>
class QLocalServer;
class QLocalSocket;

class SmfServerQt : public QObject
{
    Q_OBJECT

public:
    SmfServerQt() {}
#endif

    ~SmfServerQt() {}
    bool start() {return false;}
    int sessionListCount() const {return 0;}
private slots:
    void newClientConnected() {}
    void removeFromList() {}

private:
    QLocalServer *server;
    QLocalSocket *iClient;
};

class SmfServerQtSession : public QObject
{
    Q_OBJECT
public:
    SmfServerQtSession(QLocalSocket *clientConnection, SmfServerQt *server) {Q_UNUSED(server) Q_UNUSED(clientConnection)}
    ~SmfServerQtSession() {}

public slots:
    void readDataFromClient() {}
    
private:
    SmfServerQt *iServer;
    QLocalSocket *iClientConnection;

};

//#endif // SMFSERVERQT_P_H
