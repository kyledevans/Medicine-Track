/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QDate>

#include <QObject>

#ifndef PATIENTRECORD_H
#define PATIENTRECORD_H

class PatientRecord : public QObject
{
	Q_OBJECT
public:
	explicit PatientRecord(QObject *parent = 0);

	int id;				// patients.id
	int allscripts_id;	// patients.allscripts_id
	QString first;		// patients.first
	QString last;		// patients.last
	QDate dob;			// patients.dob
	bool active;		// patients.active

	bool exists;		// True if the record is already in the database

	bool retrieve(int);	// (int) is the patient id. Returns false if the retrieve failed
	bool commitRecord();	// Inserts (or updates) the record in the db
};

#endif // PATIENTRECORD_H
