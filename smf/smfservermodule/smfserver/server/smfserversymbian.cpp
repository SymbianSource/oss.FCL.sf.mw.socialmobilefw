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
 * 
 * 
 * Description: Private implementation for Symbian OS
 *
 */
#ifdef WRITE_LOG
#include <QFile>
#include <QTextStream>
#endif
#include <QDataStream>
#include <QStringList>
#include <QString>

#include "smfserversymbian_p.h"


SmfServerSymbian* SmfServerSymbian::NewL(CActive::TPriority aActiveObjectPriority,SmfServer* aWrapper)
	{
	SmfServerSymbian* self = new(ELeave) SmfServerSymbian(aActiveObjectPriority,aWrapper);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(); // self
	return self;
	}

void SmfServerSymbian::ConstructL()
	{
	}


SmfServerSymbian::SmfServerSymbian( CActive::TPriority aActiveObjectPriority,SmfServer* aWrapper )
: CPolicyServer(0,myPolicy, ESharableSessions), iWrapper(aWrapper)
	{
	Q_UNUSED(aActiveObjectPriority);
	}



TInt SmfServerSymbian::addToSessionMap(SmfServerSymbianSession* aSession,const RMessage2& aMsg)
	{
	writeLog("SmfServerSymbian::addToSessionMap");
	if(iSessionCount)
		{
		//The key generation 
		TInt key = qrand();
		//in case qrand generates any duplicate key
		while(iMap.Find(key))
			{
			key = qrand();
			}

		CSessionStruct session;
		session.iMsg = aMsg;
		session.iSession = aSession;
		iMap.Insert(key,session); 
		
		return key;
		}
	
	return (-1);
	}


TInt SmfServerSymbian::removeFromSessionMap(SmfServerSymbianSession* aSession,RMessage2& aMsg)
	{
	Q_UNUSED(aSession);
	Q_UNUSED(aMsg);
	return 0;
	}

//TODO - change this to a macro using qdebug  to avoid file opening and closing
void SmfServerSymbian::writeLog(QString log) const
	{
#ifdef WRITE_LOG
	QFile file("c:\\data\\SmfServerLogs.txt");
	if (!file.open(QIODevice::Append | QIODevice::Text))
		return;
	QTextStream out(&file);
	out << log << "\n";
	file.close();
#else
	Q_UNUSED(log)
#endif
	}


SmfServerSymbian::~SmfServerSymbian()
	{

	}

SmfServer* SmfServerSymbian::wrapper()
	{
	return iWrapper;
	}


CSession2* SmfServerSymbian::NewSessionL(const TVersion& aVersion, const RMessage2& /*aMessage*/) const
	{

	// Check that the version is OK
	//    TVersion v( 0, 1, 0 );
	//    if (!User::QueryVersionSupported( v, aVersion ))
	//        User::Leave( KErrNotSupported );  
	// Create the session.
	Q_UNUSED(aVersion);
	QString log("New session created");
	writeLog(log);
	return new (ELeave) SmfServerSymbianSession( const_cast<SmfServerSymbian*>(this) );
	}

SmfServerSymbianSession* SmfServerSymbian::findSession(TInt id)
	{
	writeLog("SmfServerSymbian::findSession");
	CSessionStruct* sessionStruct = iMap.Find(id);	
	if(sessionStruct)
		{
		writeLog("Session id found");
		return sessionStruct->iSession;
		}
	else
		{
		writeLog("Session id not found");
		return NULL;
		}
	}

TInt SmfServerSymbian::findAndServiceclient(TInt requestID,QByteArray* parsedData,SmfError error)
	{
	writeLog("SmfServerSymbian::findAndServiceclient");
	SmfServerSymbianSession* sessionToservice = findSession(requestID);
	if(sessionToservice)
		{
		sessionToservice->resultsAvailable(parsedData,error);
		}
	return 0;
	}

SmfServerSymbianSession::SmfServerSymbianSession(SmfServerSymbian* aServer):
			iServer(aServer),iPtrToBuf(NULL,0) ,iIntfNmaeSymbian(NULL,0), iProviderSymbian(NULL,0),iIntfNameSymbian16(NULL,0)     
	{
	iServer->iSessionCount++;
	}

SmfServerSymbianSession::~SmfServerSymbianSession()
	{
	//cleanup of client resources
	iServer->iSessionCount--;
	}


