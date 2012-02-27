/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef MEDICATIONWIZARD_H
#define MEDICATIONWIZARD_H

#include <QWizard>

#include "medicationrecord.h"

namespace Ui {
	class MedicationWizard;
}

class MedicationWizard : public QWizard
{
	Q_OBJECT

public:
	explicit MedicationWizard(QWidget *parent = 0);
	explicit MedicationWizard(MedicationRecord *, QWidget *parent = 0);
	~MedicationWizard();

public slots:
	void returnResults();
	void rejectedWizard();

signals:
	void wizardComplete(MedicationRecord *);
	void wizardRejected(MedicationRecord *);

private:
	Ui::MedicationWizard *ui;

	MedicationRecord *med;
};

#endif // MEDICATIONWIZARD_H
