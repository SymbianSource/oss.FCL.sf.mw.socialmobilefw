/*
 * smfkeystoremanager.h
 *
 *  Created on: 21.4.2010
 *      Author: lassela
 */

#ifndef SMFKEYSTOREMANAGER_H_
#define SMFKEYSTOREMANAGER_H_

#include <e32base.h>
#include <f32file.h>

#ifdef	SYMBIAN_V3
#include <rmpointerarray.h>
#include <platform/mkeystore.h>
#include <platform/flogger.h>

class CUnifiedKeyStore;
class CCTKeyInfo;
class CRSASignature;
#endif

class CSmfSignParameters;
class CSmfRsaKeyParameters;

class CSmfKeyStoreManager : public CActive 
	{
public:
	static CSmfKeyStoreManager* NewL();
	static CSmfKeyStoreManager* NewLC();
	
	~CSmfKeyStoreManager();
	
	void HandleMessageL( const RMessage2& aMessage );
	
//	void GenerateKeyL( const RMessage2& aMessage );
//	void StoreRSAKeyL( const RMessage2& aMessage );
	
private: // from CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	
private:
	CSmfKeyStoreManager();
	void ConstructL();
	
	void ContinueMessageHandlingL();
	
	void StoreRSAKeyL();
	void RSA_SHA1_SignMessageL();
	void HMAC_SHA1_SignMessageL();
		
	void DeleteKeys();
	
	void SetPassphraseTimeout();
	
	void ReadSignParametersL();
	void ReadRsaKeyParametersL();
	
private:
	RFs iFs;
	
	RPointerArray<RMessage2> iMessages;
	CSmfSignParameters* iSignParameters;
	CSmfRsaKeyParameters* iRsaKeyParameters;
#ifdef	SYMBIAN_V3	
	CCTKeyInfo*	iImportedKey;
	CUnifiedKeyStore* iKeyStore;
	RMPointerArray<CCTKeyInfo> iKeys;
	MRSASigner* iRSASigner;
	CRSASignature* iRSASignature;
#endif
	
	enum {
		EInitializingKeystore,
		ESettingPassphraseTimeout,
		EInitialized,
		EGeneratingKey,
		EImportingKey,
		EGettingKeyList,
		EGettingRSASigner,
		ERSASigningMessage,
		EListingKeys,
		EDeletingKey,
		EKeyDeleted		//state after the key has been deleted
	} iState;
	
};


#endif /* SMFKEYSTOREMANAGER_H_ */
