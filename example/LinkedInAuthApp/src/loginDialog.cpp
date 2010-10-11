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
* Siddhartha Chandra, Satish Kanteti Sasken Communication Technologies Ltd
* Description:
* Login Dialog class of Authentication Application
*/

#include <qdebug.h>

#include "authAppConstants.h"
#include "sessionSP.h"
#include "errorCodes.h"
#include "loginDialog.h"
#include "keys.h"
#include <QNetworkCookie>
#include <QInputDialog>
#include <QSslConfiguration>

static const QString kStringBoundary = "3i2ndDfv2rTHiSisAbouNdArYfORhtTPEefj3q2f";
///////////////////////////////////////////////////////////////////////////////////////////////////
// global

//static const QString kLoginURL = "http://www.facebook.com/login.php";

///////////////////////////////////////////////////////////////////////////////////////////////////
void FBLoginDialog::connectToGetSession(const QString& aToken)
{
	qDebug()<<"Inside FBLoginDialog::connectToGetSession()";
    iGetSessionRequest = FBRequest::requestWithSession(iSession);
    
    //connect(iGetSessionRequest, SIGNAL(LoadLoginPage()), this, SLOT(LoadLoginPage()));
    connect(iGetSessionRequest, SIGNAL(requestDidLoad(QVariant)), this, SLOT(requestDidLoad(QVariant)));
    connect(iGetSessionRequest, SIGNAL(requestFailedWithNetworkError(QNetworkReply::NetworkError)), this, SLOT(requestFailedWithNetworkError(QNetworkReply::NetworkError)));
    connect(iGetSessionRequest, SIGNAL(requestFailedWithFacebookError(FBError)), this, SLOT(requestFailedWithFacebookError(FBError)));

    Dictionary params;
    params["auth_token"] = aToken;

    if (iSession->apiSecret().length())
    {
        params["generate_session_secret"]="1";
    }

    if (iSession->getSessionProxy().length())
    {
        iGetSessionRequest->post(iSession->getSessionProxy(),params);
    }
    else
    {
        iGetSessionRequest->call("facebook.auth.getSession", params);
    }
}
void FBLoginDialog::connectToGetToken()
	{
		qDebug()<<"Inside FBLoginDialog::connectToGetToken()";
	    iGetSessionRequest = FBRequest::requestWithSession(iSession);

	   // connect(iGetSessionRequest, SIGNAL(requestDidLoad(QVariant)), this, SLOT(requestDidLoad(QVariant)));
	 //   connect(iGetSessionRequest, SIGNAL(requestFailedWithNetworkError(QNetworkReply::NetworkError)), this, SLOT(requestFailedWithNetworkError(QNetworkReply::NetworkError)));
	  //  connect(iGetSessionRequest, SIGNAL(requestFailedWithFacebookError(FBError)), this, SLOT(requestFailedWithFacebookError(FBError)));

	    Dictionary postParams;
		iSession->stroauth_nonce = generateNONCE(43);
		iSession->stroauth_timestamp = generateTimeStamp();//QString::number(generateTimeStamp());
		qDebug()<<"iSession->stroauth_timestamp"<<iSession->stroauth_timestamp;
		//postParams["oauth_callback"] = kBase;
		postParams["oauth_consumer_key"] = kConsumerKey;
		postParams["oauth_signature_method"] = "HMAC-SHA1";
		postParams["oauth_timestamp"] = iSession->stroauth_timestamp;
		postParams["oauth_nonce"] = iSession->stroauth_nonce;
		postParams["oauth_version"] = "1.0";
		FBRequest* req = FBRequest::requestWithSession(iSession);
		iSession->stroauth_signature = req->generateSig(postParams);
	    iGetSessionRequest->call("/uas/oauth/accessToken", postParams);
	    
	}
