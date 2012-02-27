/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef MEDICATION_01_H
#define MEDICATION_01_H

#include <QWizardPage>

#include "medicationrecord.h"

namespace Ui {
	class Medication_01;
}

class Medication_01 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Medication_01(QWidget *parent = 0);
	~Medication_01();

	void getResults(MedicationRecord *);

private:
	Ui::Medication_01 *ui;
};

#endif // MEDICATION_01_H
