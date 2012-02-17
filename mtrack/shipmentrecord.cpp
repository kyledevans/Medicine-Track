/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
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

const QString ShipmentRecord::write_off_Label("Write off");
const QString ShipmentRecord::write_off_Tooltip("Write off inventory");

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
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	if (newId == SQL::Undefined_ID)	{
		return false;
	}

	model = new QSqlQueryModel;
	query += QString("SELECT drug_id, expiration, lot, product_count, product_left, write_off, active FROM shipments WHERE id = '");
	query += QString().setNum(newId) + QString("';");

	if (!alert.attemptQuery(model, &query)) {
		return false;	// Query failed
	}

	id = newId;
	drug_id = model->record(0).value(0).toInt();
	expiration = QDate().fromString(model->record(0).value(1).toString(), "yyyy-MM-dd");
	lot = model->record(0).value(2).toString();
	product_count = model->record(0).value(3).toInt();
	product_left = model->record(0).value(4).toInt();
	write_off = model->record(0).value(5).toInt();
	active = model->record(0).value(6).toBool();
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
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	model = new QSqlQueryModel;

	if (!exists) {
		query = QString("INSERT INTO shipments (drug_id, expiration, lot, product_count, product_left, write_off, active) VALUES ('");
		query += QString().setNum(drug_id) + QString("', '");
		query += expiration.toString("yyyy-MM-dd") + QString("', '");
		query += SQL::cleanNoMatching(lot) + QString("', '");
		query += QString().setNum(product_count) + QString("', '");
		query += QString().setNum(product_left) + QString("', '");
		query += QString().setNum(write_off) + QString("', '");
		if (active) {
			query += QString("1');");
		} else {
			query += QString("0');");
		}
	} else {
		query = QString("UPDATE shipments SET drug_id = '");
		query += QString().setNum(drug_id) + QString("', expiration = '");
		query += expiration.toString("yyyy-MM-dd") + QString("', lot = '");
		query += SQL::cleanNoMatching(lot) + QString("', product_count = '");
		query += QString().setNum(product_count) + QString("', product_left = '");
		query += QString().setNum(product_left) + QString("', write_off = '");
		query += QString().setNum(write_off) + QString("', active = '");
		if (active) {
			query += QString("1' ");
		} else {
			query += QString("0' ");
		}
		query += QString("WHERE id = '");
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
