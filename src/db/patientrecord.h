﻿/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QDate>

#ifndef PATIENTRECORD_H
#define PATIENTRECORD_H

class PatientRecord
{
public:
	explicit PatientRecord();

	int id;				// patients.id
	int allscripts_id;	// patients.allscripts_id
	QString first;		// patients.first
	QString last;		// patients.last
	QDate dob;			// patients.dob
	bool active;		// patients.active

	bool exists;		// True if the record is already in the database

	bool retrieve(int);			// (int) is the patient id. Returns false if the retrieve failed
	bool commitRecord();		// Inserts (or updates) the record in the db
	bool toggleActive();		// Sets the active state to (bool)

	// TODO: Add a print() method

	/* Use these to keep UI strings consistent and sane. */
	static const QString allscripts_id_Label;
	static const QString allscripts_id_Tooltip;

	static const QString last_Label;
	static const QString last_Tooltip;

	static const QString first_Label;
	static const QString first_Tooltip;

	static const QString dob_Label;
	static const QString dob_Tooltip;
};

#endif // PATIENTRECORD_H
