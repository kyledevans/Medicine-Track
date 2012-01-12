/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef NEWPATIENTWIZARD_H
#define NEWPATIENTWIZARD_H

#include <QWizard>

#include "patientrecord.h"

namespace Ui {
    class NewPatientWizard;
}

class NewPatientWizard : public QWizard
{
    Q_OBJECT

public:
    explicit NewPatientWizard(QWidget *parent = 0);
	explicit NewPatientWizard(PatientRecord *, QWidget *parent = 0);
    ~NewPatientWizard();

public slots:
	void returnResults();

signals:
	void wizardComplete(PatientRecord *);	// Note that you must delete the record when you are finished

private:
    Ui::NewPatientWizard *ui;
	PatientRecord *patient;
};

#endif // NEWPATIENTWIZARD_H
