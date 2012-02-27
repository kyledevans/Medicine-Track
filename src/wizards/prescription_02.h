/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PRESCRIPTION_02_H
#define PRESCRIPTION_02_H

#include <QWizardPage>

#include "db/prescriptionrecord.h"
#include "medicationrecord.h"

namespace Ui {
	class Prescription_02;
}

class Prescription_02 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Prescription_02(QWidget *parent = 0);
	~Prescription_02();

	void setPrescription(PrescriptionRecord *);
	void setMedication(MedicationRecord *);
	void medUpdated();

	bool initCustom();
	void getResults();

private:
	Ui::Prescription_02 *ui;

	PrescriptionRecord *prescription;
	MedicationRecord *medication;
};

#endif // PRESCRIPTION_02_H
