/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>

#include "prescriberrecord.h"

#include "globals.h"
#include "alertinterface.h"

PrescriberRecord::PrescriberRecord():
	id(SQL::Undefined_ID),
	active(true),
	exists(false)
{
}

/* SQL without C++:
SELECT last, first, full_name, active
FROM prescribers
WHERE id = 'SOME_VAL';
*/
bool PrescriberRecord::retrieve(int newId)
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQueryModel;
	query += QString("SELECT last, first, full_name, active FROM prescribers WHERE id = '");
	query += QString().setNum(newId) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		return false;	// Query failed
	}

	id = newId;
	last = model->record(0).value(0).toString();
	first = model->record(0).value(1).toString();
	full_name = model->record(0).value(2).toString();
	active = model->record(0).value(3).toBool();
	exists = true;

	delete model;
	return true;
}
