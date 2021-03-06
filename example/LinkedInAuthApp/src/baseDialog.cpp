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
* Base class of All dialogs
*/

#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QWebFrame>
#include <QFile>
#include <QDesktopServices>
#include <QResizeEvent>
#include <QDebug>
#include <qmessagebox.h>
#include <QNetworkAccessManager>
#include <QPropertyAnimation>
#include <qnetworkproxy.h>
#include <qdesktopwidget.h>
#include <qapplication.h>
//#include <qboxlayout.h>
#include "baseDialog.h"
#include "sessionSP.h"
#include "errorCodes.h"
#include "keys.h"
#include <QSslConfiguration>

static const QString kDefaultTitle = "Connect to Facebook";
static const QString kStringBoundary = "3i2ndDfv2rTHiSisAbouNdArYfORhtTPEefj3q2f";

static QNetworkAccessManager namanager;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

FBDialog::FBDialog() : iSession(FBSession::session()), /*iWebView ( this  ),*/ iIgnorePageLoadCompleteEvent( false )
{
    createControls();
}

FBDialog::FBDialog(FBSession* aSession) : iSession ( aSession ), /*iWebView ( this  ) ,*/ iIgnorePageLoadCompleteEvent ( false )
{
    createControls();
}
void FBDialog::createControls()
{
	iWebView = new QWebView(this);
    iWebView->page()->setNetworkAccessManager(&namanager);
    count = 0;
    iWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    /*iWebSettings =  iWebView->page()->settings();
    iWebSettings->offlineStorageDefaultQuota();*/
    
    layout = new QVBoxLayout(this);
    
    
    progressbar = new QProgressBar(this);
    
    layout->addWidget(iWebView);
    //layout->addWidget(progressbar);
    
    setLayout(layout);
    
    progressbar->setOrientation(Qt::Horizontal);
    

    connect( iWebView->page(), SIGNAL(linkClicked(const QUrl &)),
                    this, SLOT(linkClicked(const QUrl &)));

    connect ( iWebView->page(), SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));

    connect ( iWebView->page(), SIGNAL(loadStarted()), this, SLOT ( loadStarted()));
    
    connect ( iWebView->page(), SIGNAL(loadProgress(int)), this, SLOT ( loadProgress(int)));
    
    connect (iWebView->page()->networkAccessManager(),SIGNAL( authenticationRequired( QNetworkReply*, QAuthenticator*)),this,SLOT( slotAuthenticationRequired( QNetworkReply*, QAuthenticator*)));
    connect (iWebView->page()->networkAccessManager(),SIGNAL( sslErrors( QNetworkReply*,QList<QSslError>&)),this,SLOT( slotsslErrors( QNetworkReply*,QList<QSslError>&)) );
    connect (iWebView->page()->networkAccessManager(),SIGNAL( proxyAuthenticationRequired(QNetworkProxy&, QAuthenticator*)),this,SLOT( slotproxyAuthenticationRequired(QNetworkProxy&, QAuthenticator*)) );
   // connect ()
    
}
QString FBDialog::generateURL( const QString& aUrl, const QHash<QString, QString>& aParams) const
{
	qDebug()<<"Inside FBDialog::generateURL()";
    QString url ( aUrl );

    QStringList pairs;
    QHashIterator<QString, QString> i(aParams);

    while (i.hasNext()) {
        i.next();

        QUrl url (i.value());
        QString pair = i.key() + "=" + url.toEncoded();
        pairs << pair.toUtf8();
    }

    if (pairs.count())
    {
        url = url + "?" + pairs.join("&");
    }

    return url;

}

QByteArray FBDialog::generatePostBody (const QHash<QString, QString>& aParams) const
{
	qDebug()<<"Inside FBDialog::generatePostBody()";
    QByteArray body;

    if (!aParams.count())
        return body;


    QString endLine = "\r\n--" + kStringBoundary + "\r\n", kStringBoundary;

    QString tmp = "--" + kStringBoundary + "\r\n";
    body.append(tmp.toUtf8());


    QHashIterator<QString, QString> i(aParams);
    while (i.hasNext()) {
        i.next();

        tmp = "Content-Disposition: form-data; name=\"" + i.key().toUtf8() + "\"\r\n\r\n" ;
        body.append(tmp.toUtf8());
        body.append(i.value().toUtf8());
        body.append(endLine.toUtf8());
    }

    return body;
}

