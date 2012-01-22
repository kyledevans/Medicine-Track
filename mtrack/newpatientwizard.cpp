/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QDebug>

#include "patientrecord.h"
#include "newpatientwizard.h"
#include "ui_newpatientwizard.h"

NewPatientWizard::NewPatientWizard(QWidget *parent) :
    QWizard(parent),
	ui(new Ui::NewPatientWizard),
	patient(NULL)
{
    ui->setupUi(this);
}

NewPatientWizard::NewPatientWizard(PatientRecord *new_patient, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::NewPatientWizard)
{
	ui->setupUi(this);

	patient = new_patient;

	if (patient->exists) {
		setField("mrNumberField", patient->allscripts_id);
		setField("lastField", patient->last);
		setField("firstField", patient->first);
		setField("dobField", patient->dob);
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

NewPatientWizard::~NewPatientWizard()
{
    delete ui;
}

void NewPatientWizard::returnResults()
{
	ui->page00->getResults(patient);
	emit(wizardComplete(patient));
}

void NewPatientWizard::rejectedWizard()
{
	emit(wizardRejected(patient));
}
