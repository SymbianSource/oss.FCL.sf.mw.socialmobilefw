/**
* Copyright (c) 2010 Sasken Communication Technologies Ltd.
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the "{License}"
* which accompanies  this distribution, and is available
* at the URL "{LicenseUrl}".
*
* Initial Contributors:
* Chandradeep Gandhi, Sasken Communication Technologies Ltd - Initial contribution
*
* Contributors:
*
* Description:
* Interface spefication for sfm service provider
*
*/

#ifndef SMFPROVIDER_H
#define SMFPROVIDER_H

class SmfContact;

/**
 * Interface for a base service provider. Other service provider classes o derive from
 * this base class so that each has access to service provider information
 *
 * All of the functionality described here should be implemented by a service
 * specific plug-in object.
 */
class SmfProvider : public QObject
{
  Q_OBJECT;

public:
  // Seeing as this is a plug-in implementation, these will realistically
  // be generated by SMF factory of some kind
  SmfProvider(QObject* parent = 0);
  ~SmfProvider();

public:
  virtual QString serviceName() = 0; // licalizable name of the service
  virtual QImage serviceIcon() = 0; // Logo of the service
  virtual QString description() = 0; // readable service description
  virtual QUrl serviceUrl() = 0; // website of the service
  virtual QUrl applicationUrl() = 0; // application providing this service
  virtual SmfContact selfContact()=0;//provides profile of the accound holder

slots:
  //None at the moment

signals:
  //None at the moment
};

#endif // SMFPROVIDER_H
