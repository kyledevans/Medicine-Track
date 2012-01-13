/*
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
    PatientRecord();

	int id;				// patients.id
	int allscripts_id;	// patients.allscripts_id
	QString first;		// patients.first
	QString last;		// patients.last
	QDate dob;			// patients.dob

	bool exists;		// True if the record is already in the database

	bool retrieve(int);	// (int) is the patient id. Returns false if the retrieve failed
};

#endif // PATIENTRECORD_H
