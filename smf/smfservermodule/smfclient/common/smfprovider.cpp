/*
 * smfprovider.cpp
 *
 *  Created on: Apr 15, 2010
 *      Author: manasij
 */
#include "smfprovider.h"

  SmfProvider::SmfProvider(QObject* parent)//:QObject(parent)
	  {
	  
	  }
  SmfProvider::~SmfProvider()
	  {
	  
	  }

  /**
   * Localizable name of the service 
   * @return service name
   */
  QString SmfProvider::serviceName() const
  {
	  return m_serviceName;
  }
  
  /**
   * Logo of the service
   * @return logo image of the service
   */
  QImage SmfProvider::serviceIcon() const
		  {
			  return m_serviceIcon;
		  }
  
  /**
   * Readable service description
   * @return service description
   */
  QString SmfProvider::description() const
		  {
	  return m_description;
		  }
  
  /*
   * Website of the service
   */
  QUrl SmfProvider::serviceUrl() const
		  {
	  return m_serviceUrl;
		  }
  
  /**
   * URL of the application providing this service
   */
  QUrl SmfProvider::applicationUrl() const  //
		  {
	  return m_appUrl;
		  }
  
  /**
   * service types - list of interfaces that this provider support
   */
  QList<QString> SmfProvider::serviceTypes() const
		  {
	  return m_serviceTypes;
		  }
  
  /////////////////////////////////////////set APIs
  /**
   * Localizable name of the service 
   * 
   */
  void SmfProvider::serviceName(QString& name)
	  {
	  m_serviceName = name;
	  }
  
  /**
   * Logo of the service
   * 
   */
  void SmfProvider::serviceIcon(QImage& image)
	  {
	  m_serviceIcon = image;
	  }
  
  /**
   * Readable service description
   * 
   */
  void SmfProvider::description(QString& desc)
	  {
	  m_description = desc;
	  }
  
  /*
   * Website of the service
   */
  void SmfProvider::serviceUrl(QUrl& url)
	  {
	  m_serviceUrl = url;
	  }
  
  /**
   * URL of the application providing this service
   */
  void SmfProvider::applicationUrl(QUrl& url)
	  {
	  m_appUrl = url;
	  }
  
  /**
   * service types - list of interfaces that this provider support
   */
 void SmfProvider::serviceTypes( QStringList& types)
	 {
	 m_serviceTypes = types;
	 }
 /**
  * Externalization
  */
 QDataStream &operator<<(QDataStream& out, const SmfProvider& base)
 	{
// 	base.writeLog("operator<<");
// 	base.writeLog("base info:-");
// 	base.writeLog(base.m_serviceName);
 //	base.writeLog(base.m_serviceIcon);
// 	base.writeLog(base.m_description);
// 	base.writeLog(base.m_serviceUrl);
// 	base.writeLog(base.m_appUrl);
// 	
 	
 	out<<base.m_serviceName;
 	out<<base.m_serviceIcon;
 	out<<base.m_description;
 	out<<base.m_serviceUrl;
 	out<<base.m_appUrl;
 	return out; 
 	}

 /**
  * Internalization
  */
 QDataStream &operator>>(QDataStream& in, SmfProvider& base)
 	{
// 	base.writeLog("operator>>");

 	
 	in>>base.m_serviceName;
 	in>>base.m_serviceIcon;
 	in>>base.m_description;
 	in>>base.m_serviceUrl;
 	in>>base.m_appUrl;
// 	base.writeLog("After writing,base info:-");
// 	base.writeLog(base.m_serviceName);
// //	base.writeLog(base.m_serviceIcon);
// 	base.writeLog(base.m_description);
// 	base.writeLog(base.m_serviceUrl);
// 	base.writeLog(base.m_appUrl);
 	return in; 
 	}
