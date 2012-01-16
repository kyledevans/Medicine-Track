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

	qDebug() << "Unfinished method called.  NewPatientWizard::NewPatientWizard(PatientRecord *new_patient, QWidget *parent)";
	// Need to get this working for modifying an existing patient (if at all)

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
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
