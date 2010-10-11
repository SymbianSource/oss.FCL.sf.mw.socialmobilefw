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

#include "hash.h"

#include <keys.h>
#include <qnetworkproxy.h>
#include <QSslConfiguration>
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
	qDebug()<<"Inside FBRequest::connect()";
    //emit requestLoading();

    /*QString url ;
    url = iUrl;
    qDebug()<<"url"<<url;*/
   
    QNetworkRequest request;
   
    QSslConfiguration config( QSslConfiguration::defaultConfiguration() );
   	request.setSslConfiguration( config );
    
   	//request.setUrl(QUrl(url));
   	QUrl myurl = kRequestTokenUrl;
    request.setUrl(myurl);

    /* from the Qt docs on QNetworkAccessManager:
       QNetworkAccessManager by default does not have a set cache.
       Qt provides a simple disk cache, QNetworkDiskCache, which can be used.

       However we will not use it.*/
    QString Authorization = "OAuth oauth_nonce=\"" + iSession->stroauth_nonce + "\", oauth_signature_method=\"HMAC-SHA1\", oauth_timestamp=\"" + iSession->stroauth_timestamp + "\", oauth_consumer_key=\"" + kConsumerKey + "\", oauth_signature=\"" + iSession->stroauth_signature.toAscii().toPercentEncoding() + "\", oauth_version=\"1.0\"";
	qDebug()<<"Authorization: "<<Authorization;
	QByteArray auth = Authorization.toUtf8();
	qDebug()<<"auth:"<<auth;
	request.setRawHeader("Authorization",auth);
	const QString contentType = "multipart/form-data; boundary=" + kStringBoundary;
	request.setHeader (QNetworkRequest::ContentTypeHeader, contentType); 
	//request.setRawHeader("User-Agent", kUserAgent);
    //request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
   // request.setAttribute(QNetworkRequest::CacheSaveControlAttribute, false);

    //iTimestamp = QDateTime::currentDateTime();

    //if (iMethod.length())
    //{
      //  const QString contentType = "multipart/form-data; boundary=" + kStringBoundary;
     //   request.setRawHeader("Content-Type", contentType.toUtf8());

        /* connect all signals from iNetWorkAccessManager to this */
        QByteArray postBody ;
        generatePostBody (postBody);
        postBody = "";
        pbar = new progressbar;
        pbar->show();
        proxysettings();
        QNetworkReply* reply = iNetworkAccessManager.post(request, postBody);

        QObject::connect(reply, SIGNAL(finished()),  this, SLOT(networkReplyFinished()));
        QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
                         this, SLOT(networkReplyError(QNetworkReply::NetworkError)));

    //}
}
void FBRequest::proxysettings()
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
///////////////////////////////////////////////////////////////////////////////////////////////////
// instance private functions
QString FBRequest::sha1()
{
	qDebug()<<"Insha1";
	CSHA1* sha=CSHA1::NewL();
	TBuf8<100> keyVal;
	keyVal.Copy(_L8("sasken"));
	CHMAC* hmac=CHMAC::NewL(keyVal,sha);
	//TBuf8<1024> baseString;
	TPtrC8 hashedSig(hmac->Hash(_L8("sasken")));	
	TBuf8<1024>bufr;
		bufr.Copy(hashedSig);
		QByteArray digest = QByteArray::fromRawData((char*)bufr.Ptr(),bufr.Length());
		qDebug()<<"digest.toHex();"<<digest.toHex();
	
	if(hmac)
	{
	    delete hmac;
	    hmac=NULL;
	}
/*	TBuf8<1024>bufr;
	bufr.Copy(hashedSig);
	QByteArray digest = QByteArray::fromRawData((char*)bufr.Ptr(),bufr.Length());
	qDebug()<<"digest.toHex();"<<digest.toHex();
	qDebug()<<"digest"<<digest;*/
	return digest.toHex();
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
	qDebug()<<"Inside FBRequest::generateGetURL()";
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
/*
 *   
  httpMethod + "&" +
  url_encode(  base_uri ) + "&" +
  sorted_query_params.each  { | k, v |
      url_encode ( k ) + "%3D" +
      url_encode ( v )
  }.join("%26")
 * 
 */
QString FBRequest::generateSig(Dictionary Params)
{
	QString joined;
	joined = "POST";
	joined += "&";
	joined += kRequestTokenUrl.toAscii().toPercentEncoding() + "&";
	
	QStringList keys = Params.keys();
	qSort(keys.begin(), keys.end(), caseInsensitiveLessThan);
	QListIterator<QString> i(keys);
	int count = 0;
	while (i.hasNext())
	{
		count++;
		if(count > 1)
			joined.append("%26");
		const QString key = i.next();
		joined.append(key.toAscii().toPercentEncoding());
		joined.append("%3D");
		joined.append(Params.value(key).toAscii().toPercentEncoding());
	}
	qDebug()<<"joined"<<joined;
	//uncomment for testing key
	//joined = "POST&https%3A%2F%2Fapi.twitter.com%2Foauth%2Frequest_token&oauth_callback%3Dhttp%253A%252F%252Flocalhost%253A3005%252Fthe_dance%252Fprocess_callback%253Fservice_provider_id%253D11%26oauth_consumer_key%3DGDdmIQH6jhtmLUypg82g%26oauth_nonce%3DQP70eNmVz8jvdPevU3oJD2AfF7R7odC2XJcn4XlZJqk%26oauth_signature_method%3DHMAC-SHA1%26oauth_timestamp%3D1272323042%26oauth_version%3D1.0";
	TBuf<1024> sid(joined.utf16());
	
	QByteArray digest1 = QByteArray::fromRawData((char*)sid.Ptr(),sid.Length());
	qDebug()<<"digest1: "<<digest1;
	TBuf8<1024>buff;//(sid);
	buff.Copy(sid);
	QByteArray digest2 = QByteArray::fromRawData((char*)buff.Ptr(),buff.Length());
	qDebug()<<"digest2: "<<digest2;

	//signature
	CSHA1* sha=CSHA1::NewL();
	//uncomment for testing key
	//QByteArray key= "MCD8BKwGdgPHvAuvgvz4EQpqDAtx89grbuNMRd7Eh98";
	QByteArray key= kConsumerSecret.toUtf8();
	key = key.toPercentEncoding();
	key.append("&");
	key.append(iSession->stroauth_TokenSecret.toUtf8().toPercentEncoding());
	qDebug()<<"Key:"<<key;
	TPtrC8 myDataDescriptor( reinterpret_cast<const TText8*> (key.constData()),key.size());
	 
	//Take a copy of the data
	HBufC8* buffer12 = HBufC8::New(myDataDescriptor.Length());
	 
	Q_CHECK_PTR(buffer12);
	buffer12->Des().Copy(myDataDescriptor  );
	TPtr8 tptr = buffer12->Des();
	
	TBuf8<100> keyVal;
	keyVal.Copy(tptr);

	CHMAC* hmac=CHMAC::NewL(keyVal,sha);

	TPtrC8 hashedSig(hmac->Hash(buff));	
	TBuf8<1024>bufr;
		bufr.Copy(hashedSig);
		QByteArray digest = QByteArray::fromRawData((char*)bufr.Ptr(),bufr.Length());
		qDebug()<<"digest.toHex() "<<digest.toHex();
		QByteArray base64 = digest.toBase64();
		qDebug()<<"Base64 Encoded  "<<base64;
	if(hmac)
	{
		delete hmac;
		hmac=NULL;
	}

	return base64;
}

void FBRequest::generatePostBody( QByteArray& body )
{
	qDebug()<<"Inside FBRequest::generatePostBody()";
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
	qDebug()<<"Inside FBRequest::handleResponseData()";
    FBError error;
    QVariant result =  parseXMLResponse( aResponseData, error);
    if (error.code() != 0)
    {
        emit requestFailedWithFacebookError(error);
    }
    else
    {
        emit requestDidLoad(result);
    }
    
    delete pbar;
}

void FBRequest::post( const QString& aUrl, const Dictionary& aParams)
{
	qDebug()<<"Inside FBRequest::post()";
    iUrl = aUrl;
    iParams = aParams;

    iSession->send(this);
}

void FBRequest::cancel()
{
	qDebug()<<"Inside FBRequest::cancel()";
}


void FBRequest::call (const QString& aMethod, const Dictionary& aParams)
{
    QByteArray dataParams;
    callWithDataParams(aMethod, aParams, dataParams, false);

}

void FBRequest::callWithDataParams (const QString& aMethod, const Dictionary& aParams, const QByteArray& aDataParam, bool aDataParamPicture)
{
	Q_UNUSED(aParams)
	Q_UNUSED(aDataParam)
	Q_UNUSED(aDataParamPicture)
    iUrl = urlForMethod(aMethod);
    iUrl +=  aMethod;
    qDebug()<<"URL inside call with Data Params"<<iUrl;
    //iMethod = aMethod;
    //iParams = aParams;
  /*  iParams["oauth_nonce"] = iSession->stroauth_nonce;
    iParams["oauth_signature_method"] = "HMAC-SHA1";
    iParams["oauth_timestamp"] = iSession->stroauth_timestamp;
    iParams["oauth_consumer_key"] = kConsumerKey;
    iParams["sig"]=iSession->stroauth_signature;
    iParams["oauth_version"] = "1.0";*/
    //iDataParam = aDataParam;
   // iDataParamPicture = aDataParamPicture;

  //  iParams["method"] = iMethod;
  //  iParams["api_key"] = iSession->apiKey();
  //  iParams["v"] = kAPIVersion;
  //  iParams["format"] = kAPIFormat;

   /* if (!isSpecialMethod())
    {
        iParams["session_key"] = iSession->sessionKey();
        iParams["call_id"] = generateCallId();

        if (iSession->sessionSecret().length())
        {
            iParams["ss"] = "1";
        }
    }*/

    // XXX: workaround what seems to be a Qt bug with the extras-devel libraries.
   // QString signature = generateSig(iParams);
   // iParams["sig"] = signature;
    // XXX: end workaround.
    
    iSession->send(this);
}


QVariant FBRequest::parseXMLResponse ( const QByteArray& aResponseData, FBError& aError)
{
	qDebug()<<"Inside FBRequest::handleResponseData()";
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
qDebug()<<"Inside FBRequest::networkReplyFinished()";
    QNetworkReply* reply = static_cast<QNetworkReply*> ( sender() );
    QByteArray responseData = reply->readAll();
    if(responseData != "")
    	{
			qDebug()<<"response data:"<<responseData;
			QList<QByteArray>MyString = responseData.split('&');
			QList<QByteArray>token = MyString[0].split('=');
			qDebug()<<token.at(1);
			iSession->stroauth_Token=token.at(1);
			QList<QByteArray>token_secret = MyString[1].split('=');
			qDebug()<<token_secret.at(1);
			iSession->stroauth_TokenSecret=token_secret.at(1);
		 
			qDebug()<<"iSession->stroauth_Token"<<iSession->stroauth_Token;
			if(iSession->stroauth_Token != "")
			{
				emit LoadLoginPage();
				qDebug()<<"Signal:LoadLoginPage() is emitted";
			}
    	}
}
void FBRequest::networkReplyFinishedNew ()
{
qDebug()<<"Inside FBRequest::networkReplyFinishedNew()";
    QNetworkReply* reply = static_cast<QNetworkReply*> ( sender() );
    QByteArray responseData = reply->readAll();
    if(responseData != "")
    	{
			qDebug()<<"response data:"<<responseData;
			QList<QByteArray>MyString = responseData.split('&');
			QList<QByteArray>token = MyString[0].split('=');
			qDebug()<<token.at(1);
			iSession->stroauth_Token=token.at(1);
			QList<QByteArray>token_secret = MyString[1].split('=');
			qDebug()<<token_secret.at(1);
			iSession->stroauth_TokenSecret=token_secret.at(1);
		 
			qDebug()<<"iSession->stroauth_Token"<<iSession->stroauth_Token;
			qDebug()<<"iSession->stroauth_TokenSecret"<<iSession->stroauth_TokenSecret;
			iSession->save();
    	}
}
void FBRequest::connect_req(QString uri)
	{
		qDebug()<<"Inside FBRequest::connect_req()";

	    QNetworkRequest request;
	   
	    QSslConfiguration config( QSslConfiguration::defaultConfiguration() );
	   	request.setSslConfiguration( config );
	   	QUrl myurl;
	   	QString Authorization;
	   	if(uri == "/request_token")
	   		{
				myurl = kRequestTokenUrl;
				Authorization = "OAuth oauth_nonce=\"" + iSession->stroauth_nonce + "\", oauth_signature_method=\"HMAC-SHA1\", oauth_timestamp=\"" + iSession->stroauth_timestamp + "\", oauth_consumer_key=\"" + kConsumerKey + "\", oauth_signature=\"" + iSession->stroauth_signature.toAscii().toPercentEncoding() + "\", oauth_version=\"1.0\"";
	   		}
	   	else
	   		{
				myurl = kAccessTokenUrl;
				Authorization = "OAuth oauth_nonce=\"" + iSession->stroauth_nonce + "\", oauth_signature_method=\"HMAC-SHA1\", oauth_timestamp=\"" + iSession->stroauth_timestamp + "\", oauth_consumer_key=\"" + kConsumerKey + "\", oauth_token=\"" + iSession->stroauth_Token + "\", oauth_verifier=\"" + iSession->stroauth_verifier + "\", oauth_signature=\"" + iSession->stroauth_signature.toAscii().toPercentEncoding() + "\", oauth_version=\"1.0\"";
	   		}
	    request.setUrl(myurl);


	    qDebug()<<"myurl:"<<myurl;
		qDebug()<<"Authorization: "<<Authorization;
		QByteArray auth = Authorization.toUtf8();
		qDebug()<<"auth:"<<auth;
		request.setRawHeader("Authorization",auth);
		const QString contentType = "multipart/form-data; boundary=" + kStringBoundary;
		request.setHeader (QNetworkRequest::ContentTypeHeader, contentType); 

		QByteArray postBody ;
		generatePostBody (postBody);
		postBody = "";
		pbar = new progressbar;
		pbar->show();
		proxysettings();
		QNetworkReply* reply = iNetworkAccessManager.post(request, postBody);

		QObject::connect(reply, SIGNAL(finished()),  this, SLOT(networkReplyFinishedNew()));
		QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(networkReplyError(QNetworkReply::NetworkError)));

   
	}
