/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef SHIPMENTRECORD_H
#define SHIPMENTRECORD_H

#include <QDate>
#include <QString>

#include <QObject>

class ShipmentRecord : public QObject
{
	Q_OBJECT
public:
	explicit ShipmentRecord(QObject *parent = 0);

	int id;					// shipments.id
	int drug_id;			// shipments.drug_id
	QDate expiration;		// shipments.expiration
	QString lot;			// shipments.lot
	int product_count;		// shipments.product_count
	int product_left;		// shipments.product_left

	bool exists;			// True if the db already contains this record

	bool retrieve(int);		// (int) is the shipment id.  Returns false if retrieve failed
	bool commitRecord();	// Inserts (or updates) the record in the db
};

#endif // SHIPMENTRECORD_H
