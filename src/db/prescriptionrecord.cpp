/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlDatabase>

#include "prescriptionrecord.h"

#include "globals.h"
#include "db/alertinterface.h"

#include <QDebug>

const QString PrescriptionRecord::amount_Label("Amount");
const QString PrescriptionRecord::amount_Tooltip("How much is dispensed");

const QString PrescriptionRecord::dose_size_Label("Dose size");
const QString PrescriptionRecord::dose_size_Tooltip("Size of a single dose");

const QString PrescriptionRecord::written_Label("Written");
const QString PrescriptionRecord::written_Tooltip("Date the prescription was written");

const QString PrescriptionRecord::filled_Label("Filled");
const QString PrescriptionRecord::filled_Tooltip("Date the prescription was filled");

const QString PrescriptionRecord::instructions_Label("Instructions");
const QString PrescriptionRecord::instructions_Tooltip("Medication instructions for the patient");

PrescriptionRecord::PrescriptionRecord():
	id(SQL::Undefined_ID),
	patient_id(SQL::Undefined_ID),
	drug_id(SQL::Undefined_ID),
	shipment_id(SQL::Undefined_ID),
	prescriber_id(SQL::Undefined_ID),
	pharmacist_id(SQL::Undefined_ID),
	amount(1),
	active(true),
	exists(false)
{
}

/* SQL without C++:
START TRANSACTION;

SELECT amount, shipment_id
FROM prescriptions
WHERE id = ?;

UPDATE prescriptions
SET active = ?
WHERE id = ?;

UPDATE shipments
SET product_left = (product_left + ?)
WHERE id = ?;

COMMIT;
*/
bool PrescriptionRecord::invalidate()
{
	QSqlDatabase db;
	QSqlQuery *model;
	AlertInterface alert;
	int amount_temp;
	int ship_id_temp;

	if (!exists) {		// Entry doesn't exist - do nothing
		return false;
	}

	if (!active) {		// Entry is already inactive
		return false;
	}

	model = new QSqlQuery;

	db = QSqlDatabase::database();	// Get the default DB
	db.transaction();

	// Get the most up to date prescription amount
	model->prepare("SELECT amount, shipment_id "
				   "FROM prescriptions "
				   "WHERE id = ?;");
	model->bindValue(0, QVariant(id));

	if (!alert.attemptQuery(model)) {
		db.rollback();
		delete model;
		return false;
	}

	model->next();
	amount_temp = model->value(0).toInt();
	ship_id_temp = model->value(1).toInt();

	// Deactivate the prescription
	model->prepare("UPDATE prescriptions "
				   "SET active = 0 "
				   "WHERE id = ?;");
	model->bindValue(0, QVariant(id));

	if (!alert.attemptQuery(model)) {
		db.rollback();
		delete model;
		return false;
	}

	// Add the inventory back
	model->prepare("UPDATE shipments "
				   "SET product_left = (product_left + ?) "
				   "WHERE id = ?;");
	model->bindValue(0, QVariant(amount_temp));
	model->bindValue(1, QVariant(ship_id_temp));

	if (!alert.attemptQuery(model)) {
		db.rollback();
		delete model;
		return false;
	}

	db.commit();
	qDebug() << "here";

	active = !active;

	delete model;
	return true;
}

/* SQL without C++:
SELECT patient_id, drug_id, shipment_id, prescriber_id, pharmacist_id, amount,
dose_size, written, filled, instructions, comments, active
FROM prescriptions
WHERE id = 'SOME_VAL';
*/
bool PrescriptionRecord::retrieve(int newId)
{
	QSqlQuery *model;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQuery;
	model->prepare("SELECT patient_id, drug_id, shipment_id, prescriber_id, pharmacist_id, amount, "
				   "dose_size, written, filled, instructions, comments, active "
				   "FROM prescriptions "
				   "WHERE id = ?;");
	model->bindValue(0, QVariant(newId));

	if (!alert.attemptQuery(model)) {
		delete model;
		return false;	// Query failed
	}

	model->next();
	id = newId;
	patient_id = model->value(0).toInt();
	drug_id = model->value(1).toInt();
	shipment_id = model->value(2).toInt();
	prescriber_id = model->value(3).toInt();
	pharmacist_id = model->value(4).toInt();
	amount = model->value(5).toInt();
	dose_size = model->value(6).toString();
	written = model->value(7).toDate();
	filled = model->value(8).toDate();
	instructions = model->value(9).toString();
	comments = model->value(10).toString();
	active = model->value(11).toBool();
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
START TRANSACTION;
INSERT INTO prescriptions (patient_id, drug_id, shipment_id, prescriber_id,
pharmacist_id, amount, dose_size, written, filled, instructions, active)
VALUES (...);

UPDATE shipments
SET shipments.product_left = (shipments.product_left - 'SOME_VAL')
WHERE shipments.id = 'SOME_VAL';
COMMIT;
*/
bool PrescriptionRecord::commitRecord()
{
	QSqlQuery *model;
	AlertInterface alert;
	QSqlDatabase db;

	model = new QSqlQuery;

	db = QSqlDatabase::database();	// Get the default DB
	db.transaction();
	model->prepare("INSERT INTO prescriptions (patient_id, drug_id, shipment_id, prescriber_id, "
				   "pharmacist_id, amount, dose_size, written, filled, instructions, active) "
				   "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
	model->bindValue(0, QVariant(patient_id));
	model->bindValue(1, QVariant(drug_id));
	model->bindValue(2, QVariant(shipment_id));
	model->bindValue(3, QVariant(prescriber_id));
	model->bindValue(4, QVariant(pharmacist_id));
	model->bindValue(5, QVariant(amount));
	model->bindValue(6, SQL::prepNoMatching(dose_size));
	model->bindValue(7, QVariant(written));
	model->bindValue(8, QVariant(filled));
	model->bindValue(9, SQL::prepNoMatching(instructions));
	model->bindValue(10, QVariant(active));

	if (!alert.attemptQuery(model)) {
		db.rollback();
		delete model;
		return false;
	}

	// Update the inventory
	model->prepare("UPDATE shipments "
				   "SET shipments.product_left = (shipments.product_left - ?) "
				   "WHERE shipments.id = ?;");
	model->bindValue(0, QVariant(amount));
	model->bindValue(1, QVariant(shipment_id));

	if (!alert.attemptQuery(model)) {
		db.rollback();
		delete model;
		return false;
	} else {
		db.commit();
		delete model;
		return true;
	}

	if (!exists) {
		id = model->lastInsertId().toInt();
		exists = true;
	}

	delete model;
	return true;
}

void PrescriptionRecord::print()
{
	qDebug() << "id =" << id << "patient_id =" << patient_id << "drug_id =" << drug_id << "shipment_id =" << shipment_id << "prescriber_id =" << prescriber_id << "pharmacist_id =" << pharmacist_id << "amount =" << amount << "dose_size =" << dose_size << "written =" << written.toString("yyyy-MM-dd") << "filled =" << filled.toString("yyyy-MM-dd") << "instructions =" << instructions;
}
