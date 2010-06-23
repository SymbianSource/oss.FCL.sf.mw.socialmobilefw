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

#ifndef SMFCLIENTSYMBIAN_H_
#define SMFCLIENTSYMBIAN_H_

#include <e32base.h>
#include <e32cmn.h>
#include "smfglobal.h"
#include "smfobserver.h"
#include "smfprovider.h"
#include <QStringList>
#include "smfclientsymbiansession.h"

/**
 * Private client implementation for Symbian.
 */
class CSmfClientSymbian : public CActive
{

public: // Constructors and destructors
	
	/**
	 * Two phase contruction.
	 * @param aObserver For callback
	 */
     static CSmfClientSymbian* NewL(smfObserver* aObserver=NULL);


     static CSmfClientSymbian* NewLC(smfObserver* aObserver);
     CSmfClientSymbian(smfObserver* aObserver);

     /**
     * ~CSmfClientSymbian.
     * Destructor.
     * Destroys the object and release all memory objects.
     */
     virtual ~CSmfClientSymbian();

 public: 
     /**
      * Calls SendReceive of the RSessionBase object.
      * @param aSerializedData serialized by the caller.
      * @param aInterfaceName Interface name
      * @param requestType Opcode
      * @param maxSize Max data size that is allocated in the client side for
      * receiving this data
      * @param xtraInfo XtraInfo when required by server other than smfProvider
      */
     TInt sendRequest(QByteArray& aSerializedData,
    		 QString aInterfaceName,
    		 SmfRequestTypeID requestType,TInt aMaxAllocation,QByteArray xtraInfo=QByteArray());
     /**
      * This overloaded API is for ESmfGetServices, where data should be
      * fetched synchronously
      */
     QByteArray sendRequest(QString aInterfaceName,
    		 SmfRequestTypeID requestType,TInt maxSize=10);
     
     /**
      * For sending request specific to DSM. These are used by SmfRelationMngr API
      * @param requestType Opcode
      * @param aSerializedData Data to be passed to DSM through server, interpretation
      * depends on Relation manager and DSM
      * @maxSize Size to be allocated in the client side
      * @return Data received from server, interpretation depends on SmfRelationMngr and DSM
      */
     QByteArray sendDSMSyncRequest(SmfRequestTypeID requestType,QByteArray& aSerializedData,SmfError& aErr,TInt maxSize=10);
     /**
      * For testing purpose only
      */
     TInt sendDummyRequest(QByteArray* provider,QString aInterfaceName,
    		 SmfRequestTypeID requestType);

     /**
     * CancelRequest.
     * Cancels an outstanding request.
     */
     void CancelRequest() ;
     


 protected: // Functions from base classes

     /**
     * From CActive, RunL.
     * Callback function.
     * Invoked to handle responses from the server.
     */
     void RunL();

     /**
     * From CActive, DoCancel.
     * Cancels any outstanding operation.
     */
     void DoCancel();
     
     TInt RunError(TInt aError);

 private: // Constructors and destructors

     /**
     * CSmfClientSymbian.
     * Performs the first phase of two phase construction.
     */
     CSmfClientSymbian();

     /**
     * ConstructL.
     * Performs the second phase construction of a
     * CSmfClientSymbian object.
     */
     void ConstructL();
     
     //debugging
     void writeLog(QString log);

 private: // Data

     /**
     * iSession, the server session.
     */
     RSmfClientSymbianSession iSession;
     /**
      * For callback, when the client gets serviced by the server
      */
     smfObserver* iObserver;
     
     //Interface name, to be passed to the server
     TBuf<128> iInterfaceName;
     QString iInterfaceNameQt;
     //Data ptr
     TPtr iDataPtr;
     
     //Bytearray
     QByteArray* iBytearray;
     HBufC* iBuffer;
     TInt iMaxMessageSize;
     
 	/**
 	 * Testing
 	 */
 	QStringList dummyList;
 	QByteArray* dummyArray;

 	
};

#endif /* SMFCLIENTSYMBIAN_H_ */
