/**
 * SmfCredMgrDb.cpp
 *
 *  Created on: Apr 27, 2010
 *      Author: pritam
 */

#include "smfcredmgrdb.h"
#include <BAUTILS.H>

CSmfCredMgrDb* CSmfCredMgrDb::NewL()
	{
	CSmfCredMgrDb* self = CSmfCredMgrDb::NewLC();
	CleanupStack::Pop(self);
	return (self);
	}

CSmfCredMgrDb* CSmfCredMgrDb::NewLC()
	{
	CSmfCredMgrDb* self = new (ELeave) CSmfCredMgrDb();
	CleanupStack::PushL(self);
	self->ConstructL();
	return (self);
	}

CSmfCredMgrDb::CSmfCredMgrDb()
	{
	RDebug::Printf("in constructor db");
	}

void CSmfCredMgrDb::ConstructL()
	{
	//create db file 
	TInt err = iFileSession.Connect();
	User::LeaveIfError(err);

	err = iFileSession.MkDirAll(KSmfCredMgrDbPrivateDir);

	TFileName dbFileName = KSmfCredMgrDbPrivateDir();
	dbFileName.Append(KSmfCredMgrDbFileName);
	iDbFilePathName = dbFileName;
	if (!BaflUtils::FileExists(iFileSession, dbFileName))
		{
		err = iFileSession.SetSessionToPrivate(EDriveC);

		// no database exists so we make one
		CompleteDBCreationL(dbFileName);
		}
	else if (BaflUtils::FileExists(iFileSession, dbFileName))
		{
		
		//Reset if necessary
		}

	}

CSmfCredMgrDb::~CSmfCredMgrDb()
	{
	//delete db file

	iDataBase.Close();
	iFileSession.Close();
	RDebug::Printf("in destructor db");
	}

void CSmfCredMgrDb::CompleteDBCreationL(const TFileName &aFileName)
	{
	User::LeaveIfError(CreateDbFile(aFileName));
	User::LeaveIfError(PluginIDTableCreate());
	User::LeaveIfError(URLTableCreate());
	User::LeaveIfError(RegTokenValidityTableCreate());
	User::LeaveIfError(AuthParamsTableCreate());
	}

TInt CSmfCredMgrDb::CreateDbFile(const TFileName &aFileName)
	{
	TInt err(KErrNone);
	err = iDataBase.Create(aFileName);
	if (err != KErrNone)
		{
		//the empty file is deleted in case of error
		DeleteDbFile(aFileName);
		}
	return err;

	}

TInt CSmfCredMgrDb::PluginIDTableCreate()
	{
	TInt err(KErrNone);
	err = iDataBase.Exec(KCreatePluginIDTable);
	if (err >= 0)
		{
		return KErrNone;
		}
	//delete file in case of incomplete table creation
	DeleteDbFile(iDbFilePathName);
	return err;
	}

TInt CSmfCredMgrDb::URLTableCreate()
	{
	TInt err(KErrNone);
	err = iDataBase.Exec(KCreateURLTable);
	if (err >= 0)
		{
		return KErrNone;
		}
	//delete file in case of incomplete table creation
	DeleteDbFile(iDbFilePathName);
	return err;
	}

TInt CSmfCredMgrDb::RegTokenValidityTableCreate()
	{
	TInt err(KErrNone);
	err = iDataBase.Exec(KCreateRegTokenValidityTable);
	if (err >= 0)
		{
		return KErrNone;
		}
	//delete file in case of incomplete table creation
	DeleteDbFile(iDbFilePathName);
	return err;
	}

TInt CSmfCredMgrDb::AuthParamsTableCreate()
	{
	TInt err(KErrNone);
	err = iDataBase.Exec(KCreateAuthParamsTable);
	if (err >= 0)
		{
		return KErrNone;
		}
	//delete file in case of incomplete table creation
	DeleteDbFile(iDbFilePathName);
	return err;
	}

TInt CSmfCredMgrDb::DeleteDbFile(const TFileName &aFileName)
	{
	//close the sql handle before deleting
	iDataBase.Close();
	TInt err = iDataBase.Delete(aFileName);
	return err;
	}

