/**
 * Copyright (c) 2010 Sasken Communication Technologies Ltd.
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of the "Eclipse Public License v1.0" 
 * which accompanies  this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html"
 *
 * Initial Contributors:
 * Chandradeep Gandhi, Sasken Communication Technologies Ltd - Initial contribution
 *
 * Contributors:
 * Manasij Roy, Nalina Hariharan
 */

#include "smfclientsymbian.h"
#include <e32cmn.h>
#include <QtGlobal>
#include <e32svr.h>
#include <e32base.h>
#include <QByteArray>
#include "smfglobal.h"
#include "smfclientglobal.h"
//testing purpose
#include <QProcess>
#include <QTextStream>
#include <QFile>
//testing end
// For starting the server process
static TInt StartServerL();
static TInt CreateServerProcessL();

CSmfClientSymbian::CSmfClientSymbian(smfObserver* aObserver)
: iObserver(aObserver),CActive( EPriorityStandard ),iDataPtr(NULL, 0, 0)
	{
	CActiveScheduler::Add(this);
	}

CSmfClientSymbian* CSmfClientSymbian::NewL(smfObserver* aObserver )
    {
	CSmfClientSymbian* self = NewLC( aObserver );
    CleanupStack::Pop( self );
    return( self ) ;
    }

CSmfClientSymbian* CSmfClientSymbian::NewLC(smfObserver* aObserver )
    {
	CSmfClientSymbian* self =
        new ( ELeave ) CSmfClientSymbian( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

void CSmfClientSymbian::ConstructL()
    {
	writeLog("CSmfClientSymbian::ConstructL");

    User::LeaveIfError(iSession.connectToServer());
    }
void CSmfClientSymbian::writeLog(QString log) 
	{
#ifdef WRITE_LOG
	QFile file("c:\\data\\SmfClientLogs.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
	         return;
    QTextStream out(&file);
    out << log << "\n";
    file.close();
#endif
	}

void CSmfClientSymbian::DoCancel()
	{
	Cancel();
	}

TInt CSmfClientSymbian::RunError(TInt aError)
	{
	QString log2("CSmfClientSymbian::RunError=");
	
	log2 += QString::number(aError);
	writeLog(log2);
	return KErrNone;
	}

void CSmfClientSymbian::RunL()
	{
	QString log2("CSmfClientSymbian::RunL=");
	log2 += QString::number(iStatus.Int());
	writeLog(log2);
    switch ( iStatus.Int() )
        {
        case KErrCancel:
            // The request was canceled
        	writeLog("KErrCancel");
            break ;

        case KErrNotReady:
        	writeLog("KErrNotReady");
        	break;

        default:
        	{
        	writeLog("RunL:-SmfContactRetrievePostsComplete");
        	//This contains error followed by actual data
        	QByteArray receivedData(reinterpret_cast<const char*>(iSession.iDataPtr.Ptr()),iSession.iDataPtr.Length());
        	QString errStr;
        	SmfError errVal;
        	int errInt;
        	QByteArray data;
        	QDataStream reader(&receivedData,QIODevice::ReadOnly);
        	reader>>errInt;
        	errVal = (SmfError)errInt;
        	reader>>data;
        	SmfRequestTypeID opcode = (SmfRequestTypeID)iSession.getLastRequest();
        	iObserver->resultsAvailable(data,opcode,errVal);
        	}
        	break;
        }
	}

QByteArray CSmfClientSymbian::sendRequest(QString aInterfaceName,
		 SmfRequestTypeID requestType)
	{
	//This will be a synchronous request
	//note session is opened in ctor and closed in dtor
	writeLog("CSmfClientSymbian::sendRequest=");
	writeLog(aInterfaceName);
	//Gets data synchronously from the server
    TPtr8 symbianBuf(iSession.sendSyncRequest(aInterfaceName,requestType));
    //convert this into bytearray
    QByteArray receivedData(reinterpret_cast<const char*>(symbianBuf.Ptr()),symbianBuf.Length());
    return receivedData;
    //
	}

TInt CSmfClientSymbian::sendRequest(QByteArray& aSerializedData,
		 QString aInterfaceName,
		 SmfRequestTypeID requestType)
	{
	//TODO:-testing puspose only, should be removed in the release
	if(requestType == SmfTest)
		{
		QString log("Before iSesson.SendAsync");
		writeLog(log);
		iSession.sendAsyncRequest(aSerializedData,aInterfaceName,requestType,iStatus);
		SetActive();
		QString log2("After setactive");
		writeLog(log2);
		}
	else
		{
		//RSessionBase objects sendreceive is called
		iSession.sendAsyncRequest(aSerializedData,aInterfaceName,requestType,iStatus);
		SetActive();
		}
	}

TInt CSmfClientSymbian::sendDummyRequest(QByteArray* provider,QString aInterfaceName,
		 SmfRequestTypeID requestType)
	{
	switch(requestType)
		{
		case SmfTest:
			{

			}
			break;
		default:
			//should panic
			break;
		}
	}

CSmfClientSymbian::~CSmfClientSymbian()
	{
	writeLog("~CSmfClientSymbian");
    Cancel(); // Causes call to DoCancel()
    iSession.Close();
	}

RSmfClientSymbianSession::RSmfClientSymbianSession()
:iDataPtr(NULL, 0, 0),iDataPtr16(NULL,0),iIntfNamePtr(NULL,0),iIntfNamePtr8(NULL,0),iPtrProvider(NULL,0)
    {
    // No implementation required
    }

TInt RSmfClientSymbianSession::connectToServer()
    {
	writeLog("RSmfClientSymbianSession::connectToServer");
    TInt error = ::StartServerL();
    writeLog("StartServerL=");
    QString err = QString::number(error);
    writeLog(err);
    if ( KErrNone == error )
        {
		
        error = CreateSession( KSmfServerName,
                               Version(),
                               4 );
        QString crtSessionErr = QString::number(error);
        writeLog(crtSessionErr);
        }
    return error;
    }

//testing
void RSmfClientSymbianSession::writeLog(QString log) const
	{
#ifdef WRITE_LOG
	QFile file("c:\\data\\SmfClientLogs.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text))
	         ;
    QTextStream out(&file);
    out << log << "\n";
    file.close();
#endif
	}

TPtr8 RSmfClientSymbianSession::sendSyncRequest(QString aInterfaceName,
		SmfRequestTypeID aRequestType)
	{
	iLastRequest = aRequestType;
	/**
	 * The message body consists of.- 
	 * 1. Interface name as string ("org.symbian.smf.client.gallery")
	 * 2. Data pointer to be filled by serialized data(QList<smfProvider>)
	 */
	QString log("RSmfClientSymbianSession::sendSyncRequest-start-");
	writeLog(log);
	writeLog(QString("aInterfaceName=")+aInterfaceName);
	
	iInterfaceNamebyte.clear();
	//Convert the interface name into TPtr
	iInterfaceName.clear();
	iInterfaceName = aInterfaceName ;
	writeLog(QString("iInterfaceName=")+iInterfaceName);
	//lets pass serialized QString
	QDataStream intfNameStream(&iInterfaceNamebyte,QIODevice::WriteOnly);
	intfNameStream<<iInterfaceName;
	log.clear();
    log = QString("iInterfaceNamebyte size=");
    log += QString::number(iInterfaceNamebyte.size());
    writeLog(log);
	if(iIntfNameBuffer8)
		{
		delete iIntfNameBuffer8;
		iIntfNameBuffer8 =NULL;
		}
	TInt serializedintfsize = iInterfaceNamebyte.size();
	writeLog("iInterfaceNamebyte.size()=");
	writeLog(QString::number(serializedintfsize));
	iIntfNameBuffer8 = HBufC8::NewL(iInterfaceNamebyte.size()*2);
	iIntfNamePtr8.Set(iIntfNameBuffer8->Des());
	iIntfNamePtr8.Copy(reinterpret_cast<TUint8*>(iInterfaceNamebyte.data()),iInterfaceNamebyte.length());
	log.clear();
    log = QString("iIntfNamePtr8 size=")+QString::number(iIntfNamePtr8.Size());
    writeLog(log);

    writeLog("Before provider symbian copy");
	
	
	iInterfaceSymbian8.Copy(iIntfNamePtr8);	
	if(iBuffer)
		{
		delete iBuffer;
		iBuffer = NULL;
		}
	iMaxMessageSize = 1000 ;
    iBuffer = HBufC8::NewL(iMaxMessageSize);
    iDataPtr.Set(iBuffer->Des());
    log.clear();
    log = QString("After iDataPtr.Set");
    writeLog(log);

	
    TIpcArgs args;

    args.Set(0, &iInterfaceSymbian8);
    args.Set(1, &iDataPtr);
    
    TInt err(KErrBadHandle);
    writeLog("Before handle");
	log.clear();
    log = QString("iInterfaceSymbian8 size=")+QString::number(iInterfaceSymbian8.Size());
    writeLog(log);
    if (Handle()) 
    	{
        err = KErrNone;
        log.clear();
        log = QString("Before sendreceive");
        writeLog(log);
        //synchronous request
        TInt sendErr = SendReceive(aRequestType, args);
        if(sendErr)
        	{
			writeLog("SendReceive error=");
			QString errStr = QString::number(sendErr);
			writeLog(errStr);
        	}
        return iDataPtr;
        }
	}

/**
 * Calls SendReceive() after converting into symbian descriptors
 *
 */
void RSmfClientSymbianSession::sendAsyncRequest(QByteArray& aSerializedData,
		QString aInterfaceName,
		SmfRequestTypeID aRequestType,
		TRequestStatus& aStatus)
	{
	iLastRequest = aRequestType;
	/**
	 * The message body consists of.- 
	 * 1. Provider Info(SmfProvider*)+ Other common class data
	 *  (when applicable)-serialized
	 * 2. Interface name as string ("org.symbian.smf.client.gallery")
	 * 3. Data pointer to be filled by serialized data
	 */
	QString log("RSmfClientSymbianSession::sendAsyncRequest-start-");
	writeLog(log);
	
	iBaseProvider= aSerializedData;
	iInterfaceName = aInterfaceName ;
	
    int size = aSerializedData.size();
    log.clear();
    log = QString("aSerializedData size=")+ QString::number(size);
    
    writeLog(log);
    if(iProviderBuf)
    	{
		delete iProviderBuf;
		iProviderBuf = NULL;
    	}
    //TODO:- KSmfProviderMaxSize
    iProviderBuf = HBufC8::NewL(iBaseProvider.size()*2);
    iPtrProvider.Set(iProviderBuf->Des());
	//convert the QByteArray into TPtr
    iPtrProvider.Copy(reinterpret_cast<const TText8*>(iBaseProvider.constData()),iBaseProvider.length());
    
	
    log.clear();
    log = QString("iPtrProvider.Copy");
    writeLog(log);
    
	//Convert the interface name into TPtr////////////////////////
	iInterfaceName.clear();
	iInterfaceName = aInterfaceName ;
	writeLog(QString("iInterfaceName=")+iInterfaceName);
	//Pass serialized QString for interface name
	QDataStream intfNameStream(&iInterfaceNamebyte,QIODevice::WriteOnly);
	intfNameStream<<iInterfaceName;
	log.clear();
    log = QString("iInterfaceNamebyte size=");
    log += QString::number(iInterfaceNamebyte.size());
    writeLog(log);
	if(iIntfNameBuffer8)
		{
		delete iIntfNameBuffer8;
		iIntfNameBuffer8 =NULL;
		}
	iIntfNameBuffer8 = HBufC8::NewL(aInterfaceName.size()*2);
	iIntfNamePtr8.Set(iIntfNameBuffer8->Des());
	//Convert into symbian data type
	iIntfNamePtr8.Copy(reinterpret_cast<TUint8*>(iInterfaceNamebyte.data()),iInterfaceNamebyte.length());
	log.clear();
    log = QString("iIntfNamePtr8 size=")+QString::number(iIntfNamePtr8.Size());
    writeLog(log);
	

	if(iBuffer)
		{
		delete iBuffer;
		iBuffer = NULL;
		}
	//TODO:-KSmfMaxDataSize
	iMaxMessageSize = 2000 ;
    iBuffer = HBufC8::NewL(iMaxMessageSize);
    iDataPtr.Set(iBuffer->Des());
    log.clear();
    log = QString("After iDataPtr.Set");
    writeLog(log);
	
	
    TIpcArgs args;
    

    args.Set(0, &iPtrProvider);
    args.Set(1, &iIntfNamePtr8);
    args.Set(2, &iDataPtr);
    
    TInt err(KErrBadHandle);
    log.clear();
    log = QString("Before Handle()");
    writeLog("Before handle");
    if (Handle()) 
    	{
        err = KErrNone;
        log.clear();
        log = QString("Before sendreceive");
        writeLog(log);
       SendReceive(aRequestType, args, aStatus);

        }
	}

// -----------------------------------------------------------------------------
// StartServerL()
// Starts the server if it is not already running
// -----------------------------------------------------------------------------
//
static TInt StartServerL()
    {
    TInt result;

    TFindServer findSmfServer( KSmfServerName );
    TFullName name;

    result = findSmfServer.Next( name );
    if ( result == KErrNone )
        {
        // Server already running
        return KErrNone;
        }

    result = CreateServerProcessL();
    if ( result != KErrNone )
        {
        return  result;
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CreateServerProcessL()
// Creates a server process
// -----------------------------------------------------------------------------
//
static TInt CreateServerProcessL()
    {
    TInt result;
    //SmfServer SID
    TUid KSmfServerUID3 = { 0xE5027327 };
    const TUidType serverUid( KNullUid, KNullUid, KSmfServerUID3 );

    RProcess server;

    result = server.Create( KSmfServerFilename, KNullDesC, serverUid );
    User::LeaveIfError(result);
    if (KErrNone != result) {
        return  result; 
    }
    else {
		//User::WaitForRequest going for infinite loop, temporary work-around
        //TRequestStatus status;
        //server.Rendezvous(status);
        server.Resume(); // logon OK - start the server
        //Going for infinite loop
        //User::WaitForRequest(status);// wait for start or death
        User::After(700000);
        server.Close();
        return KErrNone;
        //return status.Int(); // return the error
    }
    return  KErrNone;
    }
