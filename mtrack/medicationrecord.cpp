/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQuery>

#include "medicationrecord.h"

#include "globals.h"
#include "alertinterface.h"

#include <QDebug>

MedicationRecord::MedicationRecord():
	id(SQL::Undefined_ID),
	form(FORM_INT::Undefined),
	active(true),
	exists(false)
{
}

/*
SELECT name, generic, manufacturer, ndc, form, strength, amount, instructions, active
FROM drugs
WHERE id = 'SOME_VAR';
*/
bool MedicationRecord::retrieve(int newId)
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQueryModel;
	query += QString("SELECT name, generic, manufacturer, ndc, form, strength, amount, instructions, active FROM drugs WHERE id = '");
	query += QString().setNum(newId) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		return false;	// Query failed
	}

	id = newId;
	name = model->record(0).value(0).toString();
	generic = model->record(0).value(1).toString();
	manufacturer = model->record(0).value(2).toString();
	ndc = model->record(0).value(3).toString();
	form = FORM_INT::strToInt(model->record(0).value(4).toString());
	strength = model->record(0).value(5).toString();
	amount = model->record(0).value(6).toString();
	instructions = model->record(0).value(7).toString();
	active = model->record(0).value(8).toBool();
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
INSERT INTO drugs (name, generic, manufacturer, ndc, form, strength, amount, instructions, active)
VALUES ('SOME_VAL', ....);

UPDATE drugs
SET name = 'SOME_VAL', generic = 'SOME_VAL', manufacturer = 'SOME_VAL', ndc = 'SOME_VAL', form = 'SOME_VAL',
strength = 'SOME_VAL', amount = 'SOME_VAL', instructions = 'SOME_VAL', active = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool MedicationRecord::commitRecord()
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	model = new QSqlQueryModel;

	if (!exists) {	// Do an INSERT
		query = QString("INSERT INTO drugs (name, generic, manufacturer, ndc, form, strength, amount, instructions, active) VALUES ('");
		query += SQL::cleanNoMatching(name) + QString("', '");
		query += SQL::cleanNoMatching(generic) + QString("', '");
		query += SQL::cleanNoMatching(manufacturer) + QString("', '");
		query += SQL::cleanNoMatching(ndc) + QString("', '");
		query += FORM_STR::intToStr(form) + QString("', '");
		query += SQL::cleanNoMatching(strength) + QString("', '");
		query += SQL::cleanNoMatching(amount) + QString("', '");
		query += SQL::cleanNoMatching(instructions) + QString("', '");
		if (active) {
			query += QString("1');");
		} else {
			query += QString("0');");
		}
	} else {	// Need to to an UPDATE instead
		query = QString("UPDATE drugs SET name = '");
		query += SQL::cleanNoMatching(name) + QString("', generic = '");
		query += SQL::cleanNoMatching(generic) + QString("', manufacturer = '");
		query += SQL::cleanNoMatching(manufacturer) + QString("', ndc = '");
		query += SQL::cleanNoMatching(ndc) + QString("', form = '");
		query += FORM_STR::intToStr(form) + QString("', strength = '");
		query += SQL::cleanNoMatching(strength) + QString("', amount = '");
		query += SQL::cleanNoMatching(amount) + QString("', instructions = '");
		query += SQL::cleanNoMatching(instructions) + QString("', active = '");
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

void MedicationRecord::print()
{
	qDebug() << "id =" << id << ", name =" << name << ", generic =" << generic << ", manufacturer =" << manufacturer << ", ndc =" << ndc << ", form =" << FORM_STR::intToStr(form) << ", strength =" << strength << ", amount =" << amount << ", instructions =" << instructions << ", active =" << active;
}