void FBDialog::postDismissCleanup()
{
    //accept();
    // could also be reject()?
}

void FBDialog::dismiss (bool /*aAnimated*/) {
	qDebug()<<"Inside FBDialog::dismiss()";
    dialogWillDisappear();
    iLoadingUrl.clear();

    //todo: do some animations if aAnimated == true !
    postDismissCleanup();
}


void FBDialog::dismissWithSuccess( bool aSuccess, bool aAnimated)
{
	qDebug()<<"Inside FBDialog::dismissWithSuccess()";
  if (aSuccess) {
        emit dialogDidSucceed();
  } else {
        emit dialogDidCancel();
  }

  dismiss(aAnimated);
}

void FBDialog::dismissWithError (const FBError& aError, bool aAnimated)
{
	qDebug()<<"Inside FBDialog::dismissWithError()";
	qDebug()<<"aError"<<aError.description();
  emit dialogDidFailWithError( aError );
  dismiss(aAnimated);
}
void FBDialog::slotAuthenticationRequired( QNetworkReply* reply, QAuthenticator* authenticator )
	{
		Q_UNUSED(reply)
		Q_UNUSED(authenticator)
		QMessageBox msgbox;
		QString msg ("Error!Authentication Required");
		msgbox.setText(msg);
	}
void FBDialog::slotsslErrors( QNetworkReply* reply, const QList<QSslError>& errors  )
	{
		Q_UNUSED(reply)
		Q_UNUSED(errors)
		QMessageBox msgbox;
		QString msg ("Error!SSL Error");
		msgbox.setText(msg);
	}
void FBDialog::slotproxyAuthenticationRequired( const QNetworkProxy& proxy, QAuthenticator* authenticator  )
	{
		Q_UNUSED(proxy)
		Q_UNUSED(authenticator)
		QMessageBox msgbox;
		QString msg ("Error!Proxy Authenticatio Required");
		msgbox.setText(msg);
	}
void FBDialog::cancel()
{}

void FBDialog::load() {
	qDebug()<<"Inside FBDialog::load()";
}

void FBDialog::show()
{
	qDebug()<<"Inside FBDialog::show()";
    load();
    showMaximized();
    dialogWillAppear();

}