void FBLoginDialog::loadLoginPage()
{
    iGetSessionRequest = FBRequest::requestWithSession(iSession);
	qDebug()<<"Inside FBLoginDialog::loadLoginPage()";
    Dictionary getParams, postParams;
    iSession->stroauth_nonce = generateNONCE(43);
	iSession->stroauth_timestamp = generateTimeStamp();//QString::number(generateTimeStamp());
	qDebug()<<"iSession->stroauth_timestamp"<<iSession->stroauth_timestamp;
    postParams["oauth_callback"] = "oob";
    postParams["oauth_consumer_key"] = kConsumerKey;
    postParams["oauth_signature_method"] = "HMAC-SHA1";
    postParams["oauth_timestamp"] = iSession->stroauth_timestamp;
    postParams["oauth_nonce"] = iSession->stroauth_nonce;
    postParams["oauth_version"] = "1.0";
   /* FBRequest* req = FBRequest::requestWithSession(iSession);*/
    iSession->stroauth_signature = iGetSessionRequest->generateSig(postParams);
    
    connect(iGetSessionRequest, SIGNAL(LoadLoginPage()), this, SLOT(LoadLoginPage()));
    
    iGetSessionRequest->connect();
    
    
    
    
    
  
    //loadURL(kRequestTokenUrl,QNetworkAccessManager::PostOperation, getParams, postParams);

}
/*int FBLoginDialog::generateTimeStamp()
	{
		QDate oldDate;
		oldDate.setYMD(1970,1,1);
	
		QDateTime OldDateTime;
		OldDateTime.setDate(oldDate);
		QTime temp;
		temp.setHMS(0,0,0,0);
		OldDateTime.setTime(temp);
		QDateTime CurTime = QDateTime::currentDateTime();
		
		int timestamp = OldDateTime.secsTo(CurTime);
		return timestamp;
	}*/
QByteArray FBLoginDialog::generateTimeStamp()
	{
		uint time = QDateTime::currentDateTime().toTime_t();
	    QByteArray timestamp = QByteArray::number( time );
	    return timestamp;
	}
