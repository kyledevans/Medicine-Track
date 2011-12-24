/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientrecord.h"
#include "newpatientwizard.h"
#include "ui_newpatientwizard.h"

NewPatientWizard::NewPatientWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::NewPatientWizard)
{
    ui->setupUi(this);

	/*ui->page1->registerField("firstField*", ui->firstField);
	ui->page1->registerField("lastField*", ui->lastField);
	ui->page1->registerField("dobField*", ui->dobField);*/

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
}

NewPatientWizard::~NewPatientWizard()
{
    delete ui;
}

void NewPatientWizard::returnResults()
{
	PatientRecord *new_record = new PatientRecord;
	new_record->first = ui->firstField->text();
	new_record->last = ui->lastField->text();
	new_record->dob = ui->dobField->date();
	emit(wizardComplete(new_record));
}
