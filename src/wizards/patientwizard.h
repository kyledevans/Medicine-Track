/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "db/patientrecord.h"

#ifndef PATIENTWIZARD_H
#define PATIENTWIZARD_H

#include <QWizard>

namespace Ui {
	class PatientWizard;
}

class PatientWizard : public QWizard
{
	Q_OBJECT

public:
	explicit PatientWizard(PatientRecord *, QWidget *parent = 0);
	~PatientWizard();

	void changeEvent(QEvent *);

public slots:
	void returnResults();
	void rejectedWizard();

signals:
	void wizardComplete(PatientRecord *);	// Note that you must delete the record when you are finished
	void wizardRejected(PatientRecord *);	// Note that you must delete the record when you are finished

private:
	Ui::PatientWizard *ui;
	PatientRecord *patient;
};

#endif // PATIENTWIZARD_H
