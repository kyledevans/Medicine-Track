/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptionrecord.h"
#include "drugrecord.h"

#ifndef PRESCRIPTION_02_H
#define PRESCRIPTION_02_H

#include <QWizardPage>

namespace Ui {
	class Prescription_02;
}

class Prescription_02 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Prescription_02(QWidget *parent = 0);
	~Prescription_02();

	void changeEvent(QEvent *);

	void setPrescription(PrescriptionRecord *);
    void setMedication(DrugRecord *);
	void medUpdated();

	bool initCustom();
	void getResults();

private:
	Ui::Prescription_02 *ui;

	PrescriptionRecord *prescription;
    DrugRecord *medication;
};

#endif // PRESCRIPTION_02_H
