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
 * This file contains routines to handle the data-base operations like insertion, query etc. 
 * on Credential Manager Server Database
 */

#include <BAUTILS.H>
#include "smfcredmgrdbuser.h"
#include "smfcredmgrdb.h"

CSmfCredMgrDbUser* CSmfCredMgrDbUser::NewL(CSmfCredMgrServerSession* aSession)
	{
	CSmfCredMgrDbUser* self = CSmfCredMgrDbUser::NewLC(aSession);
	CleanupStack::Pop(self);
	return (self);
	}

CSmfCredMgrDbUser* CSmfCredMgrDbUser::NewLC(CSmfCredMgrServerSession* aSession)
	{
	CSmfCredMgrDbUser* self = new (ELeave) CSmfCredMgrDbUser(aSession);
	CleanupStack::PushL(self);
	self->ConstructL();
	return (self);
	}

void CSmfCredMgrDbUser::ConstructL()
	{
	TInt err = iFileSession.Connect();
	User::LeaveIfError(err);

	iFileSession.SetSessionToPrivate(EDriveC);

	TFileName dbFileName;
	iFileSession.PrivatePath(dbFileName);
	TFindFile PrivFolder(iFileSession);

	// find drive-private path
	if (KErrNone == PrivFolder.FindByDir(dbFileName, KNullDesC))
		{
		dbFileName.Copy(PrivFolder.File());
		dbFileName.Append(KSmfCredMgrDbFileName);
		iDbFilePathName = dbFileName;
		if (BaflUtils::FileExists(iFileSession, dbFileName))
			{
			//err = iDataBase.Open(dbFileName);
			User::LeaveIfError(iDataBase.Open(dbFileName));
			RSqlDatabase::TIsolationLevel isolationLevel =
					RSqlDatabase::EReadUncommitted;
			iDataBase.SetIsolationLevel(isolationLevel);
			}
		else
			{
			//file not present
			User::Leave(KErrNotFound);
			}
		}
	else
		{
		//path not present
		User::Leave(KErrPathNotFound);
		}
	}

CSmfCredMgrDbUser::CSmfCredMgrDbUser(CSmfCredMgrServerSession* aSession) :
	iSession(aSession)
	{
	}

CSmfCredMgrDbUser::~CSmfCredMgrDbUser()
	{
	iFileSession.Close();
	iDataBase.Close();
	}

TInt CSmfCredMgrDbUser::PluginIDTableInsert(const TDesC& aPluginId,
		const TDesC& aAuthAppId, TBool aEnableFlag)
	{
	TInt err(KErrNone);
	RSqlStatement sqlStatement;
	RSqlDatabase db;

	err = db.Open(iDbFilePathName);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	err = sqlStatement.Prepare(db, KSmfPluginIDTableInsert);
	TInt paramIndex(KErrNone);

	TBuf<KMaxBufSize> pluginbuf(aPluginId);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val1"));
	err = sqlStatement.BindText(paramIndex, pluginbuf);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val2"));
	err = sqlStatement.BindText(paramIndex, aAuthAppId);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val3"));
	err = sqlStatement.BindInt(paramIndex, aEnableFlag);

	err = db.Exec(KBegin);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
	err = sqlStatement.Exec();
	if (KSqlErrConstraint == err)
		{
		//Table already present.
		}
	else if (err < KErrNone)
		{
		__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
		}

	err = db.Exec(KCommit);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	//if commit fails we have to roll back
	if (err < KErrNone)
		{
		err = db.Exec(KRollback);
		}
	sqlStatement.Close();
	db.Close();
	if (err >= 0)
		{
		return KErrNone;
		}
	return err;
	}

TInt CSmfCredMgrDbUser::URLTableInsert(const TDesC& aAuthAppId,
		const TDesC& aURL)
	{
	TInt err(KErrNone);
	RSqlStatement sqlStatement;
	RSqlDatabase db;

	err = db.Open(iDbFilePathName);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	err = sqlStatement.Prepare(db, KSmfURLTableInsert);
	TInt paramIndex(KErrNone);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val1"));
	err = sqlStatement.BindText(paramIndex, aAuthAppId);

	TBuf<KMaxBufSize> urlbuf(aURL);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val2"));
	err = sqlStatement.BindText(paramIndex, urlbuf);

	err = db.Exec(KBegin);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
	err = sqlStatement.Exec();
	if (KSqlErrConstraint == err)
		{
		//Table already present.
		}
	else if (err < KErrNone)
		{
		__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
		}

	err = db.Exec(KCommit);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	//if commit fails we have to roll back
	if (err < KErrNone)
		{
		err = db.Exec(KRollback);
		}
	sqlStatement.Close();
	db.Close();
	if (err >= 0)
		{
		return KErrNone;
		}
	return err;
	}

