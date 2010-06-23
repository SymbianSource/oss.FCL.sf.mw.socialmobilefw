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
#include <smfrelationmgr.h>
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
	Q_UNUSED(aActiveObjectPriority)
	}
TInt SmfServerSymbian::addToSessionMap(SmfServerSymbianSession* aSession,const RMessage2& aMsg)
	{
	qDebug()<<("SmfServerSymbian::addToSessionMap");
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
	Q_UNUSED(aSession)
Q_UNUSED(aMsg)
//To be implemented
	return 0;
	}

//TODO - change this to a macro using qdebug  to avoid file opening and closing
/*void SmfServerSymbian::writeLog(QString log) const
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
	}*/
SmfServerSymbian::~SmfServerSymbian()
	{

	}
SmfServer* SmfServerSymbian::wrapper()
	{
	return iWrapper;
	}
CSession2* SmfServerSymbian::NewSessionL(const TVersion& aVersion, const RMessage2& /*aMessage*/) const
	{
	Q_UNUSED(aVersion)
	// Check that the version is OK
	//    TVersion v( 0, 1, 0 );
	//    if (!User::QueryVersionSupported( v, aVersion ))
	//        User::Leave( KErrNotSupported );  
	// Create the session.
	QString log("New session created");
	qDebug()<<(log);
	return new (ELeave) SmfServerSymbianSession( const_cast<SmfServerSymbian*>(this) );
	}
SmfServerSymbianSession* SmfServerSymbian::findSession(TInt id)
	{
	qDebug()<<("SmfServerSymbian::findSession");
	CSessionStruct* sessionStruct = iMap.Find(id);	
	if(sessionStruct)
		{
		qDebug()<<("Session id found");
		return sessionStruct->iSession;
		}
	else
		{
		qDebug()<<("Session id not found");
		return NULL;
		}
	}
TInt SmfServerSymbian::findAndServiceclient(TInt requestID,QByteArray* parsedData,SmfError error)
	{
	qDebug()<<("SmfServerSymbian::findAndServiceclient");
	SmfServerSymbianSession* sessionToservice = findSession(requestID);
	if(sessionToservice)
		{
		sessionToservice->resultsAvailable(parsedData,error);
		}
	return 0;
	}
SmfServerSymbianSession::SmfServerSymbianSession(SmfServerSymbian* aServer):
			iServer(aServer),iPtrToBuf(NULL,0) ,
			iIntfNmaeSymbian(NULL,0), iProviderSymbian(NULL,0),
			iIntfNameSymbian16(NULL,0) ,iXtraDataPtr(NULL,0),
			iPtrToDataForClient(NULL,0) ,iPtrDataForDSM(NULL,0),iPtrDataFromDSM(NULL,0)  
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
	qDebug()<<("SmfServerSymbianSession::resultsAvailable");
	//We should remove the request from the map as soon its no longer outstanding
	iServer->removeFromSessionMap(this,iMessage);
	
	//Note:- Session must take the ownership of the data being passed to the client session
	if(iDataForClient)
		{
		delete iDataForClient;
		iDataForClient = NULL;
		}
	iDataForClient = HBufC8::NewL(parsedData->size());
	iPtrToDataForClient.Set(iDataForClient->Des());
	iPtrToDataForClient.Copy(reinterpret_cast<const TText8*>(parsedData->constData()),parsedData->length());

	TInt writeErr = iMessage.Write(2,iPtrToDataForClient);
	qDebug()<<("Write=");
	QString wrErr = QString::number(writeErr);
	qDebug()<<(wrErr);
	//signal completion for the last request
	iMessage.Complete(error);
	}
