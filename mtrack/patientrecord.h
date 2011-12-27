/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

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

	/* Table 'patients' columns */
	int id;
	QString first;
	QString last;
	QDate dob;

	bool exists;			// True if the record is already in the database
};

#endif // PATIENTRECORD_H
