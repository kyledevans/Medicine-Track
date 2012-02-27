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

	int id;					// prescribers.id
	QString last;			// prescribers.last
	QString first;			// prescribers.first
	QString full_name;		// prescribers.full_name
	bool active;			// prescribers.active

	bool exists;			// True if the record is already in the database

	bool retrieve(int);		// (int) is the prescriber id.  Returns false on failure
	bool commitRecord();	// Inserts (or updates) the record in the db
	bool toggleActive();	// Toggles the active state

	void print();			// Prints to the debug interface

	/* Use these to keep UI strings consistent and sane. */
	static const QString last_Label;
	static const QString last_Tooltip;

	static const QString first_Label;
	static const QString first_Tooltip;

	static const QString full_name_Label;
	static const QString full_name_Tooltip;
	static const QString full_name_Label_secondary;
	static const QString full_name_Tooltip_secondary;
};

#endif // PRESCRIBERRECORD_H