TInt CSmfCredMgrDbUser::RegTokenValidityTableInsert(const TDesC& aRegToken,
		const TDesC& aAuthAppId, const TUint aValidity)
	{
	TInt err(KErrNone);
	RSqlStatement sqlStatement;
	RSqlDatabase db;

	TBuf<KMaxBufSize> regTokenBuf(aRegToken);

	err = db.Open(iDbFilePathName);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	err = sqlStatement.Prepare(db, KSmfRegTokenValidityTableInsert);
	TInt paramIndex(KErrNone);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val1"));
	err = sqlStatement.BindText(paramIndex, aRegToken);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val2"));
	err = sqlStatement.BindText(paramIndex, aAuthAppId);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val3"));
	err = sqlStatement.BindInt(paramIndex, aValidity);

	err = db.Exec(KBegin);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
	err = sqlStatement.Exec();
	if (KSqlErrConstraint == err)
		{
		//Table already present.
		}
	else if (err < KErrNone)
		{
		__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
		}

	err = db.Exec(KCommit);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	//if commit fails we have to roll back
	if (err < KErrNone)
		{
		err = db.Exec(KRollback);
		}
	sqlStatement.Close();
	db.Close();
	if (err >= 0)
		{
		return KErrNone;
		}
	return err;

	}

TInt CSmfCredMgrDbUser::AuthParamsTableInsert(const TDesC& aAuthAppId,
		const TDesC& aKey, const TDesC& aSecret)
	{
	TInt err(KErrNone);
	RSqlStatement sqlStatement;
	RSqlDatabase db;

	TBuf<KMaxBufSize> keyBuf(aKey);
	TBuf<KMaxBufSize> secretBuf(aSecret);

	err = db.Open(iDbFilePathName);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	err = sqlStatement.Prepare(db, KSmfAuthParamsTableInsert);
	TInt paramIndex(KErrNone);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val1"));
	err = sqlStatement.BindText(paramIndex, aAuthAppId);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val2"));
	err = sqlStatement.BindText(paramIndex, keyBuf);

	paramIndex = sqlStatement.ParameterIndex(_L(":Val3"));
	err = sqlStatement.BindText(paramIndex, secretBuf);

	err = db.Exec(KBegin);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
	err = sqlStatement.Exec();
	if (KSqlErrConstraint == err)
		{
		//Table already present.
		}
	else if (err < KErrNone)
		{
		__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
		}

	err = db.Exec(KCommit);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	//if commit fails we have to roll back
	if (err < KErrNone)
		{
		err = db.Exec(KRollback);
		}
	sqlStatement.Close();
	db.Close();
	if (err >= 0)
		{
		return KErrNone;
		}
	return err;
	}

void CSmfCredMgrDbUser::readFlagInPluginIdTable(const TDesC& aPluginID,
		TInt& aFlag)
	{
	TInt err(KErrNone);

	RSqlStatement sqlReadStatement;
	TInt paramIndex(KErrNone);

	err = sqlReadStatement.Prepare(iDataBase, KSmfDbReadFlagInPluginTable);

	paramIndex = sqlReadStatement.ParameterIndex(_L(":iID"));
	err = sqlReadStatement.BindText(paramIndex, aPluginID);

	while ((err = sqlReadStatement.Next()) == KSqlAtRow)
		{
		//sometimes sqlStmt.Next returns KSqlAtRow even if no row is present
		if (!sqlReadStatement.IsNull(0))
			{
			aFlag = sqlReadStatement.ColumnInt(0);
			}
		}
	sqlReadStatement.Close();
	}

void CSmfCredMgrDbUser::readAuthAppIdInPluginIdTable(const TDesC& aPluginID,
		TDes& aAuthAppId)
	{
	TInt err(KErrNone);

	RSqlStatement sqlReadStatement;
	TInt paramIndex(KErrNone);

	err = sqlReadStatement.Prepare(iDataBase, KSmfDbReadAuthAppIdInPluginTable);

	paramIndex = sqlReadStatement.ParameterIndex(_L(":iID"));
	err = sqlReadStatement.BindText(paramIndex, aPluginID);

	while ((err = sqlReadStatement.Next()) == KSqlAtRow)
		{
		//sometimes sqlStmt.Next returns KSqlAtRow even if no row is present
		if (!sqlReadStatement.IsNull(0))
			{
			sqlReadStatement.ColumnText(0, aAuthAppId);
			}
		}
	sqlReadStatement.Close();
	}

