/*
 * SmfClientGlobal.h
 *
 *  Created on: Mar 22, 2010
 *      Author: manasij
 */

#ifndef SMFCLIENTGLOBAL_H_
#define SMFCLIENTGLOBAL_H_

 #include <QtCore/QtGlobal>

_LIT(KSmfServerName,"smfserver");
_LIT( KSmfServerSemaphoreName, "smfserverSemaphore" );
_LIT( KSmfServerFilename, "smfserver" );
//TUid KSmfServerUID3 = { 0xE37E0269 };

 #ifdef SMFCLIENT_LIB_EXPORT
 #  define SMFCLIENT_EXPORT Q_DECL_EXPORT
 #else
 #  define SMFCLIENT_EXPORT Q_DECL_IMPORT
 #endif

/**
 * To be designed later
 */
#define SMF_GETSERVICES(INTERFACE,INTERFACESTRING) ;
#endif /* SMFCLIENTDEFS_H_ */