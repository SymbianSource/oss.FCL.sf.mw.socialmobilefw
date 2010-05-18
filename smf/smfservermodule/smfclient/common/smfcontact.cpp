/*
 * smfcontact.cpp
 *
 *  Created on: Apr 15, 2010
 *      Author: manasij
 */
#include "smfcontact.h"
#include <QFile>
#include <QTextStream>


	/**
	 * Constructor with default argument
	 * @param aParent The parent object
	 */
	SmfContact::SmfContact( QObject *aParent)//:QObject(aParent)
	{
		d = new SmfContactPrivate;
	}
	
	/**
	 * Copy Constructor
	 * @param aOther The reference object
	 */
	SmfContact::SmfContact( const SmfContact &aOther ): d (aOther.d)
		{
		
		}
	
	/**
	 * Destructor
	 */
	SmfContact::~SmfContact( )
		{
		
		}
	QStringList SmfContact::subTypes( ) const
			{
		
			}
	
	/**
	 * Method to convert an SmfContact to a QContact
	 * Changes made to the returned QContact will not be reflected 
	 * in its parent SmfContact object
	 * @param aContact QContact corresponding to SmfContact 
	 */
	void SmfContact::convert( QContact &aContact ) const
			{
		
			}
	
	/**
	 * Method to get the value of a sub field for this contact
	 * @param aSubType The sub field type 
	 * @return The value of the sub field subType
	 * @see subTypes()
	 */
	QVariant SmfContact::value( const QString& aSubType ) const
			{
			if(d->m_details.contains(aSubType))
				return d->m_details.value(aSubType);
			else
				return QString("Not found");
			}
	
	void SmfContact::setValue(const QString& aSubType,QVariant& value)
		{
		d->m_details.insert(aSubType,value);
		}
	void SmfContact::writeLog(QString log) const
		{
		QFile file("c:\\data\\SmfClientLogs.txt");
	    if (!file.open(QIODevice::Append | QIODevice::Text))
		         return;
	    QTextStream out(&file);
	    out << log << "\n";
	    file.close();

		}
	/**
	 * Method for Externalization. Writes the SmfContact object to 
	 * the stream and returns a reference to the stream.
	 * @param aDataStream Stream to be written
	 * @param aContact The SmfContact object to be externalized
	 * @return reference to the written stream
	 */
	//TODO:-Need to revisit

	QDataStream &operator<<( QDataStream &aDataStream, 
			const SmfContact& aContact )
		{
		aContact.writeLog("smfContact::operator<<");
		QVariant var = aContact.value("Name");
		QContactName cn = var.value<QContactName>();
		aDataStream<<cn;
		return aDataStream;
		
		}

	/**
	 * Method for Internalization. Reads a SmfContact object from 
	 * the stream and returns a reference to the stream.
	 * @param aDataStream Stream to be read
	 * @param aContact The SmfContact object to be internalized
	 * @return reference to the stream
	 */
	
	QDataStream &operator>>( QDataStream &aDataStream, 
			SmfContact& aContact )
		{
		aContact.writeLog("smfContact::operator>>");
		//explicitely adding fields for the classes that don't provide 
		//internalizatio/externalization :(
		QContactName name;
		//QString name;
		aDataStream>>name;
		QVariant var = QVariant::fromValue(name);
		aContact.setValue("Name",var);
		return aDataStream;
		}
	QDataStream &operator<<( QDataStream &aDataStream, 
			const QContactName &aContact )
		{
		//Qt mobility introduces API compatibility break
#ifdef OLDER_QT_MOBILITY
		aDataStream<<aContact.first();
		aDataStream<<aContact.last();
#else
		
		aDataStream<<aContact.firstName();
		aDataStream<<aContact.lastName();
#endif
		return aDataStream;
		}

	QDataStream &operator>>( QDataStream &aDataStream, 
			QContactName &aContact )
		{
		QString first;
		QString last;
		aDataStream>>first;
#ifdef OLDER_QT_MOBILITY
		aContact.setFirst(first);
#else
		aContact.setFirstName(first);
#endif

		aDataStream>>last;
#ifdef OLDER_QT_MOBILITY
		aContact.setLast(last);
#else
		aContact.setLastName(last);
#endif
		return aDataStream;
		}
