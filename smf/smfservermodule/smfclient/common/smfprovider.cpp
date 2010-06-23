/**
 * Copyright (c) 2010 Sasken Communication Technologies Ltd.
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of the "Eclipse Public License v1.0" 
 * which accompanies  this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html"
 *
 * Initial Contributors:
 * Chandradeep Gandhi, Sasken Communication Technologies Ltd - Initial contribution
 *
 * Contributors:
 * Manasij Roy, Nalina Hariharan
 * 
 * Description:
 * Interface for a base service provider
 *
 */
#include "smfprovider.h"

 SmfProvider::SmfProvider()
  {
  
  }
 SmfProvider::~SmfProvider()
  {
  
  }
QString SmfProvider::serviceName() const
 {
	return m_serviceName;
 }
QImage SmfProvider::serviceIcon() const
 {
	return m_serviceIcon;
 }
QString SmfProvider::description() const
 {
	 return m_description;
 }
QUrl SmfProvider::serviceUrl() const
 {
	 return m_serviceUrl;
 }
QUrl SmfProvider::applicationUrl() const  
 {
	  return m_appUrl;
 }
QList<QString> SmfProvider::supportedInterfaces() const
 {
	return m_serviceTypes;
 }
/**
 * List of languages supported by this service provider
 * @return a QStringList of languages supported by this service 
 * provider in 2 letter ISO 639-1 format.
 */
QStringList SmfProvider::supportedLanguages() const
	{
	return m_supportedLanguages;
	}
void SmfProvider::setServiceName(QString& name)
	  {
	  m_serviceName = name;
	  }
void SmfProvider::setServiceIcon(QImage& image)
	  {
	  m_serviceIcon = image;
	  }
void SmfProvider::setDescription(QString& desc)
	  {
	  m_description = desc;
	  }
void SmfProvider::setServiceUrl(QUrl& url)
	  {
	  m_serviceUrl = url;
	  }
void SmfProvider::setApplicationUrl(QUrl& url)
	  {
	  m_appUrl = url;
	  }
void SmfProvider::setSupportedInterfaces( QStringList& types)
	 {
	 m_serviceTypes = types;
	 }
/**
 * Sets the list of languages supported by this service provider
 * @param lang A QStringList of languages supported by this service 
 * provider in 2 letter ISO 639-1 format.
 */
void SmfProvider::setSupportedLanguages( QStringList& lang )
	{
	m_supportedLanguages = lang;
	}

QDataStream &operator<<(QDataStream& out, const SmfProvider& base)
 	{
	out<<base.m_serviceName;
 	out<<base.m_serviceIcon;
 	out<<base.m_description;
 	out<<base.m_serviceUrl;
 	out<<base.m_appUrl;
 	return out; 
 	}
QDataStream &operator>>(QDataStream& in, SmfProvider& base)
 	{
 	in>>base.m_serviceName;
 	in>>base.m_serviceIcon;
 	in>>base.m_description;
 	in>>base.m_serviceUrl;
 	in>>base.m_appUrl;
 	return in; 
 	}
