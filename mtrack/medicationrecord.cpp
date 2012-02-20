/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QSqlRecord>

#include "medicationrecord.h"

#include "globals.h"
#include "alertinterface.h"

#include <QDebug>


const QString MedicationRecord::name_Label("Medication");
const QString MedicationRecord::name_Tooltip("Name of the medication");

const QString MedicationRecord::generic_Label("Generic for");
const QString MedicationRecord::generic_Tooltip("Brand name of medication (empty for none)");

const QString MedicationRecord::manufacturer_Label("Manufacturer");
const QString MedicationRecord::manufacturer_Tooltip("Manufacturer");

const QString MedicationRecord::ndc_Label("NDC");
const QString MedicationRecord::ndc_Tooltip("National Drug Code");

const QString MedicationRecord::form_Label("Form");
const QString MedicationRecord::form_Tooltip("Medication form");

const QString MedicationRecord::strength_Label("Strength");
const QString MedicationRecord::strength_Tooltip("Strength");

const QString MedicationRecord::dispense_units_Label("Dispense units");
const QString MedicationRecord::dispense_units_Tooltip("Dispense units can be anything: bottles, capsules, etc.");

const QString MedicationRecord::unit_size_Label("Unit size (optional)");
const QString MedicationRecord::unit_size_Tooltip("Size of a single unit. IE: A \"75 ml\" bottle");

const QString MedicationRecord::instructions_Label("Instructions");
const QString MedicationRecord::instructions_Tooltip("Default instructions for the patient");

MedicationRecord::MedicationRecord(QObject *parent):
	QObject(parent),
	id(SQL::Undefined_ID),
	form(FORM_INT::Undefined),
	active(true),
	exists(false)
{
}

/*
SELECT name, generic, manufacturer, ndc, form, strength, dispense_units, unit_size, instructions, active
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
    query += QString("SELECT name, generic, manufacturer, ndc, form, strength, dispense_units, unit_size, instructions, active FROM drugs WHERE id = '");
	query += QString().setNum(newId) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		delete model;
		return false;	// Query failed
	}

	id = newId;
	name = model->record(0).value(0).toString();
	generic = model->record(0).value(1).toString();
	manufacturer = model->record(0).value(2).toString();
	ndc = model->record(0).value(3).toString();
	form = FORM_INT::strToInt(model->record(0).value(4).toString());
	strength = model->record(0).value(5).toString();
    dispense_units = model->record(0).value(6).toString();
    unit_size = model->record(0).value(7).toString();
    instructions = model->record(0).value(8).toString();
    active = model->record(0).value(9).toBool();
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
INSERT INTO drugs (name, generic, manufacturer, ndc, form, strength, dispense_units, unit_size, instructions, active)
VALUES ('SOME_VAL', ....);

UPDATE drugs
SET name = 'SOME_VAL', generic = 'SOME_VAL', manufacturer = 'SOME_VAL', ndc = 'SOME_VAL', form = 'SOME_VAL',
strength = 'SOME_VAL', dispense_units = 'SOME_VAL', unit_size = 'SOME_VAL',
instructions = 'SOME_VAL', active = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool MedicationRecord::commitRecord()
{
	QSqlQuery *model;
	AlertInterface alert;

	model = new QSqlQuery;

	model->prepare("INSERT INTO drugs (name, generic, manufacturer, ndc, form, strength, dispense_units, unit_size, instructions, active) "
				   "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");

	if (!exists) {	// Do an INSERT
		model->prepare("INSERT INTO drugs (name, generic, manufacturer, ndc, form, strength, dispense_units, unit_size, instructions, active) "
					   "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
		model->bindValue(0, SQL::prepNoMatching(name));
		model->bindValue(1, SQL::prepNoMatching(generic));
		model->bindValue(2, SQL::prepNoMatching(manufacturer));
		model->bindValue(3, SQL::prepNoMatching(ndc));
		model->bindValue(4, QVariant(FORM_STR::intToStr(form)));
		model->bindValue(5, SQL::prepNoMatching(strength));
		model->bindValue(6, SQL::prepNoMatching(dispense_units));
		model->bindValue(7, SQL::prepNoMatching(unit_size));
		model->bindValue(8, SQL::prepNoMatching(instructions));
		model->bindValue(9, QVariant(active));
	} else {	// Need to to an UPDATE instead
		model->prepare("UPDATE drugs "
					   "SET name = ?, generic = ?, manufacturer = ?, ndc = ?, form = ?, strength = ?, "
					   "dispense_units = ?, unit_size = ?, instructions = ?, active = ? "
					   "WHERE id = ?;");
		model->bindValue(0, SQL::prepNoMatching(name));
		model->bindValue(1, SQL::prepNoMatching(generic));
		model->bindValue(2, SQL::prepNoMatching(manufacturer));
		model->bindValue(3, SQL::prepNoMatching(ndc));
		model->bindValue(4, QVariant(FORM_STR::intToStr(form)));
		model->bindValue(5, SQL::prepNoMatching(strength));
		model->bindValue(6, SQL::prepNoMatching(dispense_units));
		model->bindValue(7, SQL::prepNoMatching(unit_size));
		model->bindValue(8, SQL::prepNoMatching(instructions));
		model->bindValue(9, QVariant(active));
		model->bindValue(10, QVariant(id));
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

void MedicationRecord::print()
{
	qDebug() << "id =" << id << ", name =" << name << ", generic =" << generic << ", manufacturer =" << manufacturer << ", ndc =" << ndc << ", form =" << FORM_STR::intToStr(form) << ", strength =" << strength << ", dispense_units =" << dispense_units << ", unit_size =" << unit_size << ", instructions =" << instructions << ", active =" << active;
}
