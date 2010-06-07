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
 *
 * Description: Testing the GetServices API of Social Mobile Framework, expected to
 * return a list of installed service providers for a given interface.
 */

#include <QtTest>

#include "smfclient.h"

class TestSmfGetServices : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testConnectToServer();
    void cleanupTestCase();

private:
    SmfClient* m_smf;
};

void TestSmfGetServices::initTestCase()
{
    m_smf = new SmfClient();
}

void TestSmfGetServices::testConnectToServer()
{
    QList<SmfProvider> *services = m_smf->GetServices("org.symbian.smf.contacts.fetcher");
    qDebug() << "SmfClient::GetServices returned " << services->length() << " services.";
}

void TestSmfGetServices::cleanupTestCase()
{
    delete m_smf;
}

QTEST_MAIN(TestSmfGetServices)
#include "testsmfgetservices.moc"

