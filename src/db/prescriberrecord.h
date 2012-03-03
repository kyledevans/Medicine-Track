/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>

#ifndef PRESCRIBERRECORD_H
#define PRESCRIBERRECORD_H

class PrescriberRecord
{
public:
	explicit PrescriberRecord();

	// Accessors
	int getId();
	QString getLast();
	QString getFirst();
	QString getFull_name();
	bool getActive();
	bool getExists();

	// Mutators
	void setId(int);
	void setLast(QString);
	void setFirst(QString);
	void setFull_name(QString);
	void setActive(bool);

	// Methods
	bool retrieve(int);		// (int) is the prescriber id.  Returns false on failure
	bool commitRecord();	// Inserts (or updates) the record in the db
	bool toggleActive();	// Toggles the active state

	void print();			// Prints to the debug interface

private:
	int id;					// prescribers.id
	QString last;			// prescribers.last
	QString first;			// prescribers.first
	QString full_name;		// prescribers.full_name
	bool active;			// prescribers.active

	bool exists;			// True if the record is already in the database
};

#endif // PRESCRIBERRECORD_H
