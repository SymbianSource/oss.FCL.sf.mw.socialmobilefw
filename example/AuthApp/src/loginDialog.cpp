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
* Login Dialog class of Authentication Application
*/

#include "authAppConstants.h"
#include "sessionSP.h"
#include "errorCodes.h"
#include "loginDialog.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
// global

static const QString kLoginURL = "http://www.facebook.com/login.php";

///////////////////////////////////////////////////////////////////////////////////////////////////
void FBLoginDialog::connectToGetSession(const QString& aToken)
{
    iGetSessionRequest = FBRequest::requestWithSession(iSession);

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

void FBLoginDialog::loadLoginPage()
{
    Dictionary getParams, postParams;

    getParams["fbconnect"] = "1";
	
    #ifdef __WINSCW__ 
		getParams["connect_display"] = "touch";
	#else
		getParams["connect_display"] = "popup";
	#endif
		
    getParams["api_key"] = iSession->apiKey();
    getParams["next"] = KSuccessurl;
    getParams["cancel_url"] = "http://www.facebook.com/connect/login_failure.html";
    getParams["req_perms"] = "read_stream,publish_stream,offline_access";

    loadURL(kLoginURL,QNetworkAccessManager::GetOperation, getParams, postParams);

}
///////////////////////////////////////////////////////////////////////////////////////////////////

FBLoginDialog::FBLoginDialog() : FBDialog () {}

FBLoginDialog::FBLoginDialog(FBSession* aSession) : FBDialog(aSession) {}

///////////////////////////////////////////////////////////////////////////////////////////////////
// FBDialog

void FBLoginDialog::load()
{
    loadLoginPage();
}

void FBLoginDialog::dialogWillDisappear()
{
  iGetSessionRequest->cancel();

  if (!iSession->isConnected())
  {
      iSession->cancelLogin();
  }
}

void FBLoginDialog::GetSessionKey(const QUrl& aUrl)
{
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

///////////////////////////////////////////////////////////////////////////////////////////////////
// slots for signals from FBRequest

void FBLoginDialog::requestDidLoad(const QVariant& aResult)
{
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
    dismissWithError(aCode, true);
}

void FBLoginDialog::requestFailedWithNetworkError( QNetworkReply::NetworkError aCode )
{
    dismissWithError(aCode, true);
}
