/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

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

	// Fill out the details if wizard is in modify mode
	if (patient->exists) {
		ui->lastField->setText(patient->last);
		ui->firstField->setText(patient->first);
		ui->dobField->setDate(patient->dob);
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
}

NewPatientWizard::~NewPatientWizard()
{
    delete ui;
}

void NewPatientWizard::returnResults()
{
	patient->first = ui->firstField->text();
	patient->last = ui->lastField->text();
	patient->dob = ui->dobField->date();
	emit(wizardComplete(patient));
}