void SmfServerSymbianSession::clientathorizationFinished(bool success)
	{
	//Client authorization failed
	if(!success)
		{
		//TODO:- Should use smf wide error instead
		iMessage.Complete(KErrPermissionDenied);
		}
	else
		{
		HandleClientMessageL(iMessage);
		}
	}


void SmfServerSymbianSession::resultsAvailable(QByteArray* parsedData,SmfError error)
	{

	//Note:- The order of serialization of parsedData - Error value followed by the data
	//parsedData is already serialized by PM
	iServer->writeLog("SmfServerSymbianSession::resultsAvailable");
	//We should remove the request from the map as soon its no longer outstanding
	iServer->removeFromSessionMap(this,iMessage);
	
	byteArrayToClnt.clear();
	byteArrayToClnt.append(*(parsedData));
	iPtrToBuf.Copy(reinterpret_cast<TUint8*>(byteArrayToClnt.data()),byteArrayToClnt.length());

	TInt writeErr = iMessage.Write(2,iPtrToBuf);
	
	iServer->writeLog("Write=");
	QString wrErr = QString::number(writeErr);
	iServer->writeLog(wrErr);
	
	//signal completion for the last request
	iMessage.Complete(error);
	}


void SmfServerSymbianSession::ServiceL(const RMessage2& aMessage)
	{    

	iServer->writeLog("SmfServerSymbianSession::ServiceL=");
	iMessage = aMessage ;
	
	QString log;
	log = QString::number(iMessage.Function());
	iServer->writeLog(log);
	
	SmfClientAuthID clientAuthID;
	clientAuthID.pid = aMessage.SecureId();
	clientAuthID.session = this;
	//check whether the client is authorized
	if(!iServer->wrapper()->isClientAuthorized(clientAuthID))
		{
		//Client is yet to be authorized
		//It should start authorization or simply return err value is yet TBD, so the following code may change
		//start authorization of client, session will be blocked untill its authorized
		//HandleclientMessageL will be called in that fn
		iServer->wrapper()->authorizeClient(clientAuthID);
		}
	else
		{
		//client is authorized, so proceed
		HandleClientMessageL(iMessage);		
		}
	}

void SmfServerSymbianSession::HandleClientMessageL(const RMessage2& aMessage)
	{
	iServer->writeLog("HandleClientMessageL");
	iLastRequest = aMessage.Function();
	
	
	/**Note:- Only ESmfGetService needs to be taken care separately as it doesn't involve createrequest for PM
	 *See SmfRequestTypeID for list of opcodes
	 *
	 *Two cases,-
	 *1.for ESmfGetServices we donno the provider info
	 *2. for rest of the cases, we know the provider info
	 *so it seems PM needs to provide two overloaded getPlugins API But for every client intf instanciation
	 *we would execute the same things twice.
	 *TODO:- to be changed after GetServices returns SmfProvider+pluginID 
	 * 
	 */
	if(iLastRequest == SmfGetService)
		{
		HandleGetService(aMessage);
		}
	else
		{
		HandleCommonServiceL(aMessage);
		}
	}

void SmfServerSymbianSession::HandleGetService(const RMessage2 & aMessage)
	{
	iServer->writeLog("SmfServerSymbianSession::HandleGetService");
	/**
	 * Note:- client sends message for this opcode in the following format,-
	 * Slot 0:- Interface Name buffer
	 * Slot 1:- Ptr to data block to be filled with SmfProvideList* serialized into QByteArray
	 */
	iInterfaceNametbuf.Zero();

	TInt readerr = aMessage.Read(0,iInterfaceNametbuf);
	
	iServer->writeLog(QString::number(readerr));
	
	QByteArray bytearray(reinterpret_cast<const char*>(iInterfaceNametbuf.Ptr()),iInterfaceNametbuf.Length()) ;
	QDataStream stream3(&bytearray,QIODevice::ReadOnly);
	stream3>>iInterfaceID;
	
	iServer->writeLog("iInterfaceID=");
	iServer->writeLog(iInterfaceID);
	
	iServer->wrapper()->getPlugins(iInterfaceID,iPluginIDMap);
	//form the plugin id list from the map
	QMapIterator<SmfPluginID, SmfProvider> iter(iPluginIDMap);
	iPluginIDList.clear();
	while (iter.hasNext()) 
		{
	iter.next();
	iPluginIDList<<iter.key();
		}
	
	//iAuthList will be filled by credential manager
	iServer->wrapper()->getAuthorizedPlugins(iPluginIDList,iAuthList);

	//iPluginIDMap now contains SmfProvider info to return to the client
	//No need to add this to session map, as we are not requesting PM for this
	QMap<SmfPluginID, SmfProvider> tempMap;
	QMapIterator<SmfPluginID, SmfProvider> i(iPluginIDMap);
	
	while (i.hasNext()) 
		{
		i.next();
		if(iAuthList.contains(i.key()))
			{
			tempMap.insert(i.key(),i.value());
			}
		}

	iPluginIDMap.clear();
	iPluginIDMap = tempMap;
	
	//form list of smfprovider from this map,-
	QList<SmfProvider> providerList;
	i = iPluginIDMap;
	while(i.hasNext())
		{
		i.next();
		providerList.append(i.value());
		}

	byteArrayToClnt.clear();
	QDataStream stream(&byteArrayToClnt,QIODevice::WriteOnly);
	stream<<providerList;

	iPtrToBuf.Copy(reinterpret_cast<TUint8*>(byteArrayToClnt.data()),byteArrayToClnt.length());
	TInt writeErr = aMessage.Write(1,iPtrToBuf);
	
	iServer->writeLog("Write=");
	QString wrErr = QString::number(writeErr);
	iServer->writeLog(wrErr);
	//signal completion
	
	TInt completion = SmfGetServiceComplete ;
	aMessage.Complete(completion);
	}