void SmfServerSymbianSession::ServiceL(const RMessage2& aMessage)
	{    

	qDebug()<<("SmfServerSymbianSession::ServiceL=");
	iMessage = aMessage ;
	QString log;
	log = QString::number(iMessage.Function());
	qDebug()<<(log);
	//construct the client auth id
	SmfClientAuthID clientAuthID;
	clientAuthID.pid = aMessage.SecureId();
	clientAuthID.session = this;
	//TODO:- No client pid checking?No capability? So why symbian client-server?
	HandleClientMessageL(iMessage);

	}
void SmfServerSymbianSession::HandleClientMessageL(const RMessage2& aMessage)
	{
	qDebug()<<("HandleClientMessageL");
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
	else if(iLastRequest == SmfRelationCreate ||
			iLastRequest == SmfRelationAssociate || 
			iLastRequest == SmfRelationSearchById ||
			iLastRequest == SmfRelationSearchByContact ||
			iLastRequest == SmfRelationCount ||
			iLastRequest == SmfRelationGet ||
			iLastRequest == SmfRelationGetAll ||
			iLastRequest == SmfRelationGetAllRelations
			)
		{
		HandleDSMServiceL(aMessage);
		}
	else
		{
		HandleCommonServiceL(aMessage);
		}
	}
void SmfServerSymbianSession::HandleDSMServiceL(const RMessage2 & aMessage)
	{
	//TODO:-If DSM takes care of deserialization and formation of User and social 
	//profile from the params then switch case can be removed
	if(iDataForDSM)
		{
		delete iDataForDSM;
		iDataForDSM = NULL;
		}
	
	switch(iLastRequest)
		{
		case SmfRelationCreate:
			{
			iDataForDSM = HBufC8::New(maxSmfRelationIdSize);
			iPtrDataForDSM.Set(iDataForDSM->Des());
			TInt readerr0 = aMessage.Read(0,iPtrDataForDSM); 

			}
			break;
		case SmfRelationAssociate:
			{
			int maxAlloc = 100;
			iDataForDSM = HBufC8::New(maxAlloc);
			iPtrDataForDSM.Set(iDataForDSM->Des());
			TInt readerr0 = aMessage.Read(0,iPtrDataForDSM); 
			}
			break;
		case SmfRelationSearchById:
			{
			int maxAlloc = MaxSmfContactSize;
			iDataForDSM = HBufC8::New(maxAlloc);
			iPtrDataForDSM.Set(iDataForDSM->Des());
			TInt readerr0 = aMessage.Read(0,iPtrDataForDSM); 
			}
			break;
		case SmfRelationCount:
			{
			int maxAlloc = 100;
			iDataForDSM = HBufC8::New(maxAlloc);
			iPtrDataForDSM.Set(iDataForDSM->Des());
			TInt readerr0 = aMessage.Read(0,iPtrDataForDSM);
			}
			break;
		case SmfRelationGet:
			{
			int maxAlloc = maxSmfRelationItemSize*maxRelationItems;
			iDataForDSM = HBufC8::New(maxAlloc);
			iPtrDataForDSM.Set(iDataForDSM->Des());
			TInt readerr0 = aMessage.Read(0,iPtrDataForDSM); 
			}
			break;
		case SmfRelationGetAll:
			{
			int maxAlloc = maxSmfRelationItemSize*maxRelationItems;
			iDataForDSM = HBufC8::New(maxAlloc);
			iPtrDataForDSM.Set(iDataForDSM->Des());
			TInt readerr0 = aMessage.Read(0,iPtrDataForDSM); 
			}
			break;
		default:
			break;
		}
	//Convert into QByteArray
	QByteArray qtdataForDSM(reinterpret_cast<const char*>(iPtrDataForDSM.Ptr()),iPtrDataForDSM.Length()) ;
	QByteArray qtdataFromDSM;
	SmfRequestTypeID opcode = (SmfRequestTypeID)iLastRequest;
	SmfError dsmErr = iServer->wrapper()->sendToDSM(qtdataForDSM,opcode,qtdataFromDSM);
	if(dsmErr == SmfNoError)
		{
		if(qtdataFromDSM.size())
			{
			if(iDataFromDSM)
				{
				delete iDataFromDSM;
				iDataFromDSM = NULL;
				}
			iDataFromDSM = HBufC8::NewL(qtdataFromDSM.size());
			iPtrDataFromDSM.Set(iDataFromDSM->Des());
			iPtrDataFromDSM.Copy(reinterpret_cast<const TText8*>(qtdataFromDSM.constData()),qtdataFromDSM.length());
			TInt writeErr = aMessage.Write(1,iPtrDataFromDSM);
			}
		}
	else
		{
		iDSMErr.Zero();
		TInt errInt = dsmErr;
		iDSMErr.AppendNum(errInt);
		TInt writeErr = aMessage.Write(2,iDSMErr);
		}
	}
