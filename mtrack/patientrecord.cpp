/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQuery>

#include "patientrecord.h"

#include "alertinterface.h"
#include "globals.h"

const QString PatientRecord::allscripts_id_Label("M.R. Number");
const QString PatientRecord::allscripts_id_Tooltip("Allscripts medical record number");

const QString PatientRecord::last_Label("Last name");
const QString PatientRecord::last_Tooltip("Last name");

const QString PatientRecord::first_Label("First name");
const QString PatientRecord::first_Tooltip("First name");

const QString PatientRecord::dob_Label("D.O.B.");
const QString PatientRecord::dob_Tooltip("Date of birth");

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
	QSqlQuery *model;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQuery;
	model->prepare("SELECT allscripts_id, last, first, dob, active "
				   "FROM patients "
				   "WHERE id = ?;");
	model->bindValue(0, QVariant(newId));

	if (!alert.attemptQuery(model)) {
		delete model;
		return false;	// Query failed
	}

	model->next();
	id = newId;
	allscripts_id = model->value(0).toInt();
	last = model->value(1).toString();
	first = model->value(2).toString();
	dob = model->value(3).toDate();
	active = model->value(4).toBool();
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
	QSqlQuery *model;
	AlertInterface alert;

	model = new QSqlQuery;

	if (!exists) {
		model->prepare("INSERT INTO patients (allscripts_id, last, first, dob, active) "
					   "VALUES (?, ?, ?, ?, ?);");
		model->bindValue(0, QVariant(allscripts_id));
		model->bindValue(1, SQL::prepNoMatching(last));
		model->bindValue(2, SQL::prepNoMatching(first));
		model->bindValue(3, QVariant(dob));
		model->bindValue(4, QVariant(active));
	} else {
		model->prepare("UPDATE patients "
					   "SET allscripts_id = ?, last = ?, first = ?, dob = ?, active = ? "
					   "WHERE id = ?;");
		model->bindValue(0, QVariant(allscripts_id));
		model->bindValue(1, SQL::prepNoMatching(last));
		model->bindValue(2, SQL::prepNoMatching(first));
		model->bindValue(3, QVariant(dob));
		model->bindValue(4, QVariant(active));
		model->bindValue(5, QVariant(id));
	}

	if (!alert.attemptQuery(model)) {
		delete model;
		return false;
	}

	if (!exists) {
		id = model->lastInsertId().toInt();
		exists = true;
	}

	delete model;
	return true;
}
