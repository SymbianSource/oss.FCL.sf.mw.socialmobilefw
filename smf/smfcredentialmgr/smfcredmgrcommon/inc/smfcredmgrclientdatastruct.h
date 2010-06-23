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
 *	This header file gives different data classes to be used
 * by Credential Manager Client and Server for data transfer.	
 *
 */
#ifndef CSMFCREDMGRCLIENTDATASTRUCT_H_
#define CSMFCREDMGRCLIENTDATASTRUCT_H_

// System includes
#include <e32cmn.h>
#include <S32MEM.H>  
#include <s32strm.h> 

#include "smfcredmgrcommon.h"

/**
 * Maximum Length for AuthTokens
 */
const TInt KMaxAuthTokenLength = 256;

/**
 * Maximum Length for Plugin ID
 */
const TInt KMaxPluginIDLength = 256;

/**
 * Maximum Length for Registration Token
 */
const TInt KMaxRegistrationTokenLength = 50;

/**
 * Maximum Length for URLString
 */
const TInt KMaxURLStringLength = 256;

/**
 * Maximum Length for Authentication Application ID
 */
const TInt KMaxAuthIdLength = 256;

/**
 * Maximum Length for SignedMessage
 */
const TInt KMaxSignedMsgLength = 256;
/**
 * This class provide details and to serialize Authentication Key And Secret   
 */
class TSmfAuthToken
	{
public:
	/**
	 * constructor
	 */
	inline TSmfAuthToken() :
		iKey(NULL), iSecret(NULL)
		{
		}
	/**
	 * Method to externalize the member variables
	 * @param aStream The Write Stream to be filled.
	 */
	void ExternalizeL(RWriteStream& aStream);

	/**
	 * Method to internalize the member variables
	 * @param aStream the stream to read from
	 */
	void InternalizeL(RReadStream& aStream);
	
	/**
	 * buf to hold the key
	 */
	HBufC* iKey;
	/**
	 * buf to hold the secret
	 */
	HBufC* iSecret;
	};

/**
 * This class provide details and to serialize for the API fetching Authentication Parameters  
 */
class CSmfFetchAuthTokenSet : public CBase
	{
public:
	/**
	 * destructor
	 */
	~CSmfFetchAuthTokenSet()
		{
		iAuthTokenArray.Reset();
		delete iRegistrationToken;
		}

	/**
	 * Method to externalize the member variables
	 * @param aStream The Write Stream to be filled.
	 */
	void ExternalizeL(RWriteStream& aStream);

	/**
	 * Method to internalize the member variables
	 * @param aSource the source to read from
	 */
	void InternalizeL(const RBuf8& aSource);
	/**
	 * buf to hold registration token
	 */
	HBufC* iRegistrationToken;
	/**
	 * Time by which the Auth set will expire
	 */
	TUint32 iValidity;
	/**
	 * array in symbian to hold the key-value pair
	 */
	RArray<TSmfAuthToken> iAuthTokenArray;
	};

/**
 * This class provide details and to serialize of data to the API retreiving URL List 
 */
class CSmfURLListParams : public CBase
	{
public:
	/**
	 * destructor
	 */
	~CSmfURLListParams()
		{
		iURLList.ResetAndDestroy();
		delete iPluginID;
		}

	/**
	 * Method to externalize the member variables
	 * @param aStream The Write Stream to be filled.
	 */
	void ExternalizeL(RWriteStream& aStream);

	/**
	 * Method to internalize the member variables
	 * @param aSource the source to read from
	 */
	void InternalizeL(const TDesC8& aSource);
	
	/**
	 * array in symbian to hold url list
	 */
	RPointerArray<HBufC> iURLList;
	
	/**
	 * buf to hold the plugin id for which urls will be queried
	 */
	HBufC* iPluginID;
	};

/**
 * This class provide details and to serialize data to the API retreiving Authenticated PluginID List 
 */
class CSmfPluginIDListParams : public CBase
	{
public:
	/**
	 * destructor
	 */
	~CSmfPluginIDListParams()
		{
		iPluginList.ResetAndDestroy();
		delete iRegistrationToken;
		}

	/**
	 * Method to externalize the member variables
	 * @param aStream The Write Stream to be filled.
	 */
	void ExternalizeL(RWriteStream& aStream);

	/**
	 * Method to internalize the member variables
	 * @param aSource the source to read from
	 */
	void InternalizeL(const RBuf8& aSource);
	/**
	 * array in symbian to hold plugin list
	 */
	RPointerArray<HBufC> iPluginList;
	/**
	 * buf to hold registration token for which the list of plugins will be queried
	 */
	HBufC* iRegistrationToken;
	};

/**
 * Class to provide details of data to API storing Parameters during Authentication process
 */