void SmfServerSymbianSession::HandleGetService(const RMessage2 & aMessage)
	{
	qDebug()<<("SmfServerSymbianSession::HandleGetService");
	
	/**
	 * Note:- client sends message for this opcode in the following format,-
	 * Slot 0:- Interface Name buffer
	 * Slot 1:- Ptr to data block to be filled with SmfProvideList* serialized into QByteArray
	 * Slot 2:- Max data size allocated in the client side
	 * TODO:- What if size of SmfProviderList to be passed to client is greater than
	 * this size???
	 */
	
	iInterfaceNametbuf.Zero();
	TInt readerr = aMessage.Read(0,iInterfaceNametbuf);
	qDebug()<<(QString::number(readerr));
	QByteArray bytearray(reinterpret_cast<const char*>(iInterfaceNametbuf.Ptr()),iInterfaceNametbuf.Length()) ;
	QDataStream stream3(&bytearray,QIODevice::ReadOnly);
	stream3>>iInterfaceID;
	qDebug()<<("iInterfaceID=");
	qDebug()<<(iInterfaceID);
	//Interface names are diff in client and plugin, replacing *.client.* with *.plugin.*
	iInterfaceID.replace(QString(".client"),QString(".plugin"));
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
	//now tempMapcontains the info to be passed to the client
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
	//now serialize this list into bytearray
	byteArrayToClnt.clear();
	QDataStream stream(&byteArrayToClnt,QIODevice::WriteOnly);
	stream<<providerList;
	qDebug()<<("providerList.count()=");
	qDebug()<<(QString::number(providerList.count()));
	qDebug()<<("Before providerListSymbian");
	//now convert it into TPtr8
	TPtrC8 providerListSymbian(reinterpret_cast<const TText8*>(byteArrayToClnt.constData()),byteArrayToClnt.length());
	qDebug()<<("After providerListSymbian");
	TInt writeErr = aMessage.Write(1,providerListSymbian);
	qDebug()<<("Write=");
	QString wrErr = QString::number(writeErr);
	qDebug()<<(wrErr);
	//signal completion
	TInt completion = SmfGetServiceComplete ;
	aMessage.Complete(completion);
	}
