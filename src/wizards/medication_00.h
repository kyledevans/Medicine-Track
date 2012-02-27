/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef MEDICATION_00_H
#define MEDICATION_00_H

#include <QWizardPage>

#include "medicationrecord.h"

namespace Ui {
	class Medication_00;
}

class Medication_00 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Medication_00(QWidget *parent = 0);
	~Medication_00();

	void setForm(int);

	void getResults(MedicationRecord *);

private:
	Ui::Medication_00 *ui;

	bool formHasFixedAmount();		// Returns true if the currently selected value in formField should have an amount associated with it
};

#endif // MEDICATION_00_H
