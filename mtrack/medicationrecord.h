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
	int id;
	int instruction_id;
	QString name;
	QString generic;
	QString manufacturer;
	QString ndc;
	int form;				// See globals.h for FORM* values
	QString strength;
	QString amount;
	bool active;

	// Table 'instructions' columns
	QString instructions;

	bool exists;			// True if the record is already in the database

	bool retrieve(int);		// (int) is the id.  Returns false if the retrieve failed
	bool commitRecord();	// Inserts (or updates) the record in the db

	void print();			// Prints to the debug interface
};

#endif // MEDICATIONRECORD_H
