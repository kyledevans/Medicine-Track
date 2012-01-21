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

ShipmentRecord::ShipmentRecord(QObject *parent):
	QObject(parent),
	id(SQL::Undefined_ID),
	drug_id(SQL::Undefined_ID),
	product_count(-1),
	product_left(-1),
	exists(false)
{
}

/* SQL without C++:
SELECT drug_id, expiration, lot, product_count, product_left
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
	query += QString("SELECT drug_id, expiration, lot, product_count, product_left FROM shipments WHERE id = '");
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
	exists = true;

	delete model;
	return true;
}

/* SQL without C++:
INSERT INTO shipments (drug_id, expiration, lot, product_count, product_left)
VALUES ('SOME_VAL', 'SOME_VAL', 'SOME_VAL', 'SOME_VAL');

UPDATE shipments
SET drug_id = 'SOME_VAL', expiration = 'SOME_VAL', lot = 'SOME_VAL', product_count = 'SOME_VAL', product_left = 'SOME_VAL'
WHERE id = 'SOME_VAL';
*/
bool ShipmentRecord::commitRecord()
{
	QSqlQueryModel *model;
	QString query;
	AlertInterface alert;

	model = new QSqlQueryModel;

	if (exists) {
		query = QString("INSERT INTO shipments (drug_id, expiration, lot, product_count, product_left) VALUES ('");
		query += QString().setNum(drug_id) + QString("', '");
		query += expiration.toString("yyyy-MM-dd") + QString("', '");
		query += SQL::cleanNoMatching(lot) + QString("', '");
		query += QString().setNum(product_count) + QString("', '");
		query += QString().setNum(product_left) + QString("');");
	} else {
		query = QString("UPDATE shipments SET drug_id = '");
		query += QString().setNum(drug_id) + QString("', expiration = '");
		query += expiration.toString("yyyy-MM-dd") + QString("', lot = '");
		query += SQL::cleanNoMatching(lot) + QString("', product_count = '");
		query += QString().setNum(product_count) + QString("', product_left = '");
		query += QString().setNum(product_left) + QString("' WHERE id = '");
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
