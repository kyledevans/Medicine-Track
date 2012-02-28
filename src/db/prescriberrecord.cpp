/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberrecord.h"

#include "alertinterface.h"
#include "../globals.h"

#include <QDebug>

const QString PrescriberRecord::last_Label("Last name");
const QString PrescriberRecord::last_Tooltip("Last name");

const QString PrescriberRecord::first_Label("First name");
const QString PrescriberRecord::first_Tooltip("First name");

const QString PrescriberRecord::full_name_Label("Full name");
const QString PrescriberRecord::full_name_Tooltip("Full name as it appears on a prescription label");

const QString PrescriberRecord::full_name_Label_secondary("Written by");
const QString PrescriberRecord::full_name_Tooltip_secondary("Who wrote the prescription");

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
	QSqlQuery *model;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQuery;
	model->prepare("SELECT last, first, full_name, active "
				   "FROM prescribers "
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
	full_name = model->value(2).toString();
	active = model->value(3).toBool();
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
UPDATE prescribers
SET active = ?
WHERE id = ?;
*/
bool PrescriberRecord::toggleActive()
{
	QSqlQuery *model;
	AlertInterface alert;

	if (!exists) {		// Entry doesn't exist - do nothing
		return false;
	}

	model = new QSqlQuery;

	model->prepare("UPDATE prescribers "
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
INSERT INTO prescribers (last, first, full_name, active)
VALUES ('SOME_VAL', 'SOME_VAL', 'SOME_VAL', 'SOME_VAL');

UPDATE prescribers
SET last = 'SOME_VAL', first = 'SOME_VAL', full_name = 'SOME_VAL', active = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool PrescriberRecord::commitRecord()
{
	QSqlQuery *model;
	AlertInterface alert;

	model = new QSqlQuery;

	if (!exists) {
		model->prepare("INSERT INTO prescribers (last, first, full_name, active) "
					   "VALUES (?, ?, ?, ?);");
		model->bindValue(0, SQL::prepNoMatching(last));
		model->bindValue(1, SQL::prepNoMatching(first));
		model->bindValue(2, SQL::prepNoMatching(full_name));
		model->bindValue(3, QVariant(active));
	} else {
		model->prepare("UPDATE prescribers "
					   "SET last = ?, first = ?, full_name = ?, active = ? "
					   "WHERE id = ?;");
		model->bindValue(0, SQL::prepNoMatching(last));
		model->bindValue(1, SQL::prepNoMatching(first));
		model->bindValue(2, SQL::prepNoMatching(full_name));
		model->bindValue(3, QVariant(active));
		model->bindValue(4, QVariant(id));
	}

	if (!alert.attemptQuery(model)) {
		delete model;
		return false;
	}
	if (!exists) {	// Update the local record if this was an insert
		id = model->lastInsertId().toInt();
		exists = true;
	}

	delete model;
	return true;
}

void PrescriberRecord::print()
{
	qDebug() << "id =" << id << ", last =" << last << ", first =" << first << ", full_name =" << full_name << ", active =" << active;
}
