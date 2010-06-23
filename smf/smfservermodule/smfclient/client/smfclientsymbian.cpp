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
static TInt StartServer();
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
        	writeLog("RunL:-default");
        	//This contains error followed by actual data
        	QByteArray receivedData(reinterpret_cast<const char*>(iSession.iDataPtr.Ptr()),iSession.iDataPtr.Length());
        	writeLog("receivedData size=");
        	writeLog(QString::number(receivedData.size()));
        	SmfError errVal;
        	int errInt;
        	QByteArray data;
        	QDataStream reader(&receivedData,QIODevice::ReadOnly);
        	reader>>errInt;
        	writeLog("errInt=");
        	writeLog(QString::number(errInt));
        	errVal = (SmfError)errInt;
        	reader>>data;
        	writeLog("data size=");
        	writeLog(QString::number(data.size()));
        	SmfRequestTypeID opcode = (SmfRequestTypeID)iSession.getLastRequest();
        	if(iObserver)
        		{
				iObserver->resultsAvailable(data,opcode,errVal);
        		}
        	}
        	break;
        }
	}

QByteArray CSmfClientSymbian::sendRequest(QString aInterfaceName,
		 SmfRequestTypeID requestType,TInt maxSize)
	{
	//This will be a synchronous request
	//note session is opened in ctor and closed in dtor
	writeLog("CSmfClientSymbian::sendRequest=");
	writeLog(aInterfaceName);
	//Gets data synchronously from the server
    TPtr8 symbianBuf(iSession.sendSyncRequest(aInterfaceName,requestType,maxSize));
    //convert this into bytearray
    QByteArray receivedData(reinterpret_cast<const char*>(symbianBuf.Ptr()),symbianBuf.Length());
    return receivedData;
    //
	}
QByteArray CSmfClientSymbian::sendDSMSyncRequest(SmfRequestTypeID requestType,QByteArray& aSerializedData,SmfError& aErr,TInt maxSize)
	{
	writeLog("CSmfClientSymbian::sendDSMSyncRequest=");
	writeLog(QString::number(requestType));
	SmfError err;
	//Gets data synchronously from the server
    TPtr8 symbianBuf(iSession.sendDSMSyncRequest(requestType,aSerializedData,err,maxSize));
    //convert this into bytearray
    QByteArray receivedData(reinterpret_cast<const char*>(symbianBuf.Ptr()),symbianBuf.Length());
    writeLog("receivedData size=");
    writeLog(QString::number(receivedData.size()));
    return receivedData;
	}
