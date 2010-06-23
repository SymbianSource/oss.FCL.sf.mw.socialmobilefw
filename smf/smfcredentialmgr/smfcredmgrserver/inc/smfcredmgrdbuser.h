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
 * This header provides a handle to use the data-base operations like insertion, query etc. which
 *  is used by the Credential Manager Server Sessions to operate on
 *  Credential Manager Server Database
 */
#ifndef SMFCREDMGRDBUSER_H_
#define SMFCREDMGRDBUSER_H_

#include <sqldb.h>
#include <e32base.h>
#include <f32file.h>
#include "smfcredmgrserversession.h"

//INSERTION
_LIT(KBegin , "BEGIN");
_LIT(KCommit, "COMMIT");
_LIT(KRollback, "ROLLBACK");
_LIT(KSmfPluginIDTableInsert, "INSERT INTO PluginIDTable(PluginId, AuthAppId, IsEnabled) VALUES(:Val1, :Val2, :Val3)");
_LIT(KSmfURLTableInsert, "INSERT INTO URLTable(AuthAppId, URL) VALUES(:Val1, :Val2)");
_LIT(KSmfRegTokenValidityTableInsert, "INSERT INTO RegTokenValidityTable(RegistrationToken, AuthAppId, Validity) VALUES(:Val1, :Val2, :Val3)");
_LIT(KSmfAuthParamsTableInsert, "INSERT INTO AuthParamsTable(AuthAppId, AuthKey, AuthSecret) VALUES(:Val1, :Val2, :Val3)");
//READ
_LIT(KSmfDbReadValidity, "SELECT Validity FROM RegTokenValidityTable WHERE AuthAppId = :iID");
_LIT(KSmfDbReadRegistrationToken, "SELECT RegistrationToken FROM RegTokenValidityTable WHERE AuthAppId = :iID");
_LIT(KSmfDbReadAuthTokens, "SELECT AuthKey, AuthSecret FROM AuthParamsTable WHERE AuthAppId = :iID");
_LIT(KSmfDbReadPluginID, "SELECT PluginId, IsEnabled FROM PluginIDTable WHERE AuthAppId = :iID");
_LIT(KSmfDbReadURL, "SELECT URL FROM URLTable WHERE AuthAppId = :iID");
_LIT(KSmfDbReadAuthAppIdInPluginTable, "SELECT AuthAppId FROM PluginIDTable WHERE PluginId = :iID");
_LIT(KSmfDbReadFlagInPluginTable, "SELECT IsEnabled FROM PluginIDTable WHERE PluginId = :iID");
_LIT(KSmfDbReadAuthAppIdInRegTokenTable, "SELECT AuthAppId, Validity FROM RegTokenValidityTable WHERE RegistrationToken = :iID");
//UPDATE 
_LIT( KUpdatePluginID, "UPDATE PluginIDTable SET PluginId =:iText, IsEnabled =:iFlag WHERE PluginId = :iID");

/**
 * The class to use the server database i.e insert table in Db, query from Db.
 */
