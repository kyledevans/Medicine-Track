/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QDate>
#include <QString>

#ifndef SHIPMENTRECORD_H
#define SHIPMENTRECORD_H

class ShipmentRecord
{
public:
	explicit ShipmentRecord();

	// Accessors
	int getId();
	int getDrug_id();
	QDate getExpiration();
	QString getLot();
	int getProduct_count();
	int getProduct_left();
	int getWrite_off();
	bool getActive();
	bool getExists();

	// Mutators
	void setId(int);
	void setDrug_id(int);
	void setExpiration(QDate);
	void setLot(QString);
	void setProduct_count(int);
	void setProduct_left(int);
	void setWrite_off(int);
	void setActive(bool);

	// Methods
	bool retrieve(int);		// (int) is the shipment id.  Returns false if retrieve failed
	bool commitRecord();	// Inserts (or updates) the record in the db
	bool toggleActive();	// Toggles the active state

	bool addWriteOff(int);	// Removes (int) amount of stock.  False on failure.
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

private:
	int id;					// shipments.id
	int drug_id;			// shipments.drug_id
	QDate expiration;		// shipments.expiration
	QString lot;			// shipments.lot
	int product_count;		// shipments.product_count
	int product_left;		// shipments.product_left
	int write_off;			// shipments.write_off
	bool active;			// shipments.active

	bool exists;			// True if the db already contains this record
};

#endif // SHIPMENTRECORD_H
