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
*
* Description:
* class to maintian session & all credential keys.
*/

#include "sessionSP.h"
#include "requestSP.h"
#include <qfile.h>
#include <QTimer>
#include <qdebug.h>
///////////////////////////////////////////////////////////////////////////////////////////////////
// global

static const QString kAPIRestURL = "http://api.facebook.com/restserver.php";
static const QString kAPIRestSecureURL = "https://api.facebook.com/restserver.php";

static const int kMaxBurstRequests = 3;
static const int kBurstDuration = 2;

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
        iGetSessionProxy ( aGetSessionProxy ),
        iRequestBurstCount(0),
        m_Client(new SmfCredMgrClient(this))	
{
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

const QString& FBSession::apiSecureURL() const
{
    return kAPIRestSecureURL;
}

bool FBSession::isConnected() const
{
    return iSessionKey.length() > 0 ;
}


void FBSession::beginSession (const QString& aSessionKey, const QString& aSessionSecret, const QDateTime& aExpires )
{
    iSessionKey = aSessionKey;
    iSessionSecret = aSessionSecret;


    iExpirationDate = aExpires;

    save();
}

bool FBSession::resume()
{
	QString CMRegToken = iSettings.value("CMRegToken").toString();
	QDateTime ExpiryTime = iSettings.value("ExpiryTime").toDateTime();
	SmfAuthParams Params;
	if(m_Client->AuthDataSet(CMRegToken,ExpiryTime,Params))
	{
		QByteArray accessToken = Params.value("accessToken");
		emit sessionDidLogin( accessToken );
        return true;
    }
    return false;
}

void FBSession::cancelLogin() {
    if (!isConnected()) {
        emit sessionDidNotLogin();
    }
}

void FBSession::logout() {

	iExpirationDate = QDateTime();
	iSessionKey.clear();
	iSessionSecret.clear();
	
	unsave();
	
	emit sessionDidLogout();
}

void FBSession::send (FBRequest* aRequest) {
    performRequest (aRequest, true);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// instance private functions
void FBSession::save()
{
  
    SmfAuthParams Params;
    Params.insert("accessToken",iSessionKey.toAscii());
    
    QList<QUrl> UrlList;
    UrlList.append(QUrl("http://www.facebook.com"));
    
    QStringList PluginList;
    PluginList.append(QString("facebook"));
    
    QString UID("0xEFE2FD23");
    
    //Currently Hardcoded with current time bcoz CM is not handling expiry time as '0' value
    iExpirationDate = QDateTime::currentDateTime();
    iExpirationDate.addYears(1);
    
    QString CMRegToken = m_Client->StoreAuthData(Params,iExpirationDate,UrlList,PluginList,UID,true);
    
    if(CMRegToken.size()){
    	iSettings.setValue("CMRegToken", CMRegToken);
    	iSettings.setValue("ExpiryTime", iExpirationDate);
    }
}

void FBSession::unsave()
{
	//Delete saved keys from Credential Manager.
}

void FBSession::startFlushTimer()
{
	int t = kBurstDuration;
    QTimer::singleShot( t, this, SLOT(requestTimerReady()));
}

void FBSession::enqueueRequest(FBRequest* aRequest)
{
    iRequestQueue.append(aRequest);
    startFlushTimer();
}

bool FBSession::performRequest(FBRequest* aRequest, bool aEnqueue) {
    // Stagger requests that happen in short bursts to prevent the server from rejecting
    // them for making too many requests in a short time

    int seconds = iLastRequestTime.secsTo( QDateTime::currentDateTime() );
	bool burst = seconds && (seconds < kBurstDuration);

	if (burst && (iRequestBurstCount > kMaxBurstRequests))
    {
        if (aEnqueue)
        {
            enqueueRequest(aRequest);
        }
        return false;
    }
    else
    {
        aRequest->connect();
		if (burst) {
			iRequestBurstCount++;
		} else {
            iRequestBurstCount = 1;
            iLastRequestTime = aRequest->timeStamp();
        }
    }
    return true;
}

void FBSession::flushRequestQueue()
{
    while ( iRequestQueue.count() ) {
      FBRequest* request = iRequestQueue.at(0);
      if (performRequest(request, false)) {
          iRequestQueue.removeAt(0);
      } else {
        startFlushTimer();
        break;
      }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// instance provate slots
void FBSession::requestTimerReady()
{
    flushRequestQueue();
}
