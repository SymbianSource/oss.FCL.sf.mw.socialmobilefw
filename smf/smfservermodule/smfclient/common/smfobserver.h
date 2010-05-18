
/*
 * SmfObserver.h
 *
 *  Created on: Mar 17, 2010
 *      Author: manasij
 */

#ifndef SMFOBSERVER_H_
#define SMFOBSERVER_H_
class QByteArray;
#include "smfglobal.h"
#include <QObject>
class smfObserver : public QObject
	{
public:
	virtual void resultsAvailable(QByteArray result,SmfRequestTypeID opcode, SmfError error)= 0;
	};

#endif
