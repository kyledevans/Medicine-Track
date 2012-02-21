/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>

#include <QObject>

#ifndef MEDICATIONRECORD_H
#define MEDICATIONRECORD_H

class MedicationRecord : public QObject
{
	Q_OBJECT
public:
	explicit MedicationRecord(QObject *parent = 0);

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

	bool retrieve(int);		// (int) is the id.  Returns false if the retrieve failed
	bool commitRecord();	// Inserts (or updates) the record in the db
	bool toggleActive();	// Sets the record as active/inactive

	void print();			// Prints to the debug interface

	/* Use these to keep UI strings consistent and sane. */
	static const QString name_Label;
	static const QString name_Tooltip;

	static const QString generic_Label;
	static const QString generic_Tooltip;

	static const QString manufacturer_Label;
	static const QString manufacturer_Tooltip;

	static const QString ndc_Label;
	static const QString ndc_Tooltip;

	static const QString form_Label;
	static const QString form_Tooltip;

	static const QString strength_Label;
	static const QString strength_Tooltip;
	static const QString strength_Tooltip_edit;

	static const QString dispense_units_Label;
	static const QString dispense_units_Tooltip;

	static const QString unit_size_Label;
	static const QString unit_size_Tooltip;

	static const QString instructions_Label;
	static const QString instructions_Tooltip;
};

#endif // MEDICATIONRECORD_H