void SmfServerSymbianSession::HandleCommonServiceL(const RMessage2& aMessage)
	{
	/**
	 * Note:- client sends message in the following format,-
	 * Slot 0:- SmfProvider* serialized+Page info flag+page number+per page (if page info flag)+xtra info flag
	 * Slot 1:- Interface Name buffer
	 * Slot 2:- Ptr to data block to be filled
	 * Slot 3 :- Xtra Data if xtra data flag
	 */
	//TODO:- Use macro instead, shared betn client-server
	TInt maxProvidersize = 1000;
	TInt maxOtherInfoSize = 300;
	TInt providerSize = aMessage.GetDesLength(0);
	if(iProviderBuf)
		{
		delete iProviderBuf;
		iProviderBuf = NULL;
		}
	iProviderBuf = HBufC8::NewL(maxProvidersize + maxOtherInfoSize);
	iProviderSymbian.Set(iProviderBuf->Des());

	TInt intfNameSize = aMessage.GetDesLength(1);
	if(iIntfNameBuf)
		{
		delete iIntfNameBuf;
		iIntfNameBuf = NULL;
		}
	iIntfNameBuf = HBufC8::NewL(1000);//for safeside
	iIntfNmaeSymbian.Set(iIntfNameBuf->Des());


	//read it into iProviderSymbian
	aMessage.ReadL(0,iProviderSymbian);
	//read it into iIntfNmaeSymbian
	aMessage.ReadL(1,iIntfNmaeSymbian);
	
	//convert SmfProvider info from Symbian into bytearray
	QByteArray providerBufQt(reinterpret_cast<const char*>(iProviderSymbian.Ptr()),iProviderSymbian.Length());
	qDebug()<<("providerBufQt.size=");
	qDebug()<<(QString::number(providerBufQt.size()));

	//now de-serialize it
	QDataStream stream(&providerBufQt,QIODevice::ReadOnly);
	SmfProvider provider;
	stream>>provider ;
	TInt pageinfoFlag=0;
	stream>>pageinfoFlag;
	TInt pageNo=-1;
	TInt perpage=-1;
	if(pageinfoFlag)
		{
		stream>>pageNo;
		stream>>perpage;
		qDebug()<<("pageNo=");
		qDebug()<<(QString::number(pageNo));
		qDebug()<<("perpage=");
		qDebug()<<(QString::number(perpage));
		}
	TInt XtraInfoFlag = 0;
	TBool isXtraData = EFalse;
	stream>>XtraInfoFlag;
	if(XtraInfoFlag)
		{
		isXtraData = ETrue;
		qDebug()<<("XtraInfoFlag=");
		qDebug()<<(QString::number(XtraInfoFlag));
		}
	qDebug()<<("iIntfNmaeSymbian.Size=");
	qDebug()<<(QString::number(iIntfNmaeSymbian.Size()));
	QByteArray bytearray(reinterpret_cast<const char*>(iIntfNmaeSymbian.Ptr()),iIntfNmaeSymbian.Length()) ;
	QDataStream intfNameStream(&bytearray,QIODevice::ReadOnly);
	iInterfaceID.clear();
	intfNameStream>>iInterfaceID;
	qDebug()<<("After de-serializing into iInterfaceID=");
	qDebug()<<(iInterfaceID);
	//Interface names are diff in client and plugin, replacing *.client.* with *.plugin.*
	iInterfaceID.replace(QString(".client"),QString(".plugin"));
	//Get the plugin ID who matches provider info for a given intf name
	SmfPluginID pluginID = iServer->wrapper()->getPlugin(iInterfaceID,provider);
	qDebug()<<("pluginID from PM=");
	qDebug()<<(pluginID);
	iPluginIDList.clear();
	//we need to check only this pluginID is authorized?
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
		if(isXtraData)
			{
			if(iXtraDataBuf)
				{
				delete iXtraDataBuf;
				iXtraDataBuf = NULL;	
				}
			iXtraDataBuf = HBufC8::NewL(XtraInfoFlag);
			iXtraDataPtr.Set(iXtraDataBuf->Des());
			QByteArray XtraBufQt(reinterpret_cast<const char*>(iXtraDataPtr.Ptr()),iXtraDataPtr.Length());
			qDebug()<<("XtraBufQt size=");
			qDebug()<<(QString::number(XtraBufQt.size()));
			iServer->wrapper()->sendToPluginManager(id,pluginID,iInterfaceID,opcode,XtraBufQt);
			}
		else
			{
			iServer->wrapper()->sendToPluginManager(id,pluginID,iInterfaceID,opcode);
			}
		}
	else
		{
		SmfError err = SmfNoAuthorizedPlugin;
		iErrBuf.Zero();
		iErrBuf.AppendNum(err);
		iMessage.Write(2,iErrBuf);
		}


	}
