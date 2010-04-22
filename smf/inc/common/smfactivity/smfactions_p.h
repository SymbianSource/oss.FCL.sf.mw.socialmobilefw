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
 * Description:
 * Contains private implementation of the smfactions
 */

#ifndef SMFACTIONS_P_H_
#define SMFACTIONS_P_H_

#include <QDateTime>
#include <QSharedData>
#include <QList>
#include <QImage>
#include <QDateTime>
#include <QStringList>
#include <QUrl>
#include <qdatastream.h>
#include <QSharedData>
#include <QMetaType>

#include "smfactions.h"

class SmfActivityObjectPrivate: public QSharedData
	{
public:	
	SmfActivityObjectPrivate();
	SmfActivityObjectPrivate(const SmfActivityObjectPrivate& aOther);
	~SmfActivityObjectPrivate();
	
	/**
	 * Data members
	 */
	QString m_id;
	QImage m_thumbnail;
	QString m_caption;
	SmfActivityObjectType m_type;
	QVariant m_objData;
	QString m_link;
	QDateTime m_time;
	QString m_content;
	};

class SmfActivityEntryPrivate: public QSharedData
	{
public:
	SmfActivityEntryPrivate();
	SmfActivityEntryPrivate(const SmfActivityEntryPrivate& aOther);
	~SmfActivityEntryPrivate();
	
	QString m_id;
	SmfPost m_title;
	SmfPost m_details;
	SmfContact m_author;
	SmfActivityVerb m_actionName;
	QList<SmfActivityObject> m_activities;
	SmfActivityObject m_targetObj;
	};

#endif /* SMFACTIONS_P_H_ */
