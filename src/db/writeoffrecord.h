/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QDate>

#ifndef WRITEOFFRECORD_H
#define WRITEOFFRECORD_H

class WriteOffRecord
{
public:
	explicit WriteOffRecord();

	// Accessors
	int getId();
	int getShipment_id();
	int getAmount();
	QDate getTimestamp();
	bool getActive();
	bool getExists();

	// Mutators
	void setId(int);
	void setShipment_id(int);
	void setAmount(int);
	void setTimestamp(QDate);
	void setActive(bool);

	// Methods
	bool retrieve(int);		// (int) is the pharmacist id.  Returns false if the retrieve failed
	bool commitRecord();	// Inserts (or Updates) the record in the db
	bool toggleActive();	// Toggles the active state

	void print();			// Prints to the debug interface

private:
	int id;				// write_offs.id
	int shipment_id;	// write_offs.shipment_id
	int amount;			// write_offs.amount
	QDate timestamp;	// write_offs.timestamp
	bool active;		// write_offs.active

	bool exists;
};

#endif // WRITEOFFRECORD_H