void FBDialog::loadURL(const QString& aUrl, QNetworkAccessManager::Operation aMethod, const QHash<QString, QString>& aGetParams, const QHash<QString, QString>&  aPostParams)
{   
	Q_UNUSED(aPostParams)
	Q_UNUSED(aMethod)
	Q_UNUSED(aGetParams)
	qDebug()<<"Inside FBDialog::loadURL()";
    //proxysettings();
    iIgnorePageLoadCompleteEvent = false;

    QNetworkCookieJar* cookieJar = iWebView->page()->networkAccessManager()->cookieJar();
    QByteArray body;

    iLoadingUrl = aUrl;// generateURL(aUrl, aPostParams);/*aUrl;*////*"http://www.google.com/";

    // This "test cookie" is required by login.php, or it complains that you need to enable JavaScript
     QNetworkCookie testCookie ("test_cookie", "1");
     testCookie.setDomain ( "www.linkedin.com" );
     testCookie.setPath ( "/" );

    QList<QNetworkCookie> cookieList;
    cookieList.append(testCookie);

    cookieJar->setCookiesFromUrl ( cookieList, QUrl(iLoadingUrl) );

    QUrl url (iLoadingUrl);
    QNetworkRequest request(url);

    QSslConfiguration config( QSslConfiguration::defaultConfiguration() );

	request.setSslConfiguration( config );
   /* QString Authorization = "OAuth oauth_nonce=\"" + iSession->stroauth_nonce + "\", oauth_signature_method=\"HMAC-SHA1\", oauth_timestamp=\"" + iSession->stroauth_timestamp + "\", oauth_consumer_key=\"" + kConsumerKey + "\", oauth_signature=\"" + iSession->stroauth_signature + "\", oauth_version=\"1.0\"";
    qDebug()<<"Authorization: "<<Authorization;
    QByteArray auth = Authorization.toUtf8();
    qDebug()<<"auth:"<<auth;
    //static const char temp[] = "OAuth oauth_nonce="oqwgSYFUD87MHmJJDv7bQqOF2EPnVus7Wkqj5duNByU", oauth_callback="http%3A%2F%2Flocalhost%2Foauth_callback", oauth_signature_method="HMAC-SHA1", oauth_timestamp="1259178158", oauth_consumer_key="ABCDEFGHIJKLMNOPQRSTUVWXYZ", oauth_signature="TLQXuUzM7omwDbtXimn6bLDvfF8=", oauth_version="1.0";
    if (aMethod == QNetworkAccessManager::PostOperation)
    {
		qDebug()<<"inside : if loop";
        const QString contentType = "multipart/form-data; boundary=" + kStringBoundary;
        request.setRawHeader("Authorization",auth);
        request.setHeader (QNetworkRequest::ContentTypeHeader, contentType);     
        body = generatePostBody (aPostParams);
    }
    bool val = request.hasRawHeader("Authorization");
    qDebug()<<"Value:"<<val;*/
    proxysettings();
    
    qDebug()<< "Check URL : " << iLoadingUrl;

    //iWebView->load( request, aMethod, body);
    iWebView->load(iLoadingUrl);
    
}
void FBDialog::proxysettings()
{
#ifdef EMULATORTESTING
	qDebug()<<"proxysettings";
	
	// Reading the keys, CSM Stubbed - START
	QFile file("c:\\data\\DoNotShare.txt");
	if (!file.open(QIODevice::ReadOnly))
		{
		qDebug()<<"File to read the windows username and password could not be opened, returning!!!";
		return;
		}
	
	QByteArray arr = file.readAll();
	QList<QByteArray> list = arr.split(' ');
	file.close();
	
	QString username(list[0]);
	QString password(list[1]);
	
    QString httpProxy = "10.1.0.214";//ipwproxy.sasken.com
    QString httpPort = "3128";

    QString httpUser =username;/* This could be taken thru an QDialog implmentation to remove the Hard coding */
    QString httpPass =password;/* This could be taken thru an QDialog implmentation to remove the Hard coding */

    /*==Classes used from Network Module==*/
    QNetworkProxy proxy;

    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName(httpProxy);
    proxy.setPort(httpPort.toInt());
    proxy.setUser(httpUser);
    proxy.setPassword(httpPass);

    QNetworkProxy::setApplicationProxy(proxy);
#endif
}
void FBDialog::dialogWillAppear() {}

void FBDialog::dialogWillDisappear() {}

void FBDialog::dialogDidSucceed (const QUrl& /*aUrl*/) {
  dismissWithSuccess(true,true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FBDialog::linkClicked ( const QUrl & url )
 {

        qDebug() << "Loading the url: " <<  url;
        
        proxysettings();
			
        iWebView->load(url);
}

void FBDialog::loadStarted()
{
    qDebug() << "Load started: " << iWebView->url();
    iWebView->repaint();
    QNetworkCookie cookie;
    cookie.setSecure(true);
    layout->addWidget(progressbar);
    progressbar->setVisible(true);
}
void FBDialog::loadProgress(int progress)
{
	progressbar->setValue(progress);
}
void FBDialog::GetSessionKey(const QUrl& aUrl)
{
	Q_UNUSED(aUrl)
}
void FBDialog::FetchKeyFromUrl(const QUrl& aUrl)
{
	Q_UNUSED(aUrl)
}
void FBDialog::loadFinished ( bool ok )
{
    qDebug() << "Load " << (ok ? "" : "un") << "successfull for: " << iWebView->url();
    progressbar->setVisible(false);
    layout->removeWidget(progressbar);
	QNetworkCookieJar kl;
	
    if (ok)
    {
		QString myhtml  = iWebView->page()->currentFrame()->toHtml();
		qDebug()<<"MyHtml:"<<myhtml.trimmed();
		QString PAth = iWebView->url().toString();
		PAth = PAth.mid(0,PAth.indexOf("?"));
		
		qDebug() << "Path is : " << PAth;
		
    }
    else
    {
        if (iIgnorePageLoadCompleteEvent)
            return;

        FBError err;
        dismissWithError(err, true);
    }
}
void FBDialog::GetAccessToken()
{

}
