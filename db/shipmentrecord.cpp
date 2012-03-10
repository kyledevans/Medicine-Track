/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipmentrecord.h"

#include "alertinterface.h"
#include "shipdeltarecord.h"
#include "globals.h"

#include <QDebug>

ShipmentRecord::ShipmentRecord():
	id(SQL::Undefined_ID),
	drug_id(SQL::Undefined_ID),
	product_count(-1),
	product_left(-1),
	write_off(0),
	active(true),
	exists(false)
{
}

int ShipmentRecord::getId()
{
	return id;
}

int ShipmentRecord::getDrug_id()
{
	return drug_id;
}

QDate ShipmentRecord::getExpiration()
{
	return expiration;
}

QString ShipmentRecord::getLot()
{
	return lot;
}

int ShipmentRecord::getProduct_count()
{
	return product_count;
}

int ShipmentRecord::getProduct_left()
{
	return product_left;
}

int ShipmentRecord::getWrite_off()
{
	return write_off;
}

bool ShipmentRecord::getActive()
{
	return active;
}

bool ShipmentRecord::getExists()
{
	return exists;
}

void ShipmentRecord::setId(int new_id)
{
	id = new_id;
}

void ShipmentRecord::setDrug_id(int new_drug_id)
{
	drug_id = new_drug_id;
}

void ShipmentRecord::setExpiration(QDate new_expiration)
{
	expiration = new_expiration;
}

void ShipmentRecord::setLot(QString new_lot)
{
	lot = new_lot;
}

void ShipmentRecord::setProduct_count(int new_product_count)
{
	product_count = new_product_count;
}

void ShipmentRecord::setProduct_left(int new_product_left)
{
	product_left = new_product_left;
}

void ShipmentRecord::setWrite_off(int new_write_off)
{
	write_off = new_write_off;
}

void ShipmentRecord::setActive(bool new_active)
{
	active = new_active;
}

/* SQL without C++:
UPDATE shipments
SET product_count = (product_count + ?), product_left = (product_left + ?)
WHERE id = ?;
*/
bool ShipmentRecord::addInventory(int delta)
{
	QSqlQuery *model;
	QSqlDatabase db;
	AlertInterface alert;
	ShipDeltaRecord rec;

	if (!exists) {
		return false;
	}

	db = QSqlDatabase::database();	// Get the default DB
	db.transaction();

	rec.setShipment_id(id);
	rec.setAmount(delta);
	rec.setCount_changed(true);
	if (!rec.commitRecord()) {
		db.rollback();
		return false;
	}

	model = new QSqlQuery;

	model->prepare("UPDATE shipments "
				   "SET product_count = (product_count + ?), product_left = (product_left + ?) "
				   "WHERE id = ?;");
	model->bindValue(0, QVariant(delta));
	model->bindValue(1, QVariant(delta));
	model->bindValue(2, QVariant(id));

	if (!alert.attemptQuery(model)) {
		db.rollback();
		delete model;
		return false;
	}
	db.commit();

	product_count += delta;
	product_left += delta;

	delete model;
	return true;
}

/* SQL without C++:
UPDATE shipments
SET product_left = (shipments.product_left - ?), write_off = (shipments.write_off + ?)
WHERE id = ?;
*/
bool ShipmentRecord::addWriteOff(int wo_amount)
{
	QSqlQuery *model;
	QSqlDatabase db;
	AlertInterface alert;
	ShipDeltaRecord rec;

	if (!exists) {
		return false;
	}

	db = QSqlDatabase::database();	// Get the default DB
	db.transaction();

	rec.setShipment_id(id);
	rec.setAmount(wo_amount);
	if (!rec.commitRecord()) {
		db.rollback();
		return false;
	}

	model = new QSqlQuery;
	model->prepare("UPDATE shipments "
				   "SET product_left = (shipments.product_left - ?), write_off = (shipments.write_off + ?) "
				   "WHERE id = ?;");
	model->bindValue(0, QVariant(wo_amount));
	model->bindValue(1, QVariant(wo_amount));
	model->bindValue(2, QVariant(id));

	if (!alert.attemptQuery(model)) {
		db.rollback();
		delete model;
		return false;
	}
	db.commit();

	write_off += wo_amount;
	product_left -= wo_amount;

	delete model;
	return true;
}

/* SQL without C++:
UPDATE shipments
SET active = ?
WHERE id = ?;
*/
bool ShipmentRecord::toggleActive()
{
	QSqlQuery *model;
	AlertInterface alert;

	if (!exists) {		// Entry doesn't exist - do nothing
		return false;
	}

	model = new QSqlQuery;

	model->prepare("UPDATE shipments "
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
		delete model;
		return false;	// Query failed
	}

	model->next();
	id = newId;
	drug_id = model->value(0).toInt();
	expiration = model->value(1).toDate();
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
		model->bindValue(1, QVariant(expiration));
		model->bindValue(2, SQL::prepNoMatching(lot));
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
		model->bindValue(1, QVariant(expiration));
		model->bindValue(2, QVariant(SQL::prepNoMatching(lot)));
		model->bindValue(3, QVariant(product_count));
		model->bindValue(4, QVariant(product_left));
		model->bindValue(5, QVariant(write_off));
		model->bindValue(6, QVariant(active));
		model->bindValue(7, QVariant(id));
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

void ShipmentRecord::print()
{
	qDebug() << "id =" << id << ", drug_id =" << drug_id << ", expiration =" << expiration.toString("yyyy-MM-dd") << ", lot =" << lot << ", product_count =" << product_count << ", product_left =" << product_left << ", write_off =" << write_off << ", active =" << active;
}
