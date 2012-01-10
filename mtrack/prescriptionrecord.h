/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QDate>

#ifndef PRESCRIPTIONRECORD_H
#define PRESCRIPTIONRECORD_H

class PrescriptionRecord
{
public:
    PrescriptionRecord();

	int id;
	int patient_id;
	QString last;
	QString first;
	QDate dob;
	QString drug;
	QString form;
	QDate written;
	QDate filled;
	QString strength;
	QString amount;
	QString dose_size;
	QString manufacturer;
	QString lot;
	QDate expiration;
	QString ndc;
	QString pharmacist;
	QString prescriber;
	QString instructions;
	QString comments;
};

#endif // PRESCRIPTIONRECORD_H
