/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QObject>

#ifndef PRESCRIBERRECORD_H
#define PRESCRIBERRECORD_H

class PrescriberRecord : public QObject
{
	Q_OBJECT
public:
	explicit PrescriberRecord(QObject *parent = 0);

	int id;					// prescribers.id
	QString last;			// prescribers.last
	QString first;			// prescribers.first
	QString full_name;		// prescribers.full_name
	bool active;			// prescribers.active

	bool exists;			// True if the record is already in the database

	bool retrieve(int);		// (int) is the prescriber id.  Returns false on failure
	bool commitRecord();	// Inserts (or updates) the record in the db

	void print();			// Prints to the debug interface
};

#endif // PRESCRIBERRECORD_H
