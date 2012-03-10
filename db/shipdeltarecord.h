/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QDate>

#ifndef SHIPDELTARECORD_H
#define SHIPDELTARECORD_H

class ShipDeltaRecord
{
public:
	explicit ShipDeltaRecord();

	// Accessors
	int getId();
	int getShipment_id();
	int getAmount();
	bool getCount_changed();
	QDate getTimestamp();
	bool getActive();
	bool getExists();

	// Mutators
	void setId(int);
	void setShipment_id(int);
	void setAmount(int);
	void setCount_changed(bool);
	void setTimestamp(QDate);
	void setActive(bool);

	// Methods
	bool retrieve(int);		// (int) is the pharmacist id.  Returns false if the retrieve failed
	bool commitRecord();	// Inserts (or Updates) the record in the db
	bool toggleActive();	// Toggles the active state

	void print();			// Prints to the debug interface

private:
	int id;				// ship_delta.id
	int shipment_id;	// ship_delta.shipment_id
	int amount;			// ship_delta.amount
	bool count_changed;	// ship_delta.count_changed
	QDate timestamp;	// ship_delta.timestamp
	bool active;		// ship_delta.active

	bool exists;
};

#endif // SHIPDELTARECORD_H
