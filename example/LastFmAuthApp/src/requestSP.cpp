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
* class to handle calls to rest Server API's
*/

#define EMULATORTESTING 1

#include "requestSP.h"
#include "sessionSP.h"
#include "xmlParser.h"
#include "errorCodes.h"

#include <QNetworkRequest>
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QCryptographicHash>
#include <QtAlgorithms>
#include <QDebug>
#include <qnetworkproxy.h>
#include "keys.h"
///////////////////////////////////////////////////////////////////////////////////////////////////
// global

static const QString kAPIVersion = "1.0";
static const QString kAPIFormat = "XML";
static const QString kStringBoundary = "3i2ndDfv2rTHiSisAbouNdArYfORhtTPEefj3q2f";


///////////////////////////////////////////////////////////////////////////////////////////////////

static bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Static class functions

FBRequest* FBRequest::request()
{
    return FBRequest::requestWithSession(FBSession::session());
}

FBRequest* FBRequest::requestWithSession (FBSession* aSession)
{
    FBRequest* request = new FBRequest(aSession);
    return request;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// instance public functions
FBRequest::FBRequest(FBSession* aSession) : iSession ( aSession ), iNetworkAccessManager ( this )
{}

const QDateTime& FBRequest::timeStamp() const
{
    return iTimestamp;
}

void FBRequest::connect()
{
	qDebug()<<"Inside FBRequest::connect";
    QString url ;
    url = generateGetURL();
    qDebug() << "Complete URL  : " << url;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    if (iMethod.length())
    {
        pbar = new progressbar;
        pbar->show();
		#ifdef __WINSCW__
			proxysettings();
		#endif
        QNetworkReply* reply = iNetworkAccessManager.get(request);
        QObject::connect(reply, SIGNAL(finished()),  this, SLOT(networkReplyFinished()));
        QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                         this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
    }
}
void FBRequest::proxysettings()
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
///////////////////////////////////////////////////////////////////////////////////////////////////
// instance private functions
QString FBRequest::md5(const QString& aData)
{
    QByteArray byteArray;
    byteArray.insert(0, aData.toUtf8());

    QByteArray md5Hash = QCryptographicHash::hash(byteArray,QCryptographicHash::Md5 ).toHex();
    QString returnString ( md5Hash );

    return returnString;
}

bool FBRequest::isSpecialMethod() const {
    return ( iMethod.compare("facebook.auth.getSession", Qt::CaseInsensitive) == 0
             ||   iMethod.compare("facebook.auth.createToken", Qt::CaseInsensitive) == 0 );
}

QString FBRequest::urlForMethod (const QString& aMethod) const {

	Q_UNUSED(aMethod)
	return iSession->apiURL();
}

QString FBRequest::generateGetURL() const
{
    const QUrl url(iUrl);
    const QString queryPrefix = url.hasQuery() ? "&" : "?";

    QStringList pairs;
    DictionaryIterator i(iParams);

    while (i.hasNext()) {
        i.next();
        pairs << i.key().toUtf8() + "=" + i.value().toUtf8();
    }

    return iUrl + queryPrefix + pairs.join("&");
}

QString FBRequest::generateCallId() const {
    QDateTime dateTime = QDateTime::currentDateTime();
    uint secs = dateTime.toTime_t();
    QString result = QString::number(secs, 10);
    return result;
}

QString FBRequest::generateSig(Dictionary Params)
{
    QString joined;
    QStringList keys = Params.keys();

    qSort(keys.begin(), keys.end(), caseInsensitiveLessThan);

    QListIterator<QString> i(keys);
    
    joined.append(iSession->apiSecret().toUtf8());
    
    while (i.hasNext())
    {
        const QString key = i.next();
        joined.append(key.toUtf8());
        //joined.append("=");
        joined.append(Params.value(key).toUtf8());
    }

    return md5(joined);
}

void FBRequest::generatePostBody( QByteArray& body )
{
    QString endLine = "\r\n--" + kStringBoundary + "\r\n";
    body.append( "--" + kStringBoundary.toUtf8() + "\r\n" ) ;


    DictionaryIterator i (iParams);

    while (i.hasNext())
    {
        i.next();

        body.append("Content-Disposition: form-data; name=\"" + i.key().toUtf8() + "\"\r\n\r\n" );
        body.append(i.value().toUtf8());
        body.append(endLine.toUtf8());
    }


    if (iDataParam.size())
    {
        if (iDataParamPicture)
        {
            body.append("Content-Disposition: form-data; filename=\"photo\"\r\n" );
            body.append("Content-Type: image/png\r\n\r\n" );
        }
        else
        {
            body.append("Content-Disposition: form-data; filename=\"data\"\r\n");
            body.append("Content-Type: content/unknown\r\n\r\n");
        }

        body.append(iDataParam);
        body.append(endLine.toUtf8());

    }
}

void FBRequest::handleResponseData( const QByteArray& aResponseData )
{
	qDebug()<<"Inside FBRequest::handleResponseData";
    FBError error;
    QVariant result =  parseXMLResponse( aResponseData, error);
    
    qDebug() << "parsed result : " << result;
    
    if (error.code() != 0)
    {
        emit requestFailedWithFlickrError(error);
    }
    else
    {
		emit requestDidLoad(result);
    }
    delete pbar;
}

void FBRequest::post( const QString& aUrl, const Dictionary& aParams)
{
    iUrl = aUrl;
    iParams = aParams;

    iSession->send(this);
}

void FBRequest::cancel()
{

}


void FBRequest::call (const QString& aMethod, const Dictionary& aParams)
{
	qDebug()<<"Inside FBRequest::call";
    QByteArray dataParams;
    callWithDataParams(aMethod, aParams, dataParams, false);

}
void FBRequest::callforToken ()
{
	
	iUrl = iSession->apiURL();
	iMethod = "auth.getSession";

	Dictionary params;
	params["method"] = iMethod;
	params["api_key"] = iSession->apiKey();
	params["token"] = iSession->apiToken();
	QString qstr = "api_key" + iSession->apiKey() + "methodauth.getSessiontoken" + iSession->apiToken() + iSession->apiSecret();
	qDebug()<<"Api Key"<<iSession->apiKey(); 
	qDebug()<<"Api Token"<<iSession->apiToken(); 
	qDebug()<<"Api Secret"<<iSession->apiSecret(); 
	qDebug()<<"qstr - sig"<<qstr<<"\n";
	params["api_sig"] = md5(qstr);
	
	iParams = params;
	
	iSession->send(this);
}

void FBRequest::callWithDataParams (const QString& aMethod, const Dictionary& aParams, const QByteArray& aDataParam, bool aDataParamPicture)
{
	qDebug()<<"Inside FBRequest::callWithDataParams";
    iUrl = urlForMethod(aMethod);
    iMethod = aMethod;
    iParams = aParams;
    iDataParam = aDataParam;
    iDataParamPicture = aDataParamPicture;

    
    iParams["method"] = iMethod;
    iParams["api_key"] = iSession->apiKey();
    
    // XXX: workaround what seems to be a Qt bug with the extras-devel libraries.
    QString signature = generateSig(iParams);
    iParams["api_sig"] = signature;
    // XXX: end workaround.
    
    iSession->send(this);
}


QVariant FBRequest::parseXMLResponse ( const QByteArray& aResponseData, FBError& aError)
{
	qDebug()<<"Inside FBRequest::parseXMLResponse";
    QXmlInputSource input;
    input.setData(aResponseData);

    FBXMLHandler handler;
    QXmlSimpleReader parser;
    parser.setContentHandler(&handler);
    bool result = parser.parse(&input);

    QVariant rootObject = handler.rootObject();
    

    if (handler.parseError() || !result)
    {
        aError.setCode( FBRESPONSE_PARSE_ERROR );
        aError.setDescription("parser was unable to parse the xml response from facebook server.");

        return QVariant();
    }
    else if (handler.rootName().compare("error_response")==0)
    {
        QVariantHash errorDict =  rootObject.toHash();

        bool result;
        int errorCode = errorDict.value("error_code").toInt(&result);

        aError.setCode( errorCode );
        aError.setDescription( errorDict.value("error_msg").toString() );

        return rootObject;
    }
    else
    {
        return rootObject;
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// instance provate slots
void FBRequest::networkReplyError ( QNetworkReply::NetworkError aCode )
{
    emit requestFailedWithNetworkError(aCode );
}

void FBRequest::networkReplyFinished ()
{
	qDebug()<<"Inside FBRequest::networkReplyFinished";
    QNetworkReply* reply = static_cast<QNetworkReply*> ( sender() );
    QByteArray responseData = reply->readAll();

    qDebug() << "Response Data(QByteArray) : " << responseData;
    QString str(responseData);
    qDebug() << "Response Data(QString) : " << str;
    handleResponseData ( responseData );

}
