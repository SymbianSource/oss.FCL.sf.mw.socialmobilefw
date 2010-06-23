/**
 * Copyright (c) 2010 Sasken Communication Technologies Ltd.
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of the "Eclipse Public License v1.0"
 * which accompanies  this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html"
 *
 * Initial Contributors:
 * Pritam Roy Biswas, Sasken Communication Technologies Ltd - Initial contribution
 *
 * Description:
 * Header file for Credential Manager Server Session.
 */

#ifndef SMFCREDMGRSERVERSESSION_H_
#define SMFCREDMGRSERVERSESSION_H_

#include <e32base.h>
#include "smfcredmgrserver.h"
#include "smfcredmgrclientdatastruct.h"
#include "smfkeystoremanager.h"

/**
 * Forward declarations
 */
class CSmfCredMgrDbUser;

/**
 * Maximun size for buffer
 */
const TInt KMaxBufSize = 512;

/**
 * Class to establish a client session at server side.
 *  Derives from CSession2
 */
class CSmfCredMgrServerSession : public CSession2
	{
public:
	/**
	 * New Methods
	 * @param aServer the server object
	 */
	static CSmfCredMgrServerSession* NewL(CSmfCredMgrServer& aServer);
	static CSmfCredMgrServerSession* NewLC(CSmfCredMgrServer& aServer);

	/**
	 * Destructor
	 */
	~CSmfCredMgrServerSession();

	/**
	 * Constructor
	 * @param aServer The server object
	 */
	CSmfCredMgrServerSession(CSmfCredMgrServer& aServer);

public:
	// from CSession2

	/**
	 * Handles the servicing of a client request that has been 
	 * passed to the server
	 * @param aMessage The message containing the details of the client request
	 */
	virtual void ServiceL(const RMessage2& aMessage);

private:
	/**
	 * Two-Phase constructor
	 */
	void ConstructL();

	/**
	 * Stores data to Db during Authentication process
	 * @param aParams class containg the data to be stored at Db
	 */
	void storeInDb(CSmfStoreAuthParams* aParams);

	/**
	 * Retieves the plugin ids from Db
	 * @param aParams class object to be updated
	 */
	void fetchPluginIDs(CSmfPluginIDListParams* aParams);

	/**
	 * Retieves the URLs from Db
	 * @param aArg class object to be updated
	 */
	void fetchURLs(CSmfURLListParams* aArg);

	/**
	 * retrieves each token set from Db and updates the array of CSmfFetchAuthTokenSet
	 * @param aParams class containg the array to be filled
	 */
	void getTokenArray(CSmfFetchAuthTokenSet* aParams);


private:

	CSmfCredMgrServer& iServer;
	CSmfCredMgrDbUser* iDbUser;
	CSmfKeyStoreManager* iKeyStore;
	CSmfFetchAuthTokenSet* iFetchAuthTokenSetParams;

	};

#endif /* SMFCREDMGRSERVERSESSION_H_ */
