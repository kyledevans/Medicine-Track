/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "drugrecord.h"

#ifndef MEDICATIONWIZARD_H
#define MEDICATIONWIZARD_H

#include <QWizard>

namespace Ui {
	class MedicationWizard;
}

class MedicationWizard : public QWizard
{
	Q_OBJECT
public:
	explicit MedicationWizard(DrugRecord *, QWidget *parent = 0);
	~MedicationWizard();

	void changeEvent(QEvent *);

public slots:
	void returnResults();
	void rejectedWizard();

signals:
	void wizardComplete(DrugRecord *);
	void wizardRejected(DrugRecord *);

private:
	Ui::MedicationWizard *ui;

	DrugRecord *med;
};

#endif // MEDICATIONWIZARD_H
