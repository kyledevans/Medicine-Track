/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef SHIPMENTRECORD_H
#define SHIPMENTRECORD_H

#include <QDate>
#include <QString>

class ShipmentRecord
{
public:
	explicit ShipmentRecord();

	int id;					// shipments.id
	int drug_id;			// shipments.drug_id
	QDate expiration;		// shipments.expiration
	QString lot;			// shipments.lot
	int product_count;		// shipments.product_count
	int product_left;		// shipments.product_left
	int write_off;			// shipments.write_off
	bool active;			// shipments.active

	bool exists;			// True if the db already contains this record

	bool retrieve(int);		// (int) is the shipment id.  Returns false if retrieve failed
	bool commitRecord();	// Inserts (or updates) the record in the db
	bool addWriteOff(int);	// Removes (int) amount of stock.  False on failure.
	bool toggleActive();	// Toggles the active state
    bool addInventory(int); // Increases the amount received and amount left by (int)

	void print();			// Prints to the debug interface

	/* Use these to keep UI strings consistent and sane. */
	static const QString expiration_Label;
	static const QString expiration_Tooltip;

	static const QString lot_Label;
	static const QString lot_Tooltip;

	static const QString product_count_Label;
	static const QString product_count_Tooltip;

	static const QString product_left_Label;
	static const QString product_left_Tooltip;

	static const QString write_off_Label;
	static const QString write_off_Tooltip;

	static const QString write_off_Label_edit;
	static const QString write_off_Tooltip_edit;

	static const QString name_barcode_Label;
	static const QString name_barcode_Tooltip;

	static const QString barcode_Label;
	static const QString barcode_Tooltip;
};

#endif // SHIPMENTRECORD_H
