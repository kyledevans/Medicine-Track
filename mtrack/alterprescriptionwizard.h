/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALTERPRESCRIPTIONWIZARD_H
#define ALTERPRESCRIPTIONWIZARD_H

#include <QWizard>

#include "prescriptionrecord.h"
#include "patientrecord.h"
#include "medicationrecord.h"
#include "shipmentrecord.h"

namespace Ui {
    class AlterPrescriptionWizard;
}

class AlterPrescriptionWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AlterPrescriptionWizard(QWidget *parent = 0);
    ~AlterPrescriptionWizard();

	void setPatient(PatientRecord *);
	void setMedication(MedicationRecord *);
	void setPrescription(PrescriptionRecord *);
	void setShipment(ShipmentRecord *);

private:
    Ui::AlterPrescriptionWizard *ui;

	PatientRecord *patient;
	MedicationRecord *medication;
	PrescriptionRecord *prescription;
	ShipmentRecord *shipment;
};

#endif // ALTERPRESCRIPTIONWIZARD_H