void CSmfCredMgrDbUser::readAuthTokens(const TDesC& aAuthAppId, RArray<
		TSmfAuthToken>& aArray)
	{
	TInt err(KErrNone);

	RSqlStatement sqlReadStatement;
	TInt paramIndex(KErrNone);

	err = sqlReadStatement.Prepare(iDataBase, KSmfDbReadAuthTokens);
	paramIndex = sqlReadStatement.ParameterIndex(_L(":iID"));
	err = sqlReadStatement.BindText(paramIndex, aAuthAppId);

	while ((err = sqlReadStatement.Next()) == KSqlAtRow)
		{
		//sometimes sqlStmt.Next returns KSqlAtRow even if no row is present
		if (!sqlReadStatement.IsNull(0))
			{
			TSmfAuthToken Set;

			Set.iKey = HBufC::NewL(KMaxAuthTokenLength);
			Set.iSecret = HBufC::NewL(KMaxAuthTokenLength);

			TBuf<KMaxBufSize> keyBuf;
			TBuf<KMaxBufSize> secretBuf;

			sqlReadStatement.ColumnText(0, keyBuf);
			sqlReadStatement.ColumnText(1, secretBuf);

			Set.iKey->Des().Copy(keyBuf);
			Set.iSecret->Des().Copy(secretBuf);
			//add it to the array
			aArray.Append(Set);

			}
		else
			{
			__ASSERT_DEBUG( 0, User::Invariant());
			}
		}
	sqlReadStatement.Close();
	}

void CSmfCredMgrDbUser::fetchPluginList(const TDesC& aRegToken, RPointerArray<
		HBufC>& aArray)
	{
	TBuf<KMaxBufSize> authAppID;
	readAuthAppIdInRegTokenTable(aRegToken, authAppID);
	readPluginId(authAppID, aArray);
	}

void CSmfCredMgrDbUser::readAuthAppIdInRegTokenTable(const TDesC& aRegToken,
		TDes& aAuthAppId)
	{
	TInt err(KErrNone);

	RSqlStatement sqlReadStatement;
	TInt paramIndex(KErrNone);
	TInt64 duration;
	TBuf<KMaxBufSize> tokenBuf(aRegToken);
	err = sqlReadStatement.Prepare(iDataBase,
			KSmfDbReadAuthAppIdInRegTokenTable);
	paramIndex = sqlReadStatement.ParameterIndex(_L(":iID"));
	err = sqlReadStatement.BindText(paramIndex, tokenBuf);

	while ((err = sqlReadStatement.Next()) == KSqlAtRow)
		{
		//sometimes sqlStmt.Next returns KSqlAtRow even if no row is present
		if (!sqlReadStatement.IsNull(0))
			{
			duration = sqlReadStatement.ColumnInt(1);
			if (duration) //to be checked with epoch date-time
				{
				sqlReadStatement.ColumnText(0, aAuthAppId);
				}
			}
		else
			{
			__ASSERT_DEBUG( 0, User::Invariant());
			}
		}
	sqlReadStatement.Close();
	}
void CSmfCredMgrDbUser::readPluginId(const TDesC& aAuthAppId, RPointerArray<
		HBufC>& aArray)
	{
	TInt err(KErrNone);
	RSqlStatement sqlReadStatement;
	TInt paramIndex(KErrNone);
	TInt flag;

	err = sqlReadStatement.Prepare(iDataBase, KSmfDbReadPluginID);
	paramIndex = sqlReadStatement.ParameterIndex(_L(":iID"));
	err = sqlReadStatement.BindText(paramIndex, aAuthAppId);

	while ((err = sqlReadStatement.Next()) == KSqlAtRow)
		{
		//sometimes sqlStmt.Next returns KSqlAtRow even if no row is present
		if (!sqlReadStatement.IsNull(0))
			{
			flag = sqlReadStatement.ColumnInt(1);
			if (flag > 0)
				{
				TBuf<KMaxBufSize> pluginbuf;
				HBufC* buf = HBufC::NewL(KMaxBufSize);
				sqlReadStatement.ColumnText(0, pluginbuf);
				buf->Des().Copy(pluginbuf);
				aArray.Append(buf);
				}
			}
		else
			{
			__ASSERT_DEBUG( 0, User::Invariant());
			}
		}
	sqlReadStatement.Close();
	}

