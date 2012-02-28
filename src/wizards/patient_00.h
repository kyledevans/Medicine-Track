/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "db/patientrecord.h"

#ifndef PATIENT_00_H
#define PATIENT_00_H

#include <QWizardPage>

namespace Ui {
	class Patient_00;
}

class Patient_00 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Patient_00(QWidget *parent = 0);
	~Patient_00();

	void getResults(PatientRecord *);

private:
	Ui::Patient_00 *ui;
};

#endif // PATIENT_00_H
