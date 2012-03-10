/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipdeltarecord.h"

#include "alertinterface.h"
#include "globals.h"

#include <QDebug>

ShipDeltaRecord::ShipDeltaRecord():
	id(SQL::Undefined_ID),
	shipment_id(SQL::Undefined_ID),
	amount(-1),
	count_changed(false),
	active(true),
	exists(false)
{
}

int ShipDeltaRecord::getId()
{
	return id;
}

int ShipDeltaRecord::getShipment_id()
{
	return shipment_id;
}

int ShipDeltaRecord::getAmount()
{
	return amount;
}

bool ShipDeltaRecord::getCount_changed()
{
	return count_changed;
}

QDate ShipDeltaRecord::getTimestamp()
{
	return timestamp;
}

bool ShipDeltaRecord::getActive()
{
	return active;
}

bool ShipDeltaRecord::getExists()
{
	return exists;
}

void ShipDeltaRecord::setId(int new_id)
{
	id = new_id;
}

void ShipDeltaRecord::setShipment_id(int new_shipment_id)
{
	shipment_id = new_shipment_id;
}

void ShipDeltaRecord::setAmount(int new_amount)
{
	amount = new_amount;
}

void ShipDeltaRecord::setCount_changed(bool new_count_changed)
{
	count_changed = new_count_changed;
}

void ShipDeltaRecord::setTimestamp(QDate new_timestamp)
{
	timestamp = new_timestamp;
}

void ShipDeltaRecord::setActive(bool new_active)
{
	active = new_active;
}

bool ShipDeltaRecord::retrieve(int new_id)
{
	QSqlQuery model;
	AlertInterface alert;

	if (new_id == SQL::Undefined_ID)	{
		return false;
	}

	model.prepare("SELECT shipment_id, amount, count_changed, timestamp, active "
				   "FROM ship_delta "
				   "WHERE id = ?;");
	model.bindValue(0, QVariant(new_id));

	if (!alert.attemptQuery(&model)) {
		return false;	// Query failed
	}

	model.next();
	id = new_id;
	shipment_id = model.value(0).toInt();
	amount = model.value(1).toInt();
	count_changed = model.value(2).toBool();
	timestamp = model.value(3).toDate();
	active = model.value(4).toBool();
	exists = true;

	return true;
}

bool ShipDeltaRecord::commitRecord()
{
	QSqlQuery model;
	AlertInterface alert;

	if (exists) {	// TODO: haven't implemented UPDATE record yet
		return false;
	}

	model.prepare("INSERT INTO ship_delta (shipment_id, amount, count_changed, timestamp, active) "
				  "VALUES (?, ?, ?, CURDATE(), ?);");
	model.bindValue(0, QVariant(shipment_id));
	model.bindValue(1, QVariant(amount));
	model.bindValue(2, QVariant(count_changed));
	model.bindValue(3, QVariant(active));

	if (!alert.attemptQuery(&model)) {
		return false;	// Query failed
	}

	if (!exists) {
		id = model.lastInsertId().toInt();
		exists = true;
	}

	return true;
}

bool ShipDeltaRecord::toggleActive()
{
	QSqlQuery model;
	AlertInterface alert;

	if (!exists) {	// Doesn't exist, so can't toggle
		return false;
	}

	model.prepare("UPDATE ship_delta "
				  "SET active = ? "
				  "WHERE id = ?;");
	model.bindValue(0, QVariant(!active));
	model.bindValue(1, QVariant(id));

	if (!alert.attemptQuery(&model)) {
		return false;	// Query failed
	}

	active = !active;

	return true;
}

void ShipDeltaRecord::print()
{
	qDebug() << "id =" << id << ", shipment_id =" << shipment_id << ", amount =" << amount << ", timestamp =" << timestamp.toString("yyyy-MM-dd") << ", active =" << active;
}
