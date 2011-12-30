/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef SHIPMENTRECORD_H
#define SHIPMENTRECORD_H

#include <QDate>
#include <QString>

class ShipmentRecord
{
public:
    ShipmentRecord();

	int id;
	int drug_id;
	QDate expiration;
	QString lot;
	int product_count;
	int product_left;

	bool exists;
};

#endif // SHIPMENTRECORD_H