TInt CSmfClientSymbian::sendRequest(QByteArray& aSerializedData,
		 QString aInterfaceName,
		 SmfRequestTypeID requestType,TInt aMaxAllocation,QByteArray xtraInfo)
	{
		//RSessionBase objects sendreceive is called
		iSession.sendAsyncRequest(aSerializedData,aInterfaceName,requestType,iStatus,aMaxAllocation,xtraInfo);
		SetActive();
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
:iDataPtr(NULL, 0, 0),iDataPtr16(NULL,0),
 iIntfNamePtr(NULL,0),iIntfNamePtr8(NULL,0),
 iPtrProvider(NULL,0),iPtrToSlot0(NULL,0)
    {
    // No implementation required
    }

TInt RSmfClientSymbianSession::connectToServer()
    {
	writeLog("RSmfClientSymbianSession::connectToServer");
    TInt error = ::StartServer();
    writeLog("StartServerL=");
    QString err = QString::number(error);
    writeLog(err);
    if ( KErrNone == error )
        {
		
        error = CreateSession(KSmfServerName,
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
/**
 * Sends sync request to the Smf server
 */
TPtr8 RSmfClientSymbianSession::sendDSMSyncRequest(SmfRequestTypeID aRequestType,
		QByteArray& aSerializedData,SmfError aErr,
		TInt maxSize)
	{
	/**
	 * Slot 0:- Data to be passed to DSM
	 * Slot 1:- Data returned from DSM
	 * Slot 2:- Error
	 */
	writeLog("RSmfClientSymbianSession::sendDSMSyncRequest");
	iLastRequest = aRequestType;
	if(iSlot0Buffer)
		{
		delete iSlot0Buffer;
		iSlot0Buffer = NULL;
		}
	iSlot0Buffer = HBufC8::NewL(aSerializedData.size());
	iPtrToSlot0.Set(iSlot0Buffer->Des());
	if(iBuffer)
		{
		delete iBuffer;
		iBuffer = NULL;
		}
    iBuffer = HBufC8::NewL(maxSize);
    iDataPtr.Set(iBuffer->Des());
    
    TIpcArgs args;

    args.Set(0, &iPtrToSlot0);
    args.Set(1, &iDataPtr);
    iDSMErr.Zero();
    args.Set(2,&iDSMErr);
    
    TInt sendErr = SendReceive(aRequestType,args);
    writeLog("SendReceive=");
    writeLog(QString::number(sendErr));
    TInt numIndex;
    TLex iLex(iDSMErr);
    
    iLex.Val(numIndex);
    aErr = (SmfError)numIndex;
    return iDataPtr;
	}
TPtr8 RSmfClientSymbianSession::sendSyncRequest(QString aInterfaceName,
		SmfRequestTypeID aRequestType,
		TInt maxSize)
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
	writeLog("Allocated for SmfProviderList=");
	writeLog(QString::number(maxSize));
	if(iBuffer)
		{
		delete iBuffer;
		iBuffer = NULL;
		}
    iBuffer = HBufC8::NewL(maxSize);
    iDataPtr.Set(iBuffer->Des());
    log.clear();
    log = QString("After iDataPtr.Set");
    writeLog(log);

	
    TIpcArgs args;

    args.Set(0, &iInterfaceSymbian8);
    args.Set(1, &iDataPtr);
    if(maxSize)
    	{
		iMaxSize = maxSize;
		args.Set(2,iMaxSize);
    	}
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
		TRequestStatus& aStatus,
		TInt aMaxAllocation,
		QByteArray aXtraInfo)
	{
	/**
	 * The message body consists of.- 
	 * Modified,-
	 * slot 0 = SmfProvider +PageInfo flag+ aPageNum + aPerPage + XtraInfo flag(size of xtra data) Serialized
	 * slot 1 = Interface name serialized
	 * slot 2 = Data pointer to filled by server
	 * slot 3= Xtra info when required by server else empty buffer
	 */
	writeLog("RSmfClientSymbianSession::sendAsyncRequest-start-");
	iLastRequest = aRequestType;
	
	
	
	
	if(iProviderBuf)
		{
		delete iProviderBuf;
		iProviderBuf = NULL;
		}
	iProviderBuf = HBufC8::NewL(aSerializedData.size());
	iPtrProvider.Set(iProviderBuf->Des());
	iPtrProvider.Copy(reinterpret_cast<const TText8*>(aSerializedData.constData()),aSerializedData.length());
	
	
	
	
	//convert the QByteArray into TPtr
    TPtrC8 ptrSlot0(reinterpret_cast<const TText8*>(aSerializedData.constData()),aSerializedData.length());
    writeLog("ptrSlot0 size=");
    writeLog(QString::number(ptrSlot0.Size()));
	//Convert the interface name into TPtr
	iInterfaceName.clear();
	iInterfaceName.append(aInterfaceName) ;
	writeLog(QString("iInterfaceName=")+iInterfaceName);
	iInterfaceNamebyte.clear();
	//Pass serialized QString for interface name
	QDataStream intfNameStream(&iInterfaceNamebyte,QIODevice::WriteOnly);
	intfNameStream<<iInterfaceName;
	writeLog("iInterfaceNamebyte size=");
	writeLog(QString::number(iInterfaceNamebyte.size()));
	if(iIntfNameBuffer8)
		{
		delete iIntfNameBuffer8;
		iIntfNameBuffer8 = NULL;
		}
	iIntfNameBuffer8 = HBufC8::NewL(iInterfaceNamebyte.size());
	iIntfNamePtr8.Set(iIntfNameBuffer8->Des());
	iIntfNamePtr8.Copy(reinterpret_cast<const TText8*>(iInterfaceNamebyte.constData()),iInterfaceNamebyte.length());
	writeLog("After iIntfNamePtr8.Copy");
	if(iBuffer)
		{
		delete iBuffer;
		iBuffer = NULL;
		}
    iBuffer = HBufC8::NewL(aMaxAllocation);
    iDataPtr.Set(iBuffer->Des());
    writeLog("After iDataPtr.Set");
    TIpcArgs args;
    
    //filling the slots
    args.Set(0, &iPtrProvider);
    args.Set(1, &iIntfNamePtr8);
    args.Set(2, &iDataPtr);
    writeLog("After setting 0,1,2 slots");
    if(aXtraInfo.size())
    	{
		TPtrC8 ptrToXtraInfo(reinterpret_cast<const TText8*>(aXtraInfo.constData()),aXtraInfo.length());
		writeLog("ptrToXtraInfo size=");
		writeLog(QString::number(ptrToXtraInfo.Size()));
		args.Set(3, &ptrToXtraInfo);
    	}
    TInt err(KErrBadHandle);
    writeLog("Before Handle()");
    if (Handle()) 
    	{
        err = KErrNone;
        writeLog("Before sendreceive");
        SendReceive(aRequestType, args, aStatus);
        }
	}

// -----------------------------------------------------------------------------
// CreateServerProcessL()
// Creates a server process
// -----------------------------------------------------------------------------
//
static TInt CreateServerProcessL()
    {
    TInt result;
    TUid KSmfServerUID3 = { 0xE5027327 };
    const TUidType serverUid( KNullUid, KNullUid, KSmfServerUID3 );

    RProcess server;

    result = server.Create( KSmfServerFilename, KNullDesC, serverUid );

    if ( result != KErrNone )
        {
        return  result;
        }

    server.Resume();
    server.Close();

    return  KErrNone;
    }
static TInt StartServer()
    {
    TInt result;

    TFindServer findSmfServer( KSmfServerFilename );
    TFullName name;

    result = findSmfServer.Next( name );
    if ( result == KErrNone )
        {
        // Server already running
        return KErrNone;
        }

    RSemaphore semaphore;
    result = semaphore.CreateGlobal( KSmfServerSemaphoreName, 0 );
    if ( result != KErrNone )
        {
        return  result;
        }

    result = CreateServerProcessL();
    if ( result != KErrNone )
        {
        return  result;
        }

    semaphore.Wait();
    semaphore.Close();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CreateServerProcess()
// Creates a server process
// -----------------------------------------------------------------------------
//
static TInt CreateServerProcess()
    {
    TInt result;
    //SmfServer SID
    TUid KSmfServerUID3 = { 0xE5027327 };
    const TUidType serverUid( KNullUid, KNullUid, KSmfServerUID3 );

    RProcess server;

    result = server.Create( KSmfServerName, KNullDesC, serverUid );

    if ( result != KErrNone )
        {
        return  result;
        }

    server.Resume();
    server.Close();

    return  KErrNone;
    }
