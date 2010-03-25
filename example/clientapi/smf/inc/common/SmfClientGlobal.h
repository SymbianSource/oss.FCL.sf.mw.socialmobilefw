/**
* Copyright (c) 2010 Sasken Communication Technologies Ltd.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "{License}"
* which accompanies  this distribution, and is available
* at the URL "{LicenseUrl}".
*
* Initial Contributors:
* Mansij Roy, Sasken Communication Technologies Ltd - Initial contribution
*
* Contributors:
*
* Description:
* Interface spefication for sfm service provider
*
*/

#ifndef SMFCLIENTDEFS_H_
#define SMFCLIENTDEFS_H_

 #include <QtCore/QtGlobal>

 #ifdef SMFCLIENT_LIBRARY
 #  define SMFCLIENT_EXPORT Q_DECL_EXPORT
 #else
 #  define SMFCLIENT_EXPORT Q_DECL_IMPORT
 #endif
/**
 * To be designed later
 */
#define SMF_GETSERVICES(INTERFACE,INTERFACESTRING) ;
#endif /* SMFCLIENTDEFS_H_ */
