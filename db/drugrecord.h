/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>

#ifndef DRUGRECORD_H
#define DRUGRECORD_H

class DrugRecord
{
public:
	explicit DrugRecord();

	// Accessors
	int getId();
	QString getName();
	QString getGeneric();
	QString getManufacturer();
	QString getNdc();
	int getForm();
	QString getStrength();
	QString getDispense_units();
	QString getUnit_size();
	QString getInstructions();
	bool getActive();
	bool getExists();

	// Mutators
	void setId(int);
	void setName(QString);
	void setGeneric(QString);
	void setManufacturer(QString);
	void setNdc(QString);
	void setForm(int);
	void setStrength(QString);
	void setDispense_units(QString);
	void setUnit_size(QString);
	void setInstructions(QString);
	void setActive(bool);

	// Methods
	bool retrieve(int);		// (int) is the id.  Returns false if the retrieve failed
	bool commitRecord();	// Inserts (or updates) the record in the db
	bool toggleActive();	// Sets the record as active/inactive

	void print();			// Prints to the debug interface

private:
	// Table 'drugs' columns
	int id;					// drugs.id
	QString name;			// drugs.name
	QString generic;		// drugs.generic
	QString manufacturer;	// drugs.manufacturer
	QString ndc;			// drugs.ndc
	int form;				// drugs.form
	QString strength;		// drugs.strength
	QString dispense_units;	// drugs.dispense_units
	QString unit_size;		// drugs.unit_size
	QString instructions;	// drugs.instructions
	bool active;			// drugs.active

	bool exists;			// True if the record is already in the database
};

#endif // DRUGRECORD_H
