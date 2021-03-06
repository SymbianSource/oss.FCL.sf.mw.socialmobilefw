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


#ifndef FB_LOGIN_DIALOG_H_
#define FB_LOGIN_DIALOG_H_

#include "baseDialog.h"
#include "requestSP.h"
#include <QNetworkReply>

// FORWARD DECLARATIONS
class FBRequest;

class FBLoginDialog : public FBDialog
{
    Q_OBJECT

private:
    FBRequest* iGetSessionRequest;
	
private slots:
    /* slots for handling signals from FBRequest iGetSessionRequest */
    void requestDidLoad (const QVariant& aResult);
    //void requestDidLoadSession (const QVariant& aResult);
    void requestFailedWithFlickrError ( const FBError& aError );
    void requestFailedWithNetworkError( QNetworkReply::NetworkError code );

public:
    FBLoginDialog(FBSession* aSession);
    FBLoginDialog();
    void connecttoGetFrob();
    void load();

private:
    void connectToGetSession();
    
    void loadLoginPage();
	
 };
#endif