class CSmfCredMgrDbUser : public CBase
	{
public:
	/**
	 * New functions
	 * @param aSession the session object using this class
	 */
	static CSmfCredMgrDbUser* NewL(CSmfCredMgrServerSession* aSession);
	static CSmfCredMgrDbUser* NewLC(CSmfCredMgrServerSession* aSession);

	/**
	 * Destructor
	 */
	~CSmfCredMgrDbUser();

public:
	/**
	 * Method to insert PluginIDTable
	 * @param aPluginID ID of the plugin 
	 * @param aAuthAppId ID of the Authentication app associated with the plugin
	 * @param aEnableFlag a flag to indicate if the plugin is enabled, 
	 * 	   i.e aEnableFlag =0 for disabled plugin
	 */
	TInt PluginIDTableInsert(const TDesC& aPluginID, const TDesC& aAuthAppId,
			TBool aEnableFlag);

	/**
	 * Method to insert URLTable
	 * @param aAuthAppId ID of the Authentication app associated with the URLs 
	 * @param aURL the URL to be stored 
	 */
	TInt URLTableInsert(const TDesC& aAuthAppId, const TDesC& aURL);

	/**
	 * Method to insert RegTokenValidityTable
	 * @param aRegToken the Registration token for the authentication app 
	 * @param aAuthAppId ID of the Authentication app
	 * @param aValidity time by which the set will expire 
	 */
	TInt RegTokenValidityTableInsert(const TDesC& aRegToken,
			const TDesC& aAuthAppId, const TUint aValidity);
	/**
	 * Method to insert AuthParamsTable
	 * @param aAuthAppId ID of the Authentication app
	 * @param aKey The Key
	 * @param aSecret The Secret 
	 */
	TInt AuthParamsTableInsert(const TDesC& aAuthAppId, const TDesC& aKey,
			const TDesC& aSecret);

	/**
	 * Method to fetch all the plugins associated with the registration token
	 * @param aRegToken the Registration token of the authentication app
	 * @param aArray The array to be updated with plugin ids 
	 */
	void fetchPluginList(const TDesC& aRegToken, RPointerArray<HBufC>& aArray);

	/**
	 * Method to Key-Secret pairs of the Authentication app
	 * @param aAuthAppId ID of the Authentication app
	 * @param aArray The array containing the key-secret pair
	 */
	void readAuthTokens(const TDesC& aAuthAppId, RArray<TSmfAuthToken>& aArray);

	/**
	 * Method to fetch all the URLs associated with the Authentication app
	 * @param aAuthAppId ID of the Authentication app
	 * @param aArray The array to be updated with URLs 
	 */
	void readURL(const TDesC& aAuthAppId, RPointerArray<HBufC>& aArray);

	/**
	 * Method to fetch Validity of the Authentication app
	 * @param aAuthAppId ID of the Authentication app
	 * @param aValidity time by which the Auth set will expire
	 */
	void readValidity(const TDesC& aAuthAppId, TInt64& aValidity);

	/**
	 * Method to fetch Registration token of the Authentication app from RegTokenValidityTable
	 * @param aAuthAppId ID of the Authentication app
	 * @param aRegToken the Registration token of the authentication app
	 */
	void readRegistrationToken(const TDesC& aAuthAppId, TDesC& aRegToken);

	/**
	 * Method to fetch Authentication app ID from RegTokenValidityTable
	 * @param aRegToken the Registration token of the authentication app
	 * @param aAuthAppId ID of the Authentication app
	 */
	void readAuthAppIdInRegTokenTable(const TDesC& aRegToken, TDes& aAuthAppId);

	/**
	 * Method to fetch Authentication app ID from PluginIDTable
	 * @param aPluginID the ID of the plugin
	 * @param aAuthAppId ID of the Authentication app
	 */
	void readAuthAppIdInPluginIdTable(const TDesC& aPluginID, TDes& aAuthAppId);

	/**
	 * Method to read the IsEnabled flag from plugin Id table
	 * @param aPluginID the ID of the plugin
	 * @param aFlag flag that indicates the plugin id is enables or disabled
	 */
	void readFlagInPluginIdTable(const TDesC& aPluginID, TInt& aFlag);

	/**
	 * Method to change a plugin id in plugin Id table
	 * @param aNewPluginID the ID of the new plugin
	 * @param aFlag flag that indicates the plugin id is enables or disabled
	 * @param aOldPluginID the ID of the plugin to be replaced
	 */
	TInt updatePlugin(const TDesC& aPluginID, const TBool& aFlag,
			const TDesC& aOldPluginID);

	/**
	 * Method to fetch a plugin id associated with Authentication App id, it is 
	 * 	called internally by fetchPluginList().
	 * @param aAuthAppId ID of the Authentication app
	 * @param aArray The array to be updated with plugin ids 
	 */
	void readPluginId(const TDesC& aAuthAppId, RPointerArray<HBufC>& aArray);

private:
	/**
	 * constructor
	 */
	CSmfCredMgrDbUser(CSmfCredMgrServerSession* aSession);

	/**
	 * Two phase constructor
	 */
	void ConstructL();

private:
	//data
	RFs iFileSession;
	TFileName iDbFilePathName;
	RSqlDatabase iDataBase;
	CSmfCredMgrServerSession* iSession;
	};

#endif /* SMFCREDMGRDBUSER_H_ */
