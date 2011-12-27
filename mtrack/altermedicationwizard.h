/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALTERMEDICATIONWIZARD_H
#define ALTERMEDICATIONWIZARD_H

#include <QWizard>

#include "medicationrecord.h"

namespace Ui {
    class AlterMedicationWizard;
}

class AlterMedicationWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AlterMedicationWizard(QWidget *parent = 0);
	explicit AlterMedicationWizard(MedicationRecord *, QWidget *parent = 0);
    ~AlterMedicationWizard();

public slots:
	void returnResults();

signals:
	void wizardComplete(MedicationRecord *);	// Note that you must delete the record when finished

private:
    Ui::AlterMedicationWizard *ui;

	MedicationRecord *med;
};

#endif // ALTERMEDICATIONWIZARD_H
