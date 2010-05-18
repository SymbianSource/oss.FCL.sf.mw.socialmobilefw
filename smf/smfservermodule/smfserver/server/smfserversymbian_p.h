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
 * Manasij Roy, Nalina Hariharan*
 * Description:
 * SMF Server private implementation for Symbian
 *
 **/

#ifndef SMFSERVERSYMBIAN_H
#define SMFSERVERSYMBIAN_H
#include <QObject>
#include <e32hashtab.h>
#include <e32base.h>
#include <QByteArray>
#include <QString>
//testing -start
#include "smfprovider.h"
//end
//SMF wide global const and macros, to be shared among SMF components as well as SMF aware apps
//#include <smf/smfGlobal.h>

//#include "clientGlobal.h"

#include "SmfServer.h"
//Forward declarations
class SmfServerSymbianSession;

/**
 * Stores session and request information.
 * For now storing RMessage2 is reduntant as we are allowing only one outstanding request per session
 */
struct CSessionStruct
	{
	SmfServerSymbianSession* iSession;
	RMessage2 iMsg;
	};



//Policy table---start
//note capabilities are yet to be finalized
//46 services as of 15th apr
const TUint myRangeCount = 1/*7*/; 

//Passing everything for the time being
const TInt myRanges[myRangeCount] = 
        {
        0/*, //ERequestPass
        1, //ERequestFail
        2, //ERequestNotSupported
        3, //ERequestDrmService
        4, //ERequestUserEnvironmentService  
        5, //ERequestCustomCheckPass
        6  //ERequestCustomCheckFail*/ 
        };
        
        
/* bis: each element of this array actually indexes to a particular element of
TPolicyElement. For eg. element 0 of this array maps element 0 of myRanges to 
a particular element of TpolicyElement array which is in this case element 0
of TPolicyElement       
*/
const TUint8 myElementsIndex[myRangeCount] = 
        {
        CPolicyServer::EAlwaysPass, //passing everything for now  
        };


/* bis: Policies used by the server to check messages from the client 
for each message, the server will carry out security check according 
to this policy. 
*/        
const CPolicyServer::TPolicyElement myElements[] = 
        {
        {_INIT_SECURITY_POLICY_PASS, CPolicyServer::EFailClient}     
        };

/* bis: TPolicy used by the server to check messages from the client 
for each message. This structure is passed to the server at its creation.
The Symbian client-server architecture will make sure that each client request
passes through the security test as indicated by myPolicy before it is handled
by myPolciyServer */        
const CPolicyServer::TPolicy myPolicy =
        {
        //bis:
        //the connection policy (CPolicyServer::EAlwaysPass) should be changed if the 
        //server wants to check for a particular policy before grating any connection
        //to the client. For eg. DRM manager server should only allow clients having DRM
        //capability to connect to it. To do so, change CPolicyserver::EAlwaysPass,
        //to the index no. of myElements which checks for ECapabilityDRM i.e index 2
        //in this case. For EACM server, the server should allow only clients having
        //ECapabilityUserEnvironment to connect to it.
        CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass 
        myRangeCount,                                   
        myRanges,
        myElementsIndex,
        myElements,
        };
//Policy table---end
/**
* Our server class - an active object - and therefore derived ultimately from CActive.
* It accepts requests from client threads and forwards
* them to the client session to be dealt with. It also handles the creation
* of the server-side client session.
**/
class SmfServerSymbian : public CPolicyServer
{
  
public:
	/**
	 * Creates a new session with the server; the function
	 * implements the pure virtutal function
	 * defined in class CServer2
	 */
	SmfServerSymbian(CActive::TPriority aActiveObjectPriority,SmfServer* aWrapper);
    static SmfServerSymbian * NewL(CActive::TPriority aActiveObjectPriority,SmfServer* aWrapper);
    ~SmfServerSymbian();
    CSession2 * NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;
  
