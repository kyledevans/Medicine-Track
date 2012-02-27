/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PRESCRIPTIONWIZARD_H
#define PRESCRIPTIONWIZARD_H

#include <QWizard>

#include "db/prescriptionrecord.h"
#include "db/patientrecord.h"
#include "medicationrecord.h"
#include "shipmentrecord.h"

namespace Ui {
	class PrescriptionWizard;
}

class PrescriptionWizard : public QWizard
{
	Q_OBJECT

public:
	explicit PrescriptionWizard(QWidget *parent = 0);
	~PrescriptionWizard();

	void setPrescription(PrescriptionRecord *);
	bool getPatient(int);

public slots:
	void changeMedication(int);
	void returnResults();
	void rejectedWizard();

signals:
	void wizardComplete(PrescriptionRecord *);
	void wizardRejected(PrescriptionRecord *);

private:
	Ui::PrescriptionWizard *ui;

	PatientRecord *patient;
	MedicationRecord *medication;
	PrescriptionRecord *prescription;
	ShipmentRecord *shipment;
};

#endif // PRESCRIPTIONWIZARD_H