void CSmfCredMgrDbUser::readURL(const TDesC& aAuthAppId,
		RPointerArray<HBufC>& aArray)
	{
	TInt err(KErrNone);

	RSqlStatement sqlReadStatement;
	TInt paramIndex(KErrNone);

	err = sqlReadStatement.Prepare(iDataBase, KSmfDbReadURL);
	paramIndex = sqlReadStatement.ParameterIndex(_L(":iID"));
	err = sqlReadStatement.BindText(paramIndex, aAuthAppId);

	while ((err = sqlReadStatement.Next()) == KSqlAtRow)
		{
		//sometimes sqlStmt.Next returns KSqlAtRow even if no row is present
		if (!sqlReadStatement.IsNull(0))
			{
			TBuf<KMaxBufSize> urlBuf;
			HBufC* buf = HBufC::NewL(KMaxBufSize);
			sqlReadStatement.ColumnText(0, urlBuf);
			buf->Des().Copy(urlBuf);
			aArray.Append(buf);
			}
		else
			{
			__ASSERT_DEBUG( 0, User::Invariant());
			}
		}
	sqlReadStatement.Close();
	}

void CSmfCredMgrDbUser::readValidity(const TDesC& aAuthAppId, TInt64& aValidity)
	{
	TInt err(KErrNone);

	RSqlStatement sqlReadStatement;
	TInt paramIndex(KErrNone);

	err = sqlReadStatement.Prepare(iDataBase, KSmfDbReadValidity);
	paramIndex = sqlReadStatement.ParameterIndex(_L(":iID"));
	err = sqlReadStatement.BindText(paramIndex, aAuthAppId);

	while ((err = sqlReadStatement.Next()) == KSqlAtRow)
		{
		//sometimes sqlStmt.Next returns KSqlAtRow even if no row is present
		if (!sqlReadStatement.IsNull(0))
			{
			aValidity = sqlReadStatement.ColumnInt64(1);
			}
		else
			{
			__ASSERT_DEBUG( 0, User::Invariant());
			}
		}
	sqlReadStatement.Close();

	}

void CSmfCredMgrDbUser::readRegistrationToken(const TDesC& aAuthAppId,
		TDesC& aRegToken)
	{
	TInt err(KErrNone);

	RSqlStatement sqlReadStatement;
	TInt paramIndex(KErrNone);

	err = sqlReadStatement.Prepare(iDataBase, KSmfDbReadRegistrationToken);
	paramIndex = sqlReadStatement.ParameterIndex(_L(":iID"));
	err = sqlReadStatement.BindText(paramIndex, aAuthAppId);

	while ((err = sqlReadStatement.Next()) == KSqlAtRow)
		{
		//sometimes sqlStmt.Next returns KSqlAtRow even if no row is present
		if (!sqlReadStatement.IsNull(0))
			{
			aRegToken = sqlReadStatement.ColumnTextL(0);//aKey.AllocL();
			}
		else
			{
			__ASSERT_DEBUG( 0, User::Invariant());
			}
		}
	sqlReadStatement.Close();
	}

TInt CSmfCredMgrDbUser::updatePlugin(const TDesC& aPluginID,
		const TBool& aFlag, const TDesC& aOldPluginID)
	{
	TInt err(KErrNone);
	RSqlStatement sqlStatement;
	TInt paramIndex(KErrNone);
	TBuf<KMaxBufSize> newPluginBuf(aPluginID);
	TBuf<KMaxBufSize> OldPluginBuf(aOldPluginID);
	RSqlDatabase db;

	err = db.Open(iDbFilePathName);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	err = sqlStatement.Prepare(db, KUpdatePluginID);

	paramIndex = sqlStatement.ParameterIndex(_L(":iText"));
	err = sqlStatement.BindText(paramIndex, newPluginBuf);

	paramIndex = sqlStatement.ParameterIndex(_L(":iFlag"));
	err = sqlStatement.BindInt(paramIndex, aFlag);

	paramIndex = sqlStatement.ParameterIndex(_L(":iID"));
	err = sqlStatement.BindText(paramIndex, OldPluginBuf);

	err = db.Exec(KBegin);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
	err = sqlStatement.Exec();
	if (KSqlErrConstraint == err)
		{
		//
		}
	else if (err < KErrNone)
		{
		__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());
		}

	err = db.Exec(KCommit);
	__ASSERT_DEBUG( (err >= KErrNone), User::Invariant());

	//if commit fails we have to roll back
	if (err < KErrNone)
		{
		err = db.Exec(KRollback);
		}
	sqlStatement.Close();
	db.Close();
	if (err >= 0)
		{
		return KErrNone;
		}
	return err;
	}
