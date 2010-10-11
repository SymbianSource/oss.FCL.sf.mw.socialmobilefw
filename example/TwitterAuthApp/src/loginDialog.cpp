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
* Login Dialog class of Authentication Application
*/

#include <qdebug.h>

#include "authAppConstants.h"
#include "sessionSP.h"
#include "errorCodes.h"
#include "loginDialog.h"
#include "keys.h"
#include <QNetworkCookie>
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

    postParams["oauth_consumer_key"] = kConsumerKey;
    postParams["oauth_signature_method"] = "HMAC-SHA1";
    postParams["oauth_timestamp"] = iSession->stroauth_timestamp;
    postParams["oauth_nonce"] = iSession->stroauth_nonce;
    postParams["oauth_version"] = "1.0";
    iSession->stroauth_signature = iGetSessionRequest->generateSig(postParams);
    
    connect(iGetSessionRequest, SIGNAL(LoadLoginPage()), this, SLOT(LoadLoginPage()));
    
    iGetSessionRequest->connect();
}

QByteArray FBLoginDialog::generateTimeStamp()
	{
		uint time = QDateTime::currentDateTime().toTime_t();
	    QByteArray timestamp = QByteArray::number( time );
	    return timestamp;
	}
QString FBLoginDialog::generateNONCE(const qint64 Length)
	{
	srand(time(0));
	
	//read upto milliseconds
	QString RetString;

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
	QString LoginUrl = kAuthorizeUrl + "?oauth_token=";
	LoginUrl += iSession->stroauth_Token;
	
/*	QNetworkCookieJar* cookieJar = iWebView->page()->networkAccessManager()->cookieJar();
	iWebView->page()->networkAccessManager()->setCookieJar(cookieJar);
	QNetworkCookie testCookie;// "test_cookie", "1");
	
	testCookie.setDomain ( "twitter.com" );
	QDateTime dt = QDateTime::currentDateTime();
	dt.addDays(1);
	testCookie.setExpirationDate(dt);
	testCookie.setHttpOnly(false);
	testCookie.setName("twittercookie");
	testCookie.setValue("1");
	testCookie.setPath ( "/" );
	testCookie.setSecure ( true);
	QList<QNetworkCookie> cookieList;
	cookieList.append(testCookie);
	cookieJar->setCookiesFromUrl ( cookieList, QUrl("https://www.twitter.com/") );
	QWebSettings::globalSettings()->setOfflineStoragePath("C:\\Data");*/
	
	iWebView->load(LoginUrl);
	qDebug()<<"Exit: LoadLoginPage()";
	   
	}
void FBLoginDialog::GetAccessToken()
	{
		qDebug()<<"Inside FBLoginDialog::GetAccessToken()";
	    Dictionary postParams;
	    iSession->stroauth_nonce = generateNONCE(43);
		iSession->stroauth_timestamp = generateTimeStamp();
		
	    postParams["oauth_consumer_key"] = kConsumerKey;
	    postParams["oauth_nonce"] = iSession->stroauth_nonce;
	    postParams["oauth_signature_method"] = "HMAC-SHA1";
	    postParams["oauth_token"] = iSession->stroauth_Token;
	    postParams["oauth_timestamp"] = iSession->stroauth_timestamp;
	    postParams["oauth_verifier"] = iSession->stroauth_verifier;
	    postParams["oauth_version"] = "1.0";
	    
	    iSession->stroauth_signature = iGetSessionRequest->generateSig(postParams);
		
	    iGetSessionRequest->connect_req("/access_token");
	}
