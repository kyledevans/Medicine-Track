/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlQuery>

#include "shipmentrecord.h"

#include "globals.h"
#include "alertinterface.h"

#include <QDebug>

const QString ShipmentRecord::expiration_Label("Expiration");
const QString ShipmentRecord::expiration_Tooltip("Expiration date");

const QString ShipmentRecord::lot_Label("Lot #");
const QString ShipmentRecord::lot_Tooltip("Lot number");

const QString ShipmentRecord::product_count_Label("Inventory received");
const QString ShipmentRecord::product_count_Tooltip("How many units were received");

const QString ShipmentRecord::product_left_Label("Inventory left");
const QString ShipmentRecord::product_left_Tooltip("How much inventory is left after prescriptions and write-offs");

const QString ShipmentRecord::write_off_Label("Written off");
const QString ShipmentRecord::write_off_Tooltip("Inventory that has been written off");

const QString ShipmentRecord::write_off_Label_edit("Write off");
const QString ShipmentRecord::write_off_Tooltip_edit("Write off inventory");

const QString ShipmentRecord::name_barcode_Label("Medication/Barcode");
const QString ShipmentRecord::name_barcode_Tooltip("Medication name or shipment barcode");

const QString ShipmentRecord::barcode_Label("Barcode");
const QString ShipmentRecord::barcode_Tooltip("Shipment barcode");

ShipmentRecord::ShipmentRecord(QObject *parent):
	QObject(parent),
	id(SQL::Undefined_ID),
	drug_id(SQL::Undefined_ID),
	product_count(-1),
	product_left(-1),
	write_off(0),
	active(true),
	exists(false)
{
}

/* SQL without C++:
SELECT drug_id, expiration, lot, product_count, product_left, write_off, active
FROM shipments
WHERE id = 'SOME_VAL';
*/
bool ShipmentRecord::retrieve(int newId)
{
	QSqlQuery *model;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQuery;
	model->prepare("SELECT drug_id, expiration, lot, product_count, product_left, write_off, active "
				   "FROM shipments "
				   "WHERE id = ?;");
	model->bindValue(0, QVariant(newId));

	if (!alert.attemptQuery(model)) {
		return false;	// Query failed
	}

	model->next();
	id = newId;
	drug_id = model->value(0).toInt();
	expiration = QDate().fromString(model->value(1).toString(), "yyyy-MM-dd");
	lot = model->value(2).toString();
	product_count = model->value(3).toInt();
	product_left = model->value(4).toInt();
	write_off = model->value(5).toInt();
	active = model->value(6).toBool();
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
INSERT INTO shipments (drug_id, expiration, lot, product_count, product_left, write_off, active)
VALUES ('SOME_VAL', 'SOME_VAL', 'SOME_VAL', 'SOME_VAL');

UPDATE shipments
SET drug_id = 'SOME_VAL', expiration = 'SOME_VAL', lot = 'SOME_VAL', product_count = 'SOME_VAL',
product_left = 'SOME_VAL', write_off = 'SOME_VAL', active = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool ShipmentRecord::commitRecord()
{
	QSqlQuery *model;
	AlertInterface alert;

	model = new QSqlQuery;

	if (!exists) {
		model->prepare("INSERT INTO shipments (drug_id, expiration, lot, product_count, product_left, write_off, active) "
					   "VALUES (?, ?, ?, ?, ?, ?, ?);");
		model->bindValue(0, QVariant(drug_id));
		model->bindValue(1, QVariant(expiration.toString("yyyy-MM-dd")));
		model->bindValue(2, QVariant(SQL::prepNoMatching(lot)));
		model->bindValue(3, QVariant(product_count));
		model->bindValue(4, QVariant(product_left));
		model->bindValue(5, QVariant(write_off));
		model->bindValue(6, QVariant(active));
	} else {
		model->prepare("UPDATE shipments "
					   "SET drug_id = ?, expiration = ?, lot = ?, product_count = ?, product_left = ?, "
					   "write_off = ?, active = ? "
					   "WHERE id = ?;");
		model->bindValue(0, QVariant(drug_id));
		model->bindValue(1, QVariant(expiration.toString("yyyy-MM-dd")));
		model->bindValue(2, QVariant(SQL::prepNoMatching(lot)));
		model->bindValue(3, QVariant(product_count));
		model->bindValue(4, QVariant(product_left));
		model->bindValue(5, QVariant(write_off));
		model->bindValue(6, QVariant(active));
		model->bindValue(7, QVariant(id));
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

/* SQL without C++:
UPDATE shipments
SET product_left = (shipments.product_left - SOME_VAL), write_off = (shipments.write_off + SOME_VAL)
WHERE id = 'SOME_VAL';
*/
bool ShipmentRecord::addWriteOff(int wo_amount)
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	if (!exists) {
		write_off += wo_amount;
		product_left -= wo_amount;
		return commitRecord();
	}

	model = new QSqlQueryModel;
	query = QString("UPDATE shipments SET product_left = (shipments.product_left - ");
	query += QString().setNum(wo_amount) + QString("), write_off = (shipments.write_off + ");
	query += QString().setNum(wo_amount) + QString(") WHERE id = '");
	query += QString().setNum(id) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		delete model;
		return false;
	}

	delete model;
	return true;
}

void ShipmentRecord::print()
{
	qDebug() << "id =" << id << ", drug_id =" << drug_id << ", expiration =" << expiration.toString("yyyy-MM-dd") << ", lot =" << lot << ", product_count =" << product_count << ", product_left =" << product_left << ", write_off =" << write_off << ", active =" << active;
}
