/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QString>
#include <QVariant>
#include <QSqlRecord>

#include "shipmentrecord.h"

#include "globals.h"
#include "alertinterface.h"

ShipmentRecord::ShipmentRecord():
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
	product_count = model->record(0).value(3).toInt;
	product_left = model->record(0).value(4).toInt();
	exists = true;

	delete model;
	return true;
}
