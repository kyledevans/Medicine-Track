/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQuery>

#include "pharmacistrecord.h"

#include "globals.h"
#include "alertinterface.h"

PharmacistRecord::PharmacistRecord():
	id(SQL::Undefined_ID),
	active(true),
	exists(false)
{
}

/* SQL without C++:
SELECT last, first, initials, active
FROM pharmacists
WHERE id = 'SOME_VAL';
*/
bool PharmacistRecord::retrieve(int newId)
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQueryModel;
	query += QString("SELECT last, first, initials, active FROM pharmacists WHERE id = '");
	query += QString().setNum(newId) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		return false;	// Query failed
	}

	id = newId;
	last = model->record(0).value(0).toString();
	first = model->record(0).value(1).toString();
	initials = model->record(0).value(2).toString();
	active = model->record(0).value(2).toBool();
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
INSERT INTO pharmacists (last, first, initials, active)
VALUES ('SOME_VAL', 'SOME_VAL', 'SOME_VAL', 'SOME_VAL');

UPDATE prescribers
SET last = 'SOME_VAL', first = 'SOME_VAL', initials = 'SOME_VAL', active = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool PharmacistRecord::commitRecord()
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	model = new QSqlQueryModel;

	if (!exists) {	// Need to do an INSERT
		query = QString("INSERT INTO pharmacists (last, first, initials, active) VALUES ('");
		query += last + QString("', '");
		query += first + QString("', '");
		query += initials + QString("', '");
		if (active) {
			query += QString("1');");
		} else {
			query += QString("0');");
		}
	} else {		// Need to do an UPDATE
		query = QString("UPDATE prescribers SET last = '");
		query += SQL::cleanNoMatching(last) + QString("', first = '");
		query += SQL::cleanNoMatching(first) + QString("', initials = '");
		query += SQL::cleanNoMatching(initials) + QString("', active = '");
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

	if (!exists) {
		id = model->query().lastInsertId().toInt();
		exists = true;
	}

	delete model;
	return true;
}
