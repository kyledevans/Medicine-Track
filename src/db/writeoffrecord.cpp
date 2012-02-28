/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "writeoffrecord.h"

#include "alertinterface.h"
#include "../globals.h"

#include <QDebug>

WriteOffRecord::WriteOffRecord():
	id(SQL::Undefined_ID),
	shipment_id(SQL::Undefined_ID),
	amount(-1),
	active(true),
	exists(false)
{
}

int WriteOffRecord::getId()
{
	return id;
}

int WriteOffRecord::getShipment_id()
{
	return shipment_id;
}

int WriteOffRecord::getAmount()
{
	return amount;
}

QDate WriteOffRecord::getTimestamp()
{
	return timestamp;
}

bool WriteOffRecord::getActive()
{
	return active;
}

bool WriteOffRecord::getExists()
{
	return exists;
}

void WriteOffRecord::setId(int new_id)
{
	id = new_id;
}

void WriteOffRecord::setShipment_id(int new_shipment_id)
{
	shipment_id = new_shipment_id;
}

void WriteOffRecord::setAmount(int new_amount)
{
	amount = new_amount;
}

void WriteOffRecord::setTimestamp(QDate new_timestamp)
{
	timestamp = new_timestamp;
}

void WriteOffRecord::setActive(bool new_active)
{
	active = new_active;
}

bool WriteOffRecord::retrieve(int new_id)
{
	QSqlQuery model;
	AlertInterface alert;

	if (new_id == SQL::Undefined_ID)	{
		return false;
	}

	model.prepare("SELECT shipment_id, amount, timestamp, active "
				   "FROM write_offs "
				   "WHERE id = ?;");
	model.bindValue(0, QVariant(new_id));

	if (!alert.attemptQuery(&model)) {
		return false;	// Query failed
	}

	model.next();
	id = new_id;
	shipment_id = model.value(0).toInt();
	amount = model.value(1).toInt();
	timestamp = model.value(2).toDate();
	active = model.value(3).toBool();
	exists = true;

	return true;
}

bool WriteOffRecord::commitRecord()
{
	QSqlQuery model;
	AlertInterface alert;

	if (exists) {	// TODO: haven't implemented UPDATE record yet
		return false;
	}

	model.prepare("INSERT INTO shipments (shipment_id, amount, timestamp, active) "
				  "VALUES (?, ?, ?, ?);");
	model.bindValue(0, QVariant(shipment_id));
	model.bindValue(1, QVariant(amount));
	model.bindValue(2, QVariant(timestamp));
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

bool WriteOffRecord::toggleActive()
{
	QSqlQuery model;
	AlertInterface alert;

	if (!exists) {	// Doesn't exist, so can't toggle
		return false;
	}

	model.prepare("UPDATE write_offs "
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

void WriteOffRecord::print()
{
	qDebug() << "id =" << id << ", shipment_id =" << shipment_id << ", amount =" << amount << ", timestamp =" << timestamp.toString("yyyy-MM-dd") << ", active =" << active;
}