void SmfServerSymbianSession::HandleCommonServiceL(const RMessage2& aMessage)
	{
	/**
	 * Note:- client sends message in the following format,-
	 * Slot 0:- SmfProvider* serialized
	 * Slot 1:- Interface Name buffer
	 * Slot 2:- Ptr to data block to be filled
	 */
	TInt providerSize = aMessage.GetDesLength(0);
	if(iProviderBuf)
		{
	delete iProviderBuf;
	iProviderBuf = NULL;
		}
	iProviderBuf = HBufC8::NewL(providerSize);
	iProviderSymbian.Set(iProviderBuf->Des());

	TInt intfNameSize = aMessage.GetDesLength(1);
	if(iIntfNameBuf)
		{
	delete iIntfNameBuf;
	iIntfNameBuf = NULL;
		}
	iIntfNameBuf = HBufC8::NewL(intfNameSize*2);//for safeside
	iIntfNmaeSymbian.Set(iIntfNameBuf->Des());


	//read it into iProviderSymbian
	aMessage.ReadL(0,iProviderSymbian);
	//read it into iIntfNmaeSymbian
	aMessage.ReadL(1,iIntfNmaeSymbian);

	//convert SmfProvider info from Symbian into bytearray
	QByteArray providerBufQt(reinterpret_cast<const char*>(iProviderSymbian.Ptr()),iProviderSymbian.Length());
	iServer->writeLog("After providerBufQt");

	//now de-serialize it
	QDataStream stream(&providerBufQt,QIODevice::ReadOnly);
	SmfProvider provider;
	stream>>provider ;
	QByteArray bytearray(reinterpret_cast<const char*>(iIntfNmaeSymbian.Ptr()),iIntfNmaeSymbian.Length()) ;
	QDataStream intfNameStream(&bytearray,QIODevice::ReadOnly);
	iInterfaceID.clear();
	intfNameStream>>iInterfaceID;

	iServer->writeLog("After de-serializing into iInterfaceID=");
	iServer->writeLog(iInterfaceID);
	
	//Get the plugin ID who matches provider info for a given intf name
	SmfPluginID pluginID = iServer->wrapper()->getPlugin(iInterfaceID,provider);
	
	iServer->writeLog("pluginID from PM=");
	iServer->writeLog(pluginID);
	
	iPluginIDList.clear();
	//we need to check only this pluginID is authorized
	iPluginIDList<<pluginID;
	
	//iAuthList will contain pluginID for a successfull case
	iAuthList.clear();
	iServer->wrapper()->getAuthorizedPlugins(iPluginIDList,iAuthList);
	if(iAuthList.contains(pluginID))
		{
		//Plugin ID is authorised, service the request
		//Gnerate request id only if the plugin ID is authorised
		TInt id = iServer->addToSessionMap(this,aMessage);
		//request PM to get the data
		SmfRequestTypeID opcode = (SmfRequestTypeID)iLastRequest;
		iServer->wrapper()->getRequestedData(id,pluginID,iInterfaceID,opcode,providerBufQt);
		}
	else
		{
		//TODO:-Plugin ID is not authorized, throw some error
		}
	}
