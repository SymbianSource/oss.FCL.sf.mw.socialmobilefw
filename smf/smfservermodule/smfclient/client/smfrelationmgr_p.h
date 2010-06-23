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
 *
 */
#include "smfprovider.h"
#include "SmfClientGlobal.h"
#include "smfGlobal.h"
#include "SmfObserver.h"

#include <QStringList>
#include "smfrelationmgr.h"
class SmfProvider;
#ifdef Q_OS_SYMBIAN
class CSmfClientSymbian;
#else
class SmfClientQt;
#endif
class SmfRelationMgrPrivate
	{
public:
	SmfRelationMgrPrivate(SmfRelationMgr* relnMngr);
	~SmfRelationMgrPrivate();
	//create a relation with first contact (id optional)
	SmfRelationId create(SmfProvider *provider=NULL, SmfContact *contact=NULL);
	/** assign contact to a relation */
	SmfError associate( const SmfRelationId& relation,
						const SmfContact* contact,
						SmfProvider* provider);

	/** remove contact from a relation */
	SmfError remove(SmfRelationId& relation,
					const SmfContact* conact);

	/** returns first relation item in the relation when exists, NULL otherwise */
	SmfRelationItem searchById(const SmfRelationId& relation);

	/** returns relation Id for a given contacts if exists, NULL otherwise */
	SmfRelationId searchByContact( SmfContact contact);

	/** returns number of contacts in a relation*/
	uint count(SmfRelationId relation);

	/** contacts and their provider */
	SmfRelationItem get(SmfRelationId& relation, quint32 index);

	/** list of contacts and their provider */
	QList<SmfRelationItem> getAll(SmfRelationId& relation);

	/** list of all relations */
	QList<SmfRelationId> getAllRelations();

	/** delete a particular relation*/
	SmfError Delete(SmfRelationId& relation);
private:
#ifdef Q_OS_SYMBIAN
  CSmfClientSymbian* m_SmfClientPrivate;
  friend class CSmfClientSymbian;
  int m_xtraInfoFlag;
  int m_pageInfoFlag;
#else
  SmfClientQt* m_SmfClientPrivate;
  friend class SmfClientQt;
#endif
  QByteArray m_dataForDSM;
  QByteArray m_dataFromDSM;
	};
