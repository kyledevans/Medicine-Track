/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>

#include "prescriptionrecord.h"

#include "globals.h"
#include "alertinterface.h"

PrescriptionRecord::PrescriptionRecord():
	id(SQL::Undefined_ID),
	patient_id(SQL::Undefined_ID),
	drug_id(SQL::Undefined_ID),
	shipment_id(SQL::Undefined_ID),
	prescriber_id(SQL::Undefined_ID),
	pharmacist_id(SQL::Undefined_ID)
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
