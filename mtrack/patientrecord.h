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
	int id;
	QString first;
	QString last;
	QDate dob;
};

#endif // PATIENTRECORD_H
