/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>

#ifndef PHARMACISTRECORD_H
#define PHARMACISTRECORD_H

class PharmacistRecord
{
public:
    PharmacistRecord();

	int id;				// pharmacists.id
	QString last;		// pharmacists.last
	QString first;		// pharmacists.first
	QString initials;	// pharmacists.initials
	bool active;		// pharmacists.active

	bool exists;		// True if the record is already in the database

	bool retrieve(int);	// (int) is the pharmacist id.  Returns false if the retrieve failed
};

#endif // PHARMACISTRECORD_H
