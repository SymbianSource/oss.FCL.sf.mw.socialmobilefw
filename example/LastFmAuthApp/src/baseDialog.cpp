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
* Base class of All dialogs
*/

#define EMULATORTESTING 1

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

static const QString kDefaultTitle = "Connect to LastFm";
static const QString kStringBoundary = "3i2ndDfv2rTHiSisAbouNdArYfORhtTPEefj3q2f";
/*static const QString kMinitokensuccessURL = "http://www.last.fm/api/grantaccess";//http://www.last.fm/api/auth/";
static const QString kUrlChk = "http://www.last.fm/home";*/
//static const QString kInvalidUrl = "http://mlogin.yahoo.com/w/login";

static QNetworkAccessManager namanager;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

FBDialog::FBDialog() : iSession(FBSession::session())
{
    createControls();
}

FBDialog::FBDialog(FBSession* aSession) : iSession ( aSession )
{
    createControls();
}
void FBDialog::createControls()
{
	flag = true;
	iWebView = new QWebView(this);
	iWebSettings =  iWebView->page()->settings();
	iWebSettings->setAttribute(QWebSettings::JavascriptEnabled, false);
	iWebSettings->setAttribute(QWebSettings::PrintElementBackgrounds,false);
	iWebSettings->setAttribute(QWebSettings::DnsPrefetchEnabled,true);
	iWebSettings->setAttribute(QWebSettings::AutoLoadImages,false);
	//iWebSettings->setAttribute(QWebSettings::JavaEnabled,false);

    iWebView->page()->setNetworkAccessManager(&namanager);

    iWebView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    
    layout = new QVBoxLayout(this);
    
    progressbar = new QProgressBar(this);
    
    //scrollbar = new QScrollArea(this);
    layout->addWidget(iWebView);
    //layout->addWidget(scrollbar);
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

}
QString FBDialog::generateURL( const QString& aUrl, const QHash<QString, QString>& aParams) const
{
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
    dialogWillDisappear();
    iLoadingUrl.clear();

    //todo: do some animations if aAnimated == true !
    postDismissCleanup();
}


void FBDialog::dismissWithSuccess( bool aSuccess, bool aAnimated)
{
  if (aSuccess) {
        emit dialogDidSucceed();
  } else {
        emit dialogDidCancel();
  }

  dismiss(aAnimated);
}

void FBDialog::dismissWithError (const FBError& aError)
{
	Q_UNUSED(aError)
	QMessageBox msgbox;
	QString msg ("Error!Please try again!");
	msgbox.setText(msg);
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

void FBDialog::load() {}

void FBDialog::show()
{
    load();
    showMaximized();
    dialogWillAppear();

}

void FBDialog::loadURL(const QString& aUrl, QNetworkAccessManager::Operation aMethod, const QHash<QString, QString>& aGetParams, const QHash<QString, QString>&  aPostParams)
{   
	Q_UNUSED(aMethod)
	Q_UNUSED(aPostParams)
    QNetworkCookieJar* cookieJar = iWebView->page()->networkAccessManager()->cookieJar();
    QByteArray body;

   iLoadingUrl = generateURL(aUrl, aGetParams);

    QUrl url (iLoadingUrl);
    QNetworkRequest request(aUrl);
 
	proxysettings();
	if(flag)
		{
			urlShoot = iLoadingUrl;
			qDebug()<< "urlShoot : " << urlShoot;
		}
    
    qDebug()<< "Check URL : " << iLoadingUrl;
    iWebView->load(url);
}
void FBDialog::proxysettings()
{
#ifdef EMULATORTESTING
	qDebug()<<"proxysettings";
	
	// Reading the keys, CSM Stubbed - START
	QFile file("c://data//DoNotShare.txt");
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
	qDebug() << "Enter: FBDialog::loadStarted()";
    qDebug() << "Load started: " << iWebView->url();
    qDebug() << "Before: layout->addWidget(progressbar);";
    layout->addWidget(progressbar);
    qDebug() << "After: layout->addWidget(progressbar);";
    qDebug() << "Before: progressbar->setVisible(true);";
    progressbar->setVisible(true);
    qDebug() << "After: progressbar->setVisible(true);";
    qDebug() << "Exit: FBDialog::loadStarted()";
    
}
void FBDialog::loadProgress(int progress)
{
	qDebug()<<"Enter: FBDialog::loadProgress(int progress)";
	progressbar->setValue(progress);
	qDebug()<<"Exit: FBDialog::loadProgress(int progress)";
}

void FBDialog::GetSessionKey(const QUrl& aUrl)
{
	Q_UNUSED(aUrl)
}
void FBDialog::connectToGetSession()
{

}
void FBDialog::loadFinished ( bool ok )
{
	qDebug()<<"Enter: FBDialog::loadFinished ( bool ok )";
    qDebug() << "Load " << (ok ? "" : "un") << "successfull for: " << iWebView->url();
    layout->removeWidget(progressbar);
    progressbar->setVisible(false);
    if (ok)
    {
        QString url = iWebView->url().toString();
        qDebug() << "Loaded URL " << url;
        qDebug() << "To be compared URL " << kMinitokensuccessURL;
        if(url.compare(kMinitokensuccessURL)==0)
        	{
				iWebView->close();
				QMessageBox msgbox;
				QString msg ("Complete the Authorization?");
				msgbox.setText(msg);
				msgbox.exec();
				
				connectToGetSession();				
				
        	}
        else if(url.compare(kUrlChk)==0)
        	{
				iWebView->load(urlShoot);
        	}
    }
    else
    {
        FBError err;
        dismissWithError(err);
    }
}
