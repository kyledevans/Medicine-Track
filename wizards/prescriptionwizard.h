/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptionrecord.h"
#include "patientrecord.h"
#include "drugrecord.h"
#include "shipmentrecord.h"

#ifndef PRESCRIPTIONWIZARD_H
#define PRESCRIPTIONWIZARD_H

#include <QWizard>

namespace Ui {
	class PrescriptionWizard;
}

class PrescriptionWizard : public QWizard
{
	Q_OBJECT
public:
	explicit PrescriptionWizard(QWidget *parent = 0);
	~PrescriptionWizard();

	void changeEvent(QEvent *);

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

	PrescriptionRecord *prescription;
	PatientRecord *patient;
	DrugRecord *medication;
	ShipmentRecord *shipment;
};

#endif // PRESCRIPTIONWIZARD_H
