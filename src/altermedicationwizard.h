/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

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
	void rejectedWizard();

signals:
	void wizardComplete(MedicationRecord *);
	void wizardRejected(MedicationRecord *);

private:
    Ui::AlterMedicationWizard *ui;

	MedicationRecord *med;
};

#endif // ALTERMEDICATIONWIZARD_H
