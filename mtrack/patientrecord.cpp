/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQuery>

#include "patientrecord.h"

#include "alertinterface.h"
#include "globals.h"

PatientRecord::PatientRecord(QObject *parent):
	QObject(parent),
	id(SQL::Undefined_ID),
	allscripts_id(SQL::Undefined_ID),
	active(true),
	exists(false)
{
}

/* SQL without C++:
SELECT allscripts_id, last, first, dob, active
FROM patients
WHERE id = 'SOME_VAL';
*/
bool PatientRecord::retrieve(int newId)
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQueryModel;
	query += QString("SELECT allscripts_id, last, first, dob, active FROM patients WHERE id = '");
	query += QString().setNum(newId) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		return false;	// Query failed
	}

	id = newId;
	allscripts_id = model->record(0).value(0).toInt();
	last = model->record(0).value(1).toString();
	first = model->record(0).value(2).toString();
	dob = QDate().fromString(model->record(0).value(3).toString(), "yyyy-MM-dd");
	active = model->record(0).value(4).toBool();
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
INSERT INTO patients (allscripts_id, last, first, dob, active)
VALUES ('SOME_VAL', 'SOME_VAL', 'SOME_VAL', 'SOME_VAL', 'SOME_VAL');

UPDATE patients
SET allscripts_id = 'SOME_VAL', last = 'SOME_VAL', first = 'SOME_VAL', dob = 'SOME_VAL', active = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool PatientRecord::commitRecord()
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	model = new QSqlQueryModel;

	if (!exists) {
		query = QString("INSERT INTO patients (allscripts_id, last, first, dob, active) VALUES ('");
		query += QString().setNum(allscripts_id) + QString("', '");
		query += SQL::cleanNoMatching(last) + QString("', '");
		query += SQL::cleanNoMatching(first) + QString("', '");
		query += dob.toString("yyyy-MM-dd") + QString("', '");
		if (active) {
			query += QString("1');");
		} else {
			query += QString("0');");
		}
	} else {
		query += QString("UPDATE patients SET allscripts_id = '");
		query += QString().setNum(allscripts_id) + QString("', last = '");
		query += SQL::cleanNoMatching(last) + QString("', first = '");
		query += SQL::cleanNoMatching(first) + QString("', dob = '");
		query += dob.toString("yyyy-MM-dd") + QString("', active = '");
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