QString FBLoginDialog::generateNONCE(const qint64 Length)
	{
	srand(time(0));
	//QDateTime UniqueNumber = QDateTime::currentDateTime();

	//read upto milliseconds
	QString RetString;//(UniqueNumber.toString("hh/mm/ss/zzz"));

	QString Letters(
			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

	//append a randomly generated string to RetString
	for (int i = 0; i < Length; i++)
		{
		RetString.insert((i), Letters.at(rand() % Letters.size()));
		}

	return RetString;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////

FBLoginDialog::FBLoginDialog() : FBDialog () {}

FBLoginDialog::FBLoginDialog(FBSession* aSession) : FBDialog(aSession) {}

///////////////////////////////////////////////////////////////////////////////////////////////////
// FBDialog

void FBLoginDialog::load()
{
	qDebug()<<"Inside FBLoginDialog::load()";
    loadLoginPage();
}

void FBLoginDialog::dialogWillDisappear()
{
	qDebug()<<"Inside FBLoginDialog::dialogWillDisappear()";
  iGetSessionRequest->cancel();

  if (!iSession->isConnected())
  {
      iSession->cancelLogin();
  }
}

void FBLoginDialog::GetSessionKey(const QUrl& aUrl)
{
	qDebug()<<"Inside FBLoginDialog::GetSessionKey()";
    const QString authToken = "auth_token=";
    const QString url ( aUrl.toString() );

    int start = url.indexOf(authToken);
    if (start != -1)
    {
        QString token;
        int end = url.indexOf("&", start);
        int delta = start + authToken.size() + 1;
        if (end != -1)
        {
            token = url.mid(delta, end - delta );
        }
        else
        {
            token = url.right(url.size() - delta + 1);
        }

        connectToGetSession(token);
    }
}
void FBLoginDialog::FetchKeyFromUrl(const QUrl& aUrl)
{
    const QString url ( aUrl.toString() );
    QStringList varList = url.split("{");
    QString newvar = varList[1];
    varList.clear();
    varList = newvar.split(",");
    QString session_key = varList[0];
    QString uid = varList[1];
    QString expires = varList[2];
    QString secret = varList[3];
    varList.clear();
    varList = session_key.split(":");
    session_key = varList[1];
    session_key = session_key.mid(1,session_key.length()-2);
    varList.clear();
    varList = uid.split(":");
    uid = varList[1];
	varList.clear();
	varList = expires.split(":");
	expires = varList[1];
	varList.clear();
	varList = secret.split(":");
	secret = varList[1];
	secret = secret.mid(1,secret.length()-2);
	varList.clear();
    qDebug()<<"session_key"<<session_key<<"\n";
    qDebug()<<"uid"<<uid<<"\n";
    qDebug()<<"expires"<<expires<<"\n";
    qDebug()<<"secret"<<secret<<"\n";
    uint expiry = expires.toUInt();//.toUInt(&conversionError);
    QDateTime expiration; expiration.setTime_t( expiry );
    iSession->beginSession(session_key, secret,expiration);
    iSession->resume();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// slots for signals from FBRequest

void FBLoginDialog::requestDidLoad(const QVariant& aResult)
{
	qDebug()<<"Inside FBLoginDialog::requestDidLoad";
    bool conversionError = false;
    QVariantHash object = aResult.toHash();
    QString sessionKey = object.value("session_key").toString();
    QString sessionSecret = object.value("secret").toString();

    QVariant ex = object.value("expires");
    uint expires = object.value("expires").toUInt(&conversionError);
    QDateTime expiration; expiration.setTime_t( expires );

    iSession->beginSession(sessionKey, sessionSecret,expiration);
    iSession->resume();

    dismissWithSuccess(true, true);
}

void FBLoginDialog::requestFailedWithFacebookError (const FBError& aCode )
{
	qDebug()<<"Inside FBLoginDialog::requestFailedWithFacebookError()";
    dismissWithError(aCode, true);
}

void FBLoginDialog::requestFailedWithNetworkError( QNetworkReply::NetworkError aCode )
{
	qDebug()<<"Inside FBLoginDialog::requestFailedWithNetworkError()";
    dismissWithError(aCode, true);
}
void FBLoginDialog::LoadLoginPage()
	{
	
	qDebug()<<"Enter: LoadLoginPage()";
	QString LoginUrl = "https://api.linkedin.com/uas/oauth/authorize?oauth_token=";//94ab03c4-ae2c-45e4-8732-0e6c4899db63";
	LoginUrl += iSession->stroauth_Token;
    proxysettings();
	qDebug()<<"LoginUrl:"<<LoginUrl;
	//TBuf<1024> sid(LoginUrl.utf16()); //uncomment to invoke browser
	//LaunchBrowserL(sid); //uncomment to invoke browser
	QNetworkRequest request;
	QSslConfiguration config = request.sslConfiguration();
	config.setProtocol(QSsl::SslV3);
	request.setSslConfiguration(config);
	request.setUrl(QUrl(LoginUrl));
	iWebView->load(request,QNetworkAccessManager::PostOperation); //uncomment to invoke browser


	qDebug()<<"Exit: LoadLoginPage()";
	   
	}
void FBLoginDialog::LaunchBrowserL(const TDesC& aUrl)
{
	qDebug()<<"Enter: LaunchBrowserL()";
	const TInt KWmlBrowserUid = 0x10008D39;
	TUid id( TUid::Uid( KWmlBrowserUid ) );
	TApaTaskList taskList( CEikonEnv::Static()->WsSession() );
	TApaTask task = taskList.FindApp( id );
	if ( task.Exists() )
	{
		qDebug()<<"Inside: if()";
		HBufC8* param = HBufC8::NewLC( aUrl.Length() + 2);
                //"4 " is to Start/Continue the browser specifying a URL
		param->Des().Append(_L("4 "));
		param->Des().Append(aUrl);
		task.SendMessage( TUid::Uid( 0 ), *param ); // Uid is not used
		CleanupStack::PopAndDestroy(param);
	}
	else
	{
		qDebug()<<"Inside: else()";
		HBufC16* param = HBufC16::NewLC( aUrl.Length() + 2);
                //"4 " is to Start/Continue the browser specifying a URL
		param->Des().Append(_L("4 "));
		param->Des().Append(aUrl);
		RApaLsSession appArcSession;
                // connect to AppArc server 
		User::LeaveIfError(appArcSession.Connect()); 
		TThreadId id;
		appArcSession.StartDocument( *param, TUid::Uid( KWmlBrowserUid)
                                                                       , id );
		appArcSession.Close(); 
		CleanupStack::PopAndDestroy(param);
	}
}
void FBLoginDialog::GetAccessToken()
	{
		/*qDebug()<<"Inside FBLoginDialog::GetAccessToken()";
	    Dictionary getParams, postParams;
	    iSession->stroauth_nonce = generateNONCE(43);
		iSession->stroauth_timestamp = generateTimeStamp();//QString::number(generateTimeStamp());
		qDebug()<<"iSession->stroauth_timestamp"<<iSession->stroauth_timestamp;
	  
	    postParams["oauth_consumer_key"] = kConsumerKey;
	    postParams["oauth_signature_method"] = "HMAC-SHA1";
	    postParams["oauth_timestamp"] = iSession->stroauth_timestamp;
	    postParams["oauth_nonce"] = iSession->stroauth_nonce;
	    postParams["oauth_version"] = "1.0";
	
	    iSession->stroauth_signature = iGetSessionRequest->generateSig(postParams);*/
	}
