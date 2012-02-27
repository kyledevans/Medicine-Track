/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PRESCRIPTION_01_H
#define PRESCRIPTION_01_H

#include <QList>

#include <QWizardPage>

#include "db/patientrecord.h"
#include "medicationrecord.h"
#include "db/prescriptionrecord.h"
#include "shipmentrecord.h"
#include "prescriberrecord.h"
#include "pharmacistrecord.h"

namespace Ui {
	class Prescription_01;
}

class Prescription_01 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Prescription_01(QWidget *parent = 0);
	~Prescription_01();

	void setPatient(PatientRecord *);
	void setPrescription(PrescriptionRecord *);
	void setMedication(MedicationRecord *);
	void setShipment(ShipmentRecord *);

	bool initCustom();
	void getResults();

	void medUpdated();

private:
	Ui::Prescription_01 *ui;

	void deleteLists();
	void setupComboBoxes();

	PatientRecord *patient;
	PrescriptionRecord *prescription;
	MedicationRecord *medication;
	ShipmentRecord *shipment;

	QList<PrescriberRecord *> prescribers;
	QList<PharmacistRecord *> pharmacists;
};

#endif // PRESCRIPTION_01_H