class CSmfStoreAuthParams : public CBase
	{
public:
	/**
	 * destructor
	 */
	~CSmfStoreAuthParams()
		{
		iAuthTokenArray.Reset();
		iURLList.ResetAndDestroy();
		iPluginIDList.ResetAndDestroy();
		delete iRegistrationToken;
		delete iAuthAppID;
		}

	/**
	 * Method to externalize the member variables
	 * @param aStream The Write Stream to be filled.
	 */
	void ExternalizeL(RWriteStream& aStream);

	/**
	 * Method to internalize the member variables
	 * @param aSource the source to read from
	 */
	void InternalizeL(const RBuf8& aSource);
	/**
	 * array to hold TSmfAuthToken elements
	 */
	RArray<TSmfAuthToken> iAuthTokenArray;
	/**
	 * array to hold list of plugins
	 */
	RPointerArray<HBufC> iPluginIDList;
	/**
	 * array to hold URl list
	 */
	RPointerArray<HBufC> iURLList;
	/**
	 * buf to hold the registration token
	 */
	HBufC* iRegistrationToken;
	/**
	 * buf to hold the authentication application ID
	 */
	HBufC* iAuthAppID;
	/**
	 * flag to indicate an enabled pluginID 
	 */
	TBool pluginIDEnabled;
	/**
	 * Time by which the Auth set will expire
	 */
	TUint32 iValidity;
	};

/**
 * Class to provide details of data to the API changing/updating Authenticated 
 * plugin list
 */
class CSmfPluginIDUpdate : public CBase
	{
public:
	~CSmfPluginIDUpdate()
		{
		delete iNewPluginID;
		delete iOldPluginID;
		}

	/**
	 * Method to externalize the member variables
	 * @param aStream The Write Stream to be filled.
	 */
	void ExternalizeL(RWriteStream& aStream);

	/**
	 * Method to internalize the member variables
	 * @param aSource the source to read from
	 */
	void InternalizeL(const RBuf8& aSource);

	/**
	 * Id of new plugin
	 */
	HBufC* iNewPluginID;
	/**
	 * Id of the old plugin to be replaced
	 */
	HBufC* iOldPluginID;
	/**
	 * flag to indicate an enabled pluginID 
	 */
	TBool pluginIDEnabled;
	};

/**
 * Class to provide data to Signing API. 
 */
class CSmfSignParameters : public CBase
{
public:
	/**
	 * New Functions 
	 */
	static CSmfSignParameters* NewL( const TDesC8& aMessage, const TDesC8& aKey );
	static CSmfSignParameters* NewL( const TDesC8& aData );
	/**
	 * Destructor 
	 */
	~CSmfSignParameters();
	/**
	 * Method to externalize the member variables
	 * @param aStream The Write Stream to be filled.
	 */
	void ExternalizeL( RWriteStream& aStream ) const;
	/**
	 * Get Method for key. 
	 */
	const TDesC8& Key() const;
	/**
	 * Get Method for message. 
	 */
	const TDesC8& Message() const;
	
private:
	/**
	 * Constructor 
	 */
	CSmfSignParameters();
	/**
	 * Two Phase constructors 
	 */
	void ConstructL( const TDesC8& aMessage, const TDesC8& aKey );
	void ConstructL( const TDesC8& aData );
	
private:
	/**
	 * Message to sign 
	 */
	RBuf8 iMessage;
	/**
	 * Key for the signature 
	 */
	RBuf8 iKey;
};

/**
 * Class to provide RSA key details 
 */
class CSmfRsaKeyParameters : public CBase
	{
public:
	/**
	 * New methods 
	 */
	static CSmfRsaKeyParameters* NewL(
			const TDesC& aKeyName, const TTime& startDate,
			const TTime& endDate, const TDesC8& aKeyData );
	static CSmfRsaKeyParameters* NewL( const TDesC8& aData );
	
	/**
	 * Destructor 
	 */
	~CSmfRsaKeyParameters();
	/**
	 * Method to externalize the member variables
	 * @param aStream The Write Stream to be filled.
	 */
	void ExternalizeL( RWriteStream& aStream ) const;
	/**
	 * Get Method for keyname. 
	 */
	const TDesC& KeyName() const;
	/**
	 * Get Method for keydata. 
	 */
	const TDesC8& KeyData() const;
	/**
	 * Get Method for start date. 
	 */
	const TTime& StartDate() const;
	/**
	 * Get Method for end date. 
	 */
	const TTime& EndDate() const;
	
private:
	/**
	 * Two phase constructors 
	 */
	void ConstructL(
			const TDesC& aKeyName, const TTime& startDate,
			const TTime& endDate, const TDesC8& aKeyData );
	void ConstructL( const TDesC8& aData );
			
private:
	/**
	 * Name of the key pair 
	 */
	RBuf iKeyName;
	/**
	 * Data of the key pair 
	 */
	RBuf8 iKeyData;
	/**
	 * Start date of validity 
	 */
	TTime iStartDate;
	/**
	 * End date of validity 
	 */
	TTime iEndDate;
	};

#endif /* CSMFCREDMGRCLIENTDATASTRUCT_H_ */
