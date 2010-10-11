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

#include "authAppConstants.h"
#include "sessionSP.h"
#include "errorCodes.h"
#include "loginDialog.h"
#include <qdebug.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
// global

static const QString kLoginURL = "http://www.last.fm/api/auth/";//http://www.flickr.com/services/auth/"; 
//static const QString kLoginURL = "http://www.flickr.com/auth-72157623913597531";


///////////////////////////////////////////////////////////////////////////////////////////////////
void FBLoginDialog::connectToGetSession()
{
	if(iGetSessionRequest == NULL ){
		
		iGetSessionRequest = FBRequest::requestWithSession(iSession);
	
		//connect(iGetSessionRequest, SIGNAL(requestDidLoad(QVariant)), this, SLOT(requestDidLoad(QVariant)));
		//connect(iGetSessionRequest, SIGNAL(requestFailedWithNetworkError(QNetworkReply::NetworkError)), this, SLOT(requestFailedWithNetworkError(QNetworkReply::NetworkError)));
		//connect(iGetSessionRequest, SIGNAL(requestFailedWithFlickrError(FBError)), this, SLOT(requestFailedWithFlickrError(FBError)));
		
	}
	
	 iGetSessionRequest->callforToken();
}
void FBLoginDialog::connecttoGetFrob()
{
	qDebug()<<"Inside FBLoginDialog::connecttoGetFrob";
    iGetSessionRequest = FBRequest::requestWithSession(iSession);

    connect(iGetSessionRequest, SIGNAL(requestDidLoad(QVariant)), this, SLOT(requestDidLoad(QVariant)));
    connect(iGetSessionRequest, SIGNAL(requestFailedWithNetworkError(QNetworkReply::NetworkError)), this, SLOT(requestFailedWithNetworkError(QNetworkReply::NetworkError)));
    connect(iGetSessionRequest, SIGNAL(requestFailedWithFlickrError(FBError)), this, SLOT(requestFailedWithFacebookError(FBError)));

    Dictionary params;

    iGetSessionRequest->call("auth.getToken", params);
    
}
void FBLoginDialog::loadLoginPage()
{
    Dictionary getParams, postParams;
    
    getParams["api_key"] = iSession->apiKey();
    getParams["token"] = iSession->apiToken();
    QString fullURL;
    fullURL = kLoginURL;
    
    loadURL(fullURL,QNetworkAccessManager::GetOperation, getParams, postParams);

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
///////////////////////////////////////////////////////////////////////////////////////////////////
// slots for signals from FBRequest

void FBLoginDialog::requestDidLoad(const QVariant& aResult)
{
	qDebug()<<"Inside FBLoginDialog::requestDidLoad";
    QVariantHash object = aResult.toHash();
    
    QString mytoken = object.value("token").toString();
    if(mytoken.length()){
    	iSession->SaveToken(mytoken);
    	show();
    }
    qDebug()<<"Token = "<<mytoken;
    qDebug() << "Root Object key : " << object.keys().takeFirst() << endl;
    
    if(object.keys().takeFirst().compare("session") == 0);
        {
        	
    		QVariantHash childobject = object.value("session").toHash();
    		qDebug() << "Child Object data : " << childobject << endl;
    		QString userName = childobject.value("name").toString();
    		QString sessionKey = childobject.value("key").toString();
    		qDebug() << "Session Key value : " << sessionKey << endl;
    		if(sessionKey.length()){
    			iSession->SaveSession(sessionKey,userName);
    		}
      	
        }
}
/*void FBLoginDialog::requestDidLoadSession(const QVariant& aResult)
{
	qDebug()<<"Inside FBLoginDialog::requestDidLoadSession";
    bool conversionError = false;
    QVariantHash object = aResult.toHash();
    QVariantHash tagName = object.value("session").toHash();  
    QString mySession = tagName.value("key").toString();
    if(mySession.length()){
    	iSession->SaveFrobKey(mySession);
    	show();
    }
    qDebug()<<"Session Key = "<<mySession;
    qDebug() << "Root Object key : " << object.keys().takeFirst() << endl;
    
    if(object.keys().takeFirst().compare("Session") == 0);
    {
    	
		QVariantHash childobject = object.value("Session").toHash();
		qDebug() << "Child Object data : " << childobject << endl;
		QString ses = childobject.value("key").toString();
		qDebug() << "Read Token value : " << ses << endl;
		if(ses.length()){
			iSession->Savetoken(ses);
		}
  	
    }
}*/
void FBLoginDialog::requestFailedWithFlickrError (const FBError& aCode )
{
    dismissWithError(aCode);
}

void FBLoginDialog::requestFailedWithNetworkError( QNetworkReply::NetworkError aCode )
{
    dismissWithError(aCode);
}
