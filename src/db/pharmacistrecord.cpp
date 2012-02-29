/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistrecord.h"

#include "alertinterface.h"
#include "../globals.h"

#include <QDebug>

const QString PharmacistRecord::last_Label("Last name");
const QString PharmacistRecord::last_Tooltip("Last name");

const QString PharmacistRecord::first_Label("First name");
const QString PharmacistRecord::first_Tooltip("First name");

const QString PharmacistRecord::initials_Label("Initials");
const QString PharmacistRecord::initials_Tooltip("Initials as they appear on a prescription label");

const QString PharmacistRecord::initials_Label_secondary("Filled by");
const QString PharmacistRecord::initials_Tooltip_secondary("Who filled the prescription");

const QString PharmacistRecord::active_Label("Status");
const QString PharmacistRecord::active_Tooltip("Active or inactive status");

PharmacistRecord::PharmacistRecord():
	id(SQL::Undefined_ID),
	active(true),
	exists(false)
{
}

int PharmacistRecord::getId()
{
	return id;
}

QString PharmacistRecord::getLast()
{
	return last;
}

QString PharmacistRecord::getFirst()
{
	return first;
}

QString PharmacistRecord::getInitials()
{
	return initials;
}

bool PharmacistRecord::getActive()
{
	return active;
}

bool PharmacistRecord::getExists()
{
	return exists;
}

void PharmacistRecord::setId(int new_id)
{
	id = new_id;
}

void PharmacistRecord::setLast(QString new_last)
{
	last = new_last;
}

void PharmacistRecord::setFirst(QString new_first)
{
	first = new_first;
}

void PharmacistRecord::setInitials(QString new_initials)
{
	initials = new_initials;
}

void PharmacistRecord::setActive(bool new_active)
{
	active = new_active;
}



/* SQL without C++:
UPDATE pharmacists
SET active = ?
WHERE id = ?;
*/
bool PharmacistRecord::toggleActive()
{
	QSqlQuery *model;
	AlertInterface alert;

	if (!exists) {		// Entry doesn't exist - do nothing
		return false;
	}

	model = new QSqlQuery;

	model->prepare("UPDATE pharmacists "
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
SELECT last, first, initials, active
FROM pharmacists
WHERE id = 'SOME_VAL';
*/
bool PharmacistRecord::retrieve(int newId)
{
	QSqlQuery *model;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQuery;
	model->prepare("SELECT last, first, initials, active "
				   "FROM pharmacists "
				   "WHERE id = ?;");
	model->bindValue(0, QVariant(newId));

	if (!alert.attemptQuery(model)) {
		delete model;
		return false;	// Query failed
	}

	model->next();
	id = newId;
	last = model->value(0).toString();
	first = model->value(1).toString();
	initials = model->value(2).toString();
	active = model->value(3).toBool();
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
INSERT INTO pharmacists (last, first, initials, active)
VALUES ('SOME_VAL', 'SOME_VAL', 'SOME_VAL', 'SOME_VAL');

UPDATE pharmacists
SET last = 'SOME_VAL', first = 'SOME_VAL', initials = 'SOME_VAL', active = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool PharmacistRecord::commitRecord()
{
	QSqlQuery *model;
	AlertInterface alert;

	model = new QSqlQuery;

	if (!exists) {	// Need to do an INSERT
		model->prepare("INSERT INTO pharmacists (last, first, initials, active) "
					   "VALUES (?, ?, ?, ?);");
		model->bindValue(0, SQL::prepNoMatching(last));
		model->bindValue(1, SQL::prepNoMatching(first));
		model->bindValue(2, SQL::prepNoMatching(initials));
		model->bindValue(3, QVariant(active));
	} else {		// Need to do an UPDATE
		model->prepare("UPDATE pharmacists "
					   "SET last = ?, first = ?, initials = ?, active = ? "
					   "WHERE id = ?;");
		model->bindValue(0, SQL::prepNoMatching(last));
		model->bindValue(1, SQL::prepNoMatching(first));
		model->bindValue(2, SQL::prepNoMatching(initials));
		model->bindValue(3, QVariant(active));
		model->bindValue(4, QVariant(id));
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

void PharmacistRecord::print()
{
	qDebug() << "id =" << id << ", last =" << last << ", first =" << first << ", initials =" << initials << ", active =" << active;
}
