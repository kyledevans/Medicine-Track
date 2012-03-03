/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "db/drugrecord.h"

#ifndef MEDICATION_00_H
#define MEDICATION_00_H

#include <QWizardPage>

namespace Ui {
	class Medication_00;
}

class Medication_00 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Medication_00(QWidget *parent = 0);
	~Medication_00();

	void changeEvent(QEvent *);

	void setForm(int);

    void getResults(DrugRecord *);

private:
	Ui::Medication_00 *ui;

	bool formHasFixedAmount();		// Returns true if the currently selected value in formField should have an amount associated with it
};

#endif // MEDICATION_00_H
