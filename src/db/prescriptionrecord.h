/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QDate>

#ifndef PRESCRIPTIONRECORD_H
#define PRESCRIPTIONRECORD_H

class PrescriptionRecord
{
public:
	explicit PrescriptionRecord();

	// Accessors
	int getId();
	int getPatient_id();
	int getDrug_id();
	int getShipment_id();
	int getPrescriber_id();
	int getPharmacist_id();
	int getAmount();
	QString getDose_size();
	QDate getWritten();
	QDate getFilled();
	QString getInstructions();
	QString getComments();
	bool getActive();
	bool getExists();

	// Mutators
	void setId(int);
	void setPatient_id(int);
	void setDrug_id(int);
	void setShipment_id(int);
	void setPrescriber_id(int);
	void setPharmacist_id(int);
	void setAmount(int);
	void setDose_size(QString);
	void setWritten(QDate);
	void setFilled(QDate);
	void setInstructions(QString);
	void setComments(QString);
	void setActive(bool);

	// Methods
	bool retrieve(int);		// (int) is the prescription id.  Returns false if retrieve fails
	bool commitRecord();	// Returns false if the procedure failed
	bool toggleActive();

	void print();			// Prints to the default debug interface

	/* Use these to keep UI strings consistent and sane. */
	static const QString amount_Label;
	static const QString amount_Tooltip;

	static const QString dose_size_Label;
	static const QString dose_size_Tooltip;

	static const QString written_Label;
	static const QString written_Tooltip;

	static const QString filled_Label;
	static const QString filled_Tooltip;

	static const QString instructions_Label;
	static const QString instructions_Tooltip;

	static const QString active_Label;
	static const QString active_Tooltip;

private:
	int id;					// prescriptions.id
	int patient_id;			// prescriptions.patient_id
	int drug_id;			// prescriptions.drug_id
	int shipment_id;		// prescriptions.shipment_id
	int prescriber_id;		// prescriptions.prescriber_id
	int pharmacist_id;		// prescriptions.pharmacist_id
	int amount;				// prescriptions.amount
	QString dose_size;		// prescriptions.dose_size
	QDate written;			// prescriptions.written
	QDate filled;			// prescriptions.filled
	QString instructions;	// prescriptions.instructions
	QString comments;		// prescriptions.comments
	bool active;			// prescriptions.active

	bool exists;			// True if the record is already in the database
};

#endif // PRESCRIPTIONRECORD_H
