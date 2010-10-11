/**
* Copyright (c) 2010 Sasken Communication Technologies Ltd.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "{License}"
* which accompanies  this distribution, and is available
* at the URL "{LicenseUrl}".
*
* Initial Contributors:
* Narasimhulu Kavadapu, Sasken Communication Technologies Ltd - Initial contribution
*
* Contributors:
* Siddhartha Chandra, Sasken Communication Technologies Ltd
* Description:
* class to maintian session & all credential keys.
*/

#include "sessionSP.h"
#include "requestSP.h"
#include <qfile.h>
#include <QTimer>
#include <qdebug.h>
#include <keys.h>
///////////////////////////////////////////////////////////////////////////////////////////////////
// global


static FBSession* sharedSession = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Static class functions
FBSession* FBSession::session()
{
    return sharedSession;
}

void FBSession::setSession(FBSession* aSession)
{
    sharedSession = aSession;
}


FBSession* FBSession::sessionForApplication ( const QString& aAppKey, const QString& aAppSecret, const QString& aSessionProxy)
{
    FBSession* session = new FBSession ( aAppKey, aAppSecret, aSessionProxy );
    return session;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// instance public functions
FBSession::FBSession( const QString& aAppKey, const QString& aAppSecret, const QString& aGetSessionProxy ) :
        iApiKey (aAppKey),
        iApiSecret ( aAppSecret ),
        m_Client(new SmfCredMgrClient(this))	
{
	Q_UNUSED(aGetSessionProxy)
    if (!sharedSession)
    {
        sharedSession = this;
    }
}

FBSession::~FBSession()
{
	if(m_Client){
		delete m_Client;
		m_Client = NULL;
	}
}

const QString& FBSession::apiURL() const
{
    return kAPIRestURL;
}
void FBSession::SaveToken (const QString& token )
{
	iToken = token;
}
void FBSession::SaveSession (const QString& session, const QString& name )
{
	qDebug()<<"Inside FBSession::SaveSession";
	qDebug()<<"token argument = "<<session;
	iLastfmSessionKey = session;
	iLastfmUserName = name;
    save();
    emit sessionDidLogin(session);
}
bool FBSession::resume()
{
    QString CMRegToken = iSettings.value("CMFlickrRegToken", "NA" ).toString();
	QDateTime ExpiryTime = iSettings.value("FlckrExpiryTime","NA").toDateTime();
	
	qDebug() << "CMRegToken = :" << CMRegToken;
	qDebug() << "ExpiryTime = :" << ExpiryTime;
	
	SmfAuthParams Params;
	if(m_Client->AuthDataSet(CMRegToken,ExpiryTime,Params))
	{
		QByteArray accessToken = Params.value("finalToken");
       emit sessionDidLogin( accessToken );
       return true;
    }
    return false;
}
void FBSession::logout() {

	iLastfmSessionKey.clear();
	iLastfmUserName.clear();
	iToken.clear();

        unsave();

        emit sessionDidLogout();
}

void FBSession::send (FBRequest* aRequest) {
	qDebug()<<"Inside FBSession::send";
    performRequest (aRequest, true);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// instance private functions
void FBSession::save()
{
	qDebug()<<"Inside FBSession::save()";
	qDebug()<<"Session Key= "<<iLastfmSessionKey;

    SmfAuthParams Params;
    Params.insert("ApiKey",kApiKey.toAscii());
    Params.insert("ApiSecret",kApiSecret.toAscii());
    Params.insert("ApiToken",iToken.toAscii());
    Params.insert("SessionKey",iLastfmSessionKey.toAscii());
    Params.insert("Name",iLastfmUserName.toAscii());
    
    QList<QUrl> UrlList;
    UrlList.append(QUrl(kAPIRestURL));
    
    QStringList PluginList;
    PluginList.append(QString("LastFmmusiceventplugin.qtplugin"));
    PluginList.append(QString("LastFmmusicsearchplugin.qtplugin"));
    PluginList.append(QString("LastFmmusicserviceplugin.qtplugin"));
    PluginList.append(QString("LastFmplaylistserviceplugin.qtplugin"));
    QString UID("0xE1D8C7D8");
    
    //Currently Hardcoded with current time bcoz CM is not handling expiry time as '0' value
    QDateTime ExpirationDate = QDateTime::currentDateTime();
    ExpirationDate.addYears(1);
    
    QString CMRegToken = m_Client->StoreAuthData(Params,ExpirationDate,UrlList,PluginList,UID,true);
    
    if(CMRegToken.size()){
    	iSettings.setValue("CMLastFmRegToken", CMRegToken);
    	iSettings.setValue("LastFmExpiryTime", ExpirationDate);
    }
}

void FBSession::unsave()
{

}
bool FBSession::performRequest(FBRequest* aRequest, bool aEnqueue) {
    // Stagger requests that happen in short bursts to prevent the server from rejecting
    // them for making too many requests in a short time
	Q_UNUSED(aEnqueue)
	qDebug()<<"Inside FBSession::performRequest";
        aRequest->connect();
        return true;
}
