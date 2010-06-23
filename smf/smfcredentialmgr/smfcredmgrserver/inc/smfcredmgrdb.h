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
 * This header defines the database-table and the creation of database that will be 
 *  used by the Credential Manager Server to store all 
 *  the credentials related to an Authentication Application
 */
#ifndef SMFCREDMGRDB_H_
#define SMFCREDMGRDB_H_
#include <sqldb.h>
#include <e32base.h>
#include <f32file.h>

//the private folder of the app
_LIT(KSmfCredMgrDbPrivateDir, "C:\\private\\EF0FFBD7\\");
_LIT(KSmfCredMgrDbFileName, "[EF0FFBD7]CREDMGRDB.db");

//Create Table
_LIT8(KCreatePluginIDTable,"CREATE TABLE PluginIDTable(PluginId TEXT PRIMARY KEY, AuthAppId TEXT UNIQUE, IsEnabled INT)");
_LIT8(KCreateURLTable,"CREATE TABLE URLTable(AuthAppId TEXT UNIQUE, URL TEXT,PRIMARY KEY(AuthAppId,URL))");
_LIT8(KCreateRegTokenValidityTable,"CREATE TABLE RegTokenValidityTable(RegistrationToken TEXT PRIMARY KEY, AuthAppId TEXT UNIQUE, Validity BIGINT)");
_LIT8(KCreateAuthParamsTable,"CREATE TABLE AuthParamsTable(AuthAppId TEXT UNIQUE, AuthKey TEXT, AuthSecret TEXT, PRIMARY KEY(AuthAppId,AuthKey))");

/**
 * The creator class of the server database.
 *  Derives from CBase
 */
class CSmfCredMgrDb : public CBase
	{
public:
	/**
	 * New Methods
	 */
	static CSmfCredMgrDb* NewL();
	/**
	 * New Methods
	 */
	static CSmfCredMgrDb* NewLC();

	/**
	 * destructor
	 */
	~CSmfCredMgrDb();

private:
	/**
	 * constructor
	 */
	CSmfCredMgrDb();

	/**
	 * two phase constructor
	 */
	void ConstructL();

	/**
	 * Method to create the Db file
	 * @param aFileName The file name
	 */
	TInt CreateDbFile(const TFileName &aFileName);

	/**
	 * Method to crete the Db tables
	 * @param aFileName The Db file name
	 */
	void CompleteDBCreationL(const TFileName &aFileName);

	/**
	 * Creates the Plugin Id table 
	 */
	TInt PluginIDTableCreate();

	/**
	 * Creates the URL table 
	 */
	TInt URLTableCreate();

	/**
	 * Creates the RegistrationToken-Validity Table  
	 */
	TInt RegTokenValidityTableCreate();

	/**
	 * Creates the Authentication Parameters Table  
	 */
	TInt AuthParamsTableCreate();

	/**
	 * Deletes the Db 
	 * @param aFileName The Db file name
	 */
	TInt DeleteDbFile(const TFileName &aFileName);

private:
	RFs iFileSession;
	TFileName iDbFilePathName;
	RSqlDatabase iDataBase;
	};

#endif /* SMFCREDMGRDB_H_ */
