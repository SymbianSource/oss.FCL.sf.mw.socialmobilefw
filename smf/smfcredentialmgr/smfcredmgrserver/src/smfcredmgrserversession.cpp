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

#include <S32MEM.H>
#include <e32math.h>
#include <time.h>
#include <stdlib.h>
#include <random.h>
#include <smfcredmgrcommon.h>
#include <smfcredmgrclientdatastruct.h>
#include <smfutils.h>


#include "smfcredmgrserversession.h"
#include "smfcredmgrserver.h"
#include "smfcredmgrdbuser.h"

CSmfCredMgrServerSession* CSmfCredMgrServerSession::NewLC(
		CSmfCredMgrServer& aServer)
	{
	CSmfCredMgrServerSession* self = new (ELeave) CSmfCredMgrServerSession(
			aServer);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CSmfCredMgrServerSession* CSmfCredMgrServerSession::NewL(
		CSmfCredMgrServer& aServer)
	{
	CSmfCredMgrServerSession* self = CSmfCredMgrServerSession::NewLC(aServer);
	CleanupStack::Pop(self);
	return self;
	}

CSmfCredMgrServerSession::CSmfCredMgrServerSession(CSmfCredMgrServer& aServer) :
	iServer(aServer)
	{
	RDebug::Printf("in constructor CSmfCredMgrServerSession");
	}

void CSmfCredMgrServerSession::ConstructL()
	{
	iDbUser = CSmfCredMgrDbUser::NewL(this);
	iKeyStore = CSmfKeyStoreManager::NewL();
	}

CSmfCredMgrServerSession::~CSmfCredMgrServerSession()
	{
	RDebug::Printf("in destructor CSmfCredMgrServerSession");
	delete iDbUser;
	delete iKeyStore;
	}

void CSmfCredMgrServerSession::ServiceL(const RMessage2& aMessage)
	{
	TInt err = KErrNone;
	switch (aMessage.Function())
		{

		case ESendAuthDataSet:
			{
			//create buffer to read the received data
			RBuf8 dataBuf;
			CleanupClosePushL(dataBuf);
			dataBuf.CreateL(aMessage.GetDesLength(0));
			aMessage.ReadL(0, dataBuf);
			CSmfFetchAuthTokenSet* fetchAuthTokenSetParams =
					new (ELeave) CSmfFetchAuthTokenSet;

			CleanupStack::PushL(fetchAuthTokenSetParams);

			fetchAuthTokenSetParams->InternalizeL(dataBuf);

			getTokenArray(fetchAuthTokenSetParams);

			//create buffer to serialize the data to be sent
			CBufFlat* buf = CBufFlat::NewL(512);
			CleanupStack::PushL(buf);
			RBufWriteStream stream(*buf);
			CleanupClosePushL(stream);

			fetchAuthTokenSetParams->ExternalizeL(stream);

			stream.CommitL();

			TPtr8 bufPtr = buf->Ptr(0);

			TInt err = aMessage.Write(1, bufPtr);

			CleanupStack::PopAndDestroy(&stream);
			CleanupStack::PopAndDestroy(buf);
			CleanupStack::PopAndDestroy(fetchAuthTokenSetParams);
			CleanupStack::PopAndDestroy(&dataBuf);
			aMessage.Complete(err);
			}
			break;

		case EUpdatePluginIDList:
			{
			//create buffer to read the received data
			RBuf8 dataBuf;
			CleanupClosePushL(dataBuf);
			dataBuf.CreateL(aMessage.GetDesLength(0));
			aMessage.ReadL(0, dataBuf);

			CSmfPluginIDUpdate* changePluginListParams =
					new (ELeave) CSmfPluginIDUpdate;
			CleanupStack::PushL(changePluginListParams);

			changePluginListParams->InternalizeL(dataBuf);

			iDbUser->updatePlugin(changePluginListParams->iNewPluginID->Des(),
					changePluginListParams->pluginIDEnabled,
					changePluginListParams->iOldPluginID->Des());

			CleanupStack::PopAndDestroy(changePluginListParams);
			CleanupStack::PopAndDestroy(&dataBuf);
			aMessage.Complete(err);
			}
			break;

		case ESendURLList:
			{
			//create buffer to read the received data
			RBuf8 dataBuf;
			CleanupClosePushL(dataBuf);
			dataBuf.CreateL(aMessage.GetDesLength(0));
			aMessage.ReadL(0, dataBuf);

			CSmfURLListParams* fetchURLListParams =
					new (ELeave) CSmfURLListParams;
			CleanupStack::PushL(fetchURLListParams);

			fetchURLListParams->InternalizeL(dataBuf);

			fetchURLs(fetchURLListParams);

			//to serialize data we need a buffer
			CBufFlat* buf = CBufFlat::NewL(512);
			CleanupStack::PushL(buf);
			RBufWriteStream stream(*buf);
			CleanupClosePushL(stream);

			fetchURLListParams->ExternalizeL(stream);

			stream.CommitL();

			TPtr8 bufPtr = buf->Ptr(0);

			TInt err = aMessage.Write(1, bufPtr);

			CleanupStack::PopAndDestroy(&stream);
			CleanupStack::PopAndDestroy(buf);
			CleanupStack::PopAndDestroy(fetchURLListParams);
			CleanupStack::PopAndDestroy(&dataBuf);
			aMessage.Complete(err);
			}
			break;

		case ESendPluginIDList:
			{
			//create buffer to read
			RBuf8 dataBuf;
			CleanupClosePushL(dataBuf);
			dataBuf.CreateL(aMessage.GetDesLength(0));
			aMessage.ReadL(0, dataBuf);

			CSmfPluginIDListParams* fetchPluginListParams =
					new (ELeave) CSmfPluginIDListParams;
			CleanupStack::PushL(fetchPluginListParams);
			fetchPluginListParams->InternalizeL(dataBuf);

			fetchPluginIDs(fetchPluginListParams);

			//to serialize data we need a buffer
			CBufFlat* buf = CBufFlat::NewL(512);
			CleanupStack::PushL(buf);
			RBufWriteStream stream(*buf);
			CleanupClosePushL(stream);
			fetchPluginListParams->ExternalizeL(stream);

			stream.CommitL();

			TPtr8 bufPtr = buf->Ptr(0);

			TInt err = aMessage.Write(1, bufPtr);

			CleanupStack::PopAndDestroy(&stream);
			CleanupStack::PopAndDestroy(buf);
			CleanupStack::PopAndDestroy(fetchPluginListParams);
			CleanupStack::PopAndDestroy(&dataBuf);
			aMessage.Complete(err);
			}
			break;

		case ECheckPluginAuthentication:
			{
			//create buffer to read
			RBuf8 dataBuf;
			CleanupClosePushL(dataBuf);

			dataBuf.CreateL(aMessage.GetDesLength(0));
			aMessage.ReadL(0, dataBuf);

			RDesReadStream stream(dataBuf);
			CleanupClosePushL(stream);

			TBool isAuthenticated = EFalse;

			HBufC* buf = SmfUtils::InternalizeDesL(stream);
			CleanupStack::PushL(buf);
			TBuf<KMaxBufSize> pluginIDBuf(buf->Des());

			TInt value = 0;
			iDbUser->readFlagInPluginIdTable(pluginIDBuf, value);
			if (value > 0)
				isAuthenticated = ETrue;
			else
				isAuthenticated = EFalse;

			HBufC8* isAuthenticatedBuf = HBufC8::NewLC(32);

			TPtr8 isAuthenticatedBufPtr(isAuthenticatedBuf->Des());
			isAuthenticatedBufPtr.AppendNum(isAuthenticated);

			TInt err = aMessage.Write(1, isAuthenticatedBufPtr);

			CleanupStack::PopAndDestroy(isAuthenticatedBuf);
			CleanupStack::PopAndDestroy(buf);
			CleanupStack::PopAndDestroy(&stream);
			CleanupStack::PopAndDestroy(&dataBuf);
			aMessage.Complete(err);
			}
			break;

		case EStoreAuthData:
			{
			//create buffer to read
			RBuf8 dataBuf;
			CleanupClosePushL(dataBuf);
			dataBuf.CreateL(aMessage.GetDesLength(0));
			aMessage.ReadL(0, dataBuf);

			CSmfStoreAuthParams* authenticationProcessData =
					new (ELeave) CSmfStoreAuthParams;
			CleanupStack::PushL(authenticationProcessData);

			authenticationProcessData->InternalizeL(dataBuf);


			TBuf8<56> regToken;
			regToken.SetLength(regToken.MaxLength());
			TRandom::RandomL(regToken);
			authenticationProcessData->iRegistrationToken = HBufC::NewL(KMaxRegistrationTokenLength);
			TPtr tokenPtr(authenticationProcessData->iRegistrationToken->Des());
			tokenPtr.Copy(regToken);
			storeInDb(authenticationProcessData);

			aMessage.WriteL(1, regToken);
			
			CleanupStack::PopAndDestroy(authenticationProcessData->iRegistrationToken);
			CleanupStack::PopAndDestroy(authenticationProcessData);
			CleanupStack::PopAndDestroy(&dataBuf);
			aMessage.Complete(err);
			}
			break;
		case ESmfHMACSHA1SignMessage:
		case ESmfRSASignMessage:
		case ESmfStoreRSAKey:
		case ESmfDeleteKeys:
			{
			iKeyStore->HandleMessageL(aMessage);
			}
			break;
		default:
			//todo -panic client
			break;
		}
	}

void CSmfCredMgrServerSession::storeInDb(
		CSmfStoreAuthParams* aAuthenticationProcessData)
	{
	TBuf<KMaxBufSize> authAppIDbuf(
			aAuthenticationProcessData->iAuthAppID->Des());

	iDbUser->RegTokenValidityTableInsert(
			aAuthenticationProcessData->iRegistrationToken->Des(),
			authAppIDbuf, aAuthenticationProcessData->iValidity);

	for (int i = 0; i < aAuthenticationProcessData->iAuthTokenArray.Count(); i++)
		{
		iDbUser->AuthParamsTableInsert(
				authAppIDbuf,
				aAuthenticationProcessData->iAuthTokenArray[i].iKey->Des(),
				aAuthenticationProcessData->iAuthTokenArray[i].iSecret->Des());
		}

	//iterate pluginlist and insert in DB
	for (int i = 0; i < aAuthenticationProcessData->iPluginIDList.Count(); i++)
		{
		HBufC* buf = aAuthenticationProcessData->iPluginIDList[i];
		iDbUser->PluginIDTableInsert(buf->Des(), authAppIDbuf,
				aAuthenticationProcessData->pluginIDEnabled);
		}

	//iterate URLlist and insert in DB
	for (int i = 0; i < aAuthenticationProcessData->iURLList.Count(); i++)
		{
		HBufC* buf = aAuthenticationProcessData->iURLList[i];
		iDbUser->URLTableInsert(authAppIDbuf, buf->Des());
		}
	}

void CSmfCredMgrServerSession::fetchPluginIDs(CSmfPluginIDListParams* aParams)
	{
	TBuf<KMaxBufSize> tokenBuf(aParams->iRegistrationToken->Des());
	iDbUser->fetchPluginList(tokenBuf, aParams->iPluginList);
	}

void CSmfCredMgrServerSession::fetchURLs(CSmfURLListParams* aArg)
	{
	TBuf<KMaxBufSize> authAppIDBuf;
	iDbUser->readAuthAppIdInPluginIdTable(aArg->iPluginID->Des(), authAppIDBuf);
	iDbUser->readURL(authAppIDBuf, aArg->iURLList);
	}


void CSmfCredMgrServerSession::getTokenArray(CSmfFetchAuthTokenSet* aParams)
	{
	TBuf<KMaxBufSize> authAppIDBuf;
	iDbUser->readAuthAppIdInRegTokenTable(aParams->iRegistrationToken->Des(),
			authAppIDBuf);
	iDbUser->readAuthTokens(authAppIDBuf, aParams->iAuthTokenArray);
	}

