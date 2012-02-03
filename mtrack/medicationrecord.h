/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>

#ifndef MEDICATIONRECORD_H
#define MEDICATIONRECORD_H

class MedicationRecord
{
public:
    MedicationRecord();

	// Table 'drugs' columns
	int id;					// drugs.id
	QString name;			// drugs.name
	QString generic;		// drugs.generic
	QString manufacturer;	// drugs.manufacturer
	QString ndc;			// drugs.ndc
	int form;				// drugs.form
	int strength;			// drugs.strength
	QString str_units;		// drugs.str_units
	QString dispense_units;	// drugs.dispense_units
	QString unit_size;		// drugs.unit_size
	QString instructions;	// drugs.instructions
	bool active;			// drugs.active

	bool exists;			// True if the record is already in the database

	bool retrieve(int);		// (int) is the id.  Returns false if the retrieve failed
	bool commitRecord();	// Inserts (or updates) the record in the db

	void print();			// Prints to the debug interface
};

#endif // MEDICATIONRECORD_H
