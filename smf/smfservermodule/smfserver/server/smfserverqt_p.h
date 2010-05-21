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

#include "smfglobal.h"

#include <QObject>
class QLocalServer;
class QLocalSocket;

class SmfServerQt : public QObject
{
    Q_OBJECT

public:
    SmfServerQt();
    ~SmfServerQt();

    bool start();
    int sessionListCount() const;
    void writeLog(QString log) const;
    void clientAuthorizationFinished(bool success);
    int findAndServiceclient(int requestID,QByteArray* parsedData,SmfError error);

private slots:
    void newClientConnected();
    void removeFromList();

private:
    QLocalServer *m_server;
    QLocalSocket *m_client;
};

class SmfServerQtSession : public QObject
{
    Q_OBJECT

public:
    SmfServerQtSession(QLocalSocket *clientConnection, SmfServerQt *server);
    ~SmfServerQtSession();

    void clientAuthorizationFinished(bool success);

public slots:
    void readDataFromClient();
    
private:
    SmfServerQt *m_server;
    QLocalSocket *m_clientConnection;

};

#endif // SMFSERVERQT_P_H
