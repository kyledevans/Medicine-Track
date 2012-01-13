/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>

#include "patientrecord.h"

#include "alertinterface.h"
#include "globals.h"

PatientRecord::PatientRecord():
	id(SQL::Undefined_ID),
	allscripts_id(SQL::Undefined_ID),
	exists(false)
{
}

/* SQL without C++:
SELECT allscripts_id, last, first, dob
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
	query += QString("SELECT allscripts_id, last, first, dob FROM patients WHERE id = '");
	query += QString().setNum(newId) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		return false;	// Query failed
	}

	id = newId;
	allscripts_id = model->record(0).value(0).toInt();
	last = model->record(0).value(1).toString();
	first = model->record(0).value(2).toString();
	dob = QDate().fromString(model->record(0).value(3).toString(), "yyyy-MM-dd");
	exists = true;

	delete model;
	return true;
}