    /**
     * Returns SmfServer
     */
    SmfServer* wrapper();
    /**
     * Adds session info to the currently active session map iMap. This map is used to
     * retreive the corresponding session to be serviced. The keys for this map is sent
     * alongwith the request to other components
     * @param aSession Session to be added
     * @param aMsg Currently redundant
     */
    TInt addToSessionMap(SmfServerSymbianSession* aSession,const RMessage2& aMsg);
    /**
     * Removes the session from the active list of session map
     * @param aSession Session to be removed.
     * @param aMsg Currently redundant
     */
    TInt removeFromSessionMap(SmfServerSymbianSession* aSession,RMessage2& aMsg);
    /**
     * Returns a symbian session for the given key from the iMap.
     * @param id Session ID in the SmfServer Session map
     */
    SmfServerSymbianSession* findSession(TInt id);
    /**
     * Finds the given session and services it with the data.
     * @param requestID Session ID
     * @param parsedData Parsed and serialized data to be sent to client.
     * @param error Error code
     */
    TInt findAndServiceclient(TInt requestID,QByteArray* parsedData,SmfError error);
	/**
	 * Debugging
	 */
	void writeLog(QString log)const;
//private:
    void ConstructL();
    SmfServer* iWrapper;
    
	/**
	 * Keeps track of the requests sent by each session
	 */
    RHashMap<TInt,CSessionStruct> iMap;
    
    /**
     * The key in the iMap.
     * Its sent to the wrapper
     */
    TInt iRequestID;
    
    TInt iSessionCount;
};

/**
This class represents a session with the  Smf server.
Functions are provided to respond appropriately to client messages.
*/
class SmfServerSymbianSession : public CSession2
{
public:
	/**
	 * Creates a session.
	 * @param aServer The server handle
	 */
	SmfServerSymbianSession(SmfServerSymbian* aServer);
    ~SmfServerSymbianSession();
    /**
     * From CSession2
     */
    void ServiceL(const RMessage2 & aMessage);
    /**
     * Called by the SmfServerSymbian when results are available
     * @param parsedData Parsed serialized data
     * @param error Smf Error code
     */
	void resultsAvailable(QByteArray* parsedData,SmfError error);
	/**
	 * Called by the SmfServer when client authorization finishes.
	 * @param success success of the authorization
	 */
	void clientathorizationFinished(bool success);
protected:
    //TMessageParams ReadMessageAndRetrieveParams (const RMessage2 & aMessage);
    void PanicClient(const RMessage2 & aMessage, TInt aPanic) const;
    /**
     * Called by ServiceL()
     * It tests the function code and then delegates to
     * the appropriate function.
     */
    void HandleClientMessageL(const RMessage2 & aMessage);
    
    //Following are for handling specific client requests
    /**
     * ESmfGetService
     */
    void HandleGetService(const RMessage2 & aMessage);
    
    /**
     * Handles all the opcodes except ESmfGetService
     */
    void HandleCommonServiceL(const RMessage2 & aMessage);
    
    
//private:
    SmfServerSymbian* iServer;
    RMessage2 iMessage;

	//this interface id will be provided by Smf client, will map Smf Client 
	// interface hierarchy
	SmfInterfaceID iInterfaceID;
	TPtr8 iIntfNmaeSymbian;
	TPtr iIntfNameSymbian16;
	TBuf8<125> iInterfaceNametbuf;
	TPtr8 iProviderSymbian;
	HBufC8* iProviderBuf;
	HBufC8* iIntfNameBuf;
	QMap<SmfPluginID,SmfProvider> iPluginIDMap;
	QList<SmfPluginID> iPluginIDList;
	QList<SmfPluginID> iAuthList;
	
	/**
	 * To keep track of the requests sent by this session,not required for now, as only one
	 * outstanding request per session is required
	 */
	RArray<RMessage2> iRequestList;
	/**
	 * Last request opcode
	 */
	TInt iLastRequest;
	//testing
	TBuf<525> dataToPass;
	HBufC8* iBuf;
	HBufC* iBuf16;
	TPtr8 iPtrToBuf;
	QByteArray byteArrayToClnt;
	SmfProvider* providerToClnt;
	TInt iError;
};
#endif // SMFSERVERSYMBIAN_H