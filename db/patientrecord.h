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
	explicit PatientRecord();

	// Accessors
	int getId();
	int getAllscripts_id();
	QString getLast();
	QString getFirst();
	QDate getDob();
	bool getActive();
	bool getExists();

	// Mutators
	void setId(int);
	void setAllscripts_id(int);
	void setLast(QString);
	void setFirst(QString);
	void setDob(QDate);
	void setActive(bool);

	// Methods
	bool retrieve(int);		// (int) is the patient id. Returns false if the retrieve failed
	bool commitRecord();	// Inserts (or updates) the record in the db
	bool toggleActive();	// Sets the active state to (bool)

	void print();			// Prints to the debug interface

private:
	int id;				// patients.id
	int allscripts_id;	// patients.allscripts_id
	QString first;		// patients.first
	QString last;		// patients.last
	QDate dob;			// patients.dob
	bool active;		// patients.active

    // TODO: This is a pointless property.  If we want to check the existence of the patient record, then we should just check the id.
	bool exists;		// True if the record is already in the database
};

#endif // PATIENTRECORD_H
