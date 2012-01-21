/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQuery>

#include "prescriberrecord.h"

#include "globals.h"
#include "alertinterface.h"

PrescriberRecord::PrescriberRecord(QObject *parent):
	QObject(parent),
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

/* SQL without C++:
INSERT INTO prescribers (last, first, full_name, active)
VALUES ('SOME_VAL', 'SOME_VAL', 'SOME_VAL', 'SOME_VAL');

UPDATE prescribers
SET last = 'SOME_VAL', first = 'SOME_VAL', full_name = 'SOME_VAL', active = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool PrescriberRecord::commitRecord()
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	model = new QSqlQueryModel;

	if (exists) {
		query = QString("INSERT INTO prescribers (last, first, full_name, active) VALUES ('");
		query += SQL::cleanNoMatching(last) + QString("', '");
		query += SQL::cleanNoMatching(first) + QString("', '");
		query += SQL::cleanNoMatching(full_name) + QString("', '");
		if (active) {
			query += QString("1');");
		} else {
			query += QString("0');");
		}
	} else {
		query += QString("UPDATE prescribers SET last = '");
		query += SQL::cleanNoMatching(last) + QString("', first = '");
		query += SQL::cleanNoMatching(first) + QString("', full_name = '");
		query += SQL::cleanNoMatching(full_name) + QString("', active = '");
		if (active) {
			query += QString("1' WHERE id = '");
		} else {
			query += QString("0' WHERE id = '");
		}
		query += QString().setNum(id) + QString("';");
	}

	if (!alert.attemptQuery(model, &query)) {
		delete model;
		return false;
	}
	if (!exists) {	// Update the local record if this was an insert
		id = model->query().lastInsertId().toInt();
		exists = true;
	}

	delete model;
	return true;
}
