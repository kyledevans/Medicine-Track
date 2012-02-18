/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlDatabase>

#include "prescriptionrecord.h"

#include "globals.h"
#include "alertinterface.h"

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

PrescriptionRecord::PrescriptionRecord(QObject *parent):
	QObject(parent),
	id(SQL::Undefined_ID),
	patient_id(SQL::Undefined_ID),
	drug_id(SQL::Undefined_ID),
	shipment_id(SQL::Undefined_ID),
	prescriber_id(SQL::Undefined_ID),
	pharmacist_id(SQL::Undefined_ID),
	amount(1),
	exists(false)
{
}

/* SQL without C++:
SELECT patient_id, drug_id, shipment_id, prescriber_id, pharmacist_id, amount, dose_size, written, filled, instructions, comments, active
FROM prescriptions
WHERE id = 'SOME_VAL';
*/
bool PrescriptionRecord::retrieve(int newId)
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQueryModel;
	query += QString("SELECT patient_id, drug_id, shipment_id, prescriber_id, pharmacist_id, amount, dose_size, written, filled, instructions, comments, active FROM prescriptions WHERE id = '");
	query += QString().setNum(newId) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		return false;	// Query failed
	}

	id = newId;
	patient_id = model->record(0).value(0).toInt();
	drug_id = model->record(0).value(1).toInt();
	shipment_id = model->record(0).value(2).toInt();
	prescriber_id = model->record(0).value(3).toInt();
	pharmacist_id = model->record(0).value(4).toInt();
	amount = model->record(0).value(5).toInt();
	dose_size = model->record(0).value(6).toString();
	written = QDate().fromString(model->record(0).value(7).toString(), "yyyy-MM-dd");
	filled = QDate().fromString(model->record(0).value(8).toString(), "yyyy-MM-dd");
	instructions = model->record(0).value(9).toString();
	comments = model->record(0).value(10).toString();
	active = model->record(0).value(11).toBool();
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
START TRANSACTION;
INSERT INTO prescriptions (patient_id, drug_id, shipment_id, prescriber_id,
pharmacist_id, amount, dose_size, written, filled, instructions)
VALUES (...);

UPDATE shipments
SET shipments.product_left = (shipments.product_left - 'SOME_VAL')
WHERE shipments.id = 'SOME_VAL';
COMMIT;

UPDATE prescriptions
SET patient_id = 'SOME_VAL', drug_id = 'SOME_VAL', shipment_id = 'SOME_VAL',
prescriber_id = 'SOME_VAL', pharmacist_id = 'SOME_VAL', amount = 'SOME_VAL',
dose_size = 'SOME_VAL', written = 'SOME_VAL', filled = 'SOME_VAL', instructions = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool PrescriptionRecord::commitRecord()
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;
	QSqlDatabase db;

	model = new QSqlQueryModel();

	if (!exists) {
		db = QSqlDatabase::database();	// Get the default DB
		db.transaction();
		query = QString("INSERT INTO prescriptions (patient_id, drug_id, shipment_id, prescriber_id, pharmacist_id, amount, dose_size, written, filled, instructions) VALUES ('");
		query += QString().setNum(patient_id) + QString("', '");
		query += QString().setNum(drug_id) + QString("', '");
		query += QString().setNum(shipment_id) + QString("', '");
		query += QString().setNum(prescriber_id) + QString("', '");
		query += QString().setNum(pharmacist_id) + QString("', '");
		query += QString().setNum(amount) + QString("', '");
		query += SQL::cleanNoMatching(dose_size) + QString("', '");
		query += written.toString("yyyy-MM-dd") + QString("', '");
		query += filled.toString("yyyy-MM-dd") + QString("', '");
		query += SQL::cleanNoMatching(instructions) + QString("');");

		if (!alert.attemptQuery(model, &query)) {
			delete model;
			db.rollback();
			return false;
		}

		// Update the inventory
		query = QString("UPDATE shipments SET shipments.product_left = (shipments.product_left - '");
		query += QString().setNum(amount) + QString("') WHERE shipments.id = '");
		query += QString().setNum(shipment_id) + QString("';");

		if (!alert.attemptQuery(model, &query)) {
			db.rollback();
			delete model;
			return false;
		} else {
			db.commit();
			delete model;
			return true;
		}
	}

	if (!exists) {
		id = model->query().lastInsertId().toInt();
		exists = true;
	}

	delete model;
	return true;
}

void PrescriptionRecord::print()
{
	qDebug() << "id =" << id << "patient_id =" << patient_id << "drug_id =" << drug_id << "shipment_id =" << shipment_id << "prescriber_id =" << prescriber_id << "pharmacist_id =" << pharmacist_id << "amount =" << amount << "dose_size =" << dose_size << "written =" << written.toString("yyyy-MM-dd") << "filled =" << filled.toString("yyyy-MM-dd") << "instructions =" << instructions;
}
