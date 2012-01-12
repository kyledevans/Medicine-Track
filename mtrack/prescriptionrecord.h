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

	int id;					// prescriptions.id
	int patient_id;			// prescriptions.patient_id
	int drug_id;			// prescriptions.drug_id
	int shipment_id;		// prescriptions.shipment_id
	int prescriber_id;		// prescriptions.prescriber_id
	int pharmacist_id;		// prescriptions.pharmacist_id
	QString amount;			// prescriptions.amount
	QString dose_size;		// prescriptions.dose_size
	QDate written;			// prescriptions.written
	QDate filled;			// prescriptions.filled
	QString instructions;	// prescriptions.instructions
	QString comments;		// prescriptions.comments

	bool exists;			// True if the record is already in the database
};

#endif // PRESCRIPTIONRECORD_H
