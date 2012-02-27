/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QDebug>

#include "db/patientrecord.h"
#include "patientwizard.h"
#include "ui_patientwizard.h"

PatientWizard::PatientWizard(QWidget *parent) :
	QWizard(parent),
	ui(new Ui::PatientWizard),
	patient(NULL)
{
	ui->setupUi(this);
}

PatientWizard::PatientWizard(PatientRecord *new_patient, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::PatientWizard)
{
	ui->setupUi(this);

	patient = new_patient;

	if (patient->exists) {
		this->setWindowTitle("Alter Patient Wizard");
		setField("mrNumberField", patient->allscripts_id);
		setField("lastField", patient->last);
		setField("firstField", patient->first);
		setField("dobField", patient->dob);
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

PatientWizard::~PatientWizard()
{
	delete ui;
}

void PatientWizard::returnResults()
{
	ui->page0->getResults(patient);
	emit(wizardComplete(patient));
}

void PatientWizard::rejectedWizard()
{
	emit(wizardRejected(patient));
}
