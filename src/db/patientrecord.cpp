/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientrecord.h"

#include "alertinterface.h"
#include "../globals.h"

#include <QDebug>

PatientRecord::PatientRecord():
	id(SQL::Undefined_ID),
	allscripts_id(SQL::Undefined_ID),
	active(true),
	exists(false)
{
}

int PatientRecord::getId()
{
	return id;
}

int PatientRecord::getAllscripts_id()
{
	return allscripts_id;
}

QString PatientRecord::getLast()
{
	return last;
}

QString PatientRecord::getFirst()
{
	return first;
}

QDate PatientRecord::getDob()
{
	return dob;
}

bool PatientRecord::getActive()
{
	return active;
}

bool PatientRecord::getExists()
{
	return exists;
}

void PatientRecord::setId(int new_id)
{
	id = new_id;
}

void PatientRecord::setAllscripts_id(int new_allscripts_id)
{
	allscripts_id = new_allscripts_id;
}

void PatientRecord::setLast(QString new_last)
{
	last = new_last;
}

void PatientRecord::setFirst(QString new_first)
{
	first = new_first;
}

void PatientRecord::setDob(QDate new_dob)
{
	dob = new_dob;
}

void PatientRecord::setActive(bool new_active)
{
	active = new_active;
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
UPDATE patients
SET active = ?
WHERE id = ?;
*/
bool PatientRecord::toggleActive()
{
	QSqlQuery *model;
	AlertInterface alert;

	if (!exists) {		// Entry doesn't exist - do nothing
		return false;
	}

	model = new QSqlQuery;

	model->prepare("UPDATE patients "
				   "SET active = ? "
				   "WHERE id = ?;");
	model->bindValue(0, !active);
	model->bindValue(1, QVariant(id));

	if (!alert.attemptQuery(model)) {
		delete model;
		return false;
	}

	active = !active;

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

void PatientRecord::print()
{
	qDebug() << "id =" << id << ", allscripts_id =" << allscripts_id << ", last =" << last << ", first =" << first << ", dob =" << dob << ", active =" << active;
}
