/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patient_00.h"
#include "ui_patient_00.h"

#include "../db/globals.h"

Patient_00::Patient_00(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::Patient_00)
{
	ui->setupUi(this);

	ui->dobField->setDate(DEFAULTS::Date);

	registerField("mrNumberField*", ui->mrNumberField);
	registerField("lastField*", ui->lastField);
	registerField("firstField*", ui->firstField);
	registerField("dobField*", ui->dobField);
}

Patient_00::~Patient_00()
{
	delete ui;
}

void Patient_00::getResults(PatientRecord *patient)
{
	patient->setAllscripts_id(field("mrNumberField").toInt());
	patient->setLast(field("lastField").toString());
	patient->setFirst(field("firstField").toString());
	patient->setDob(field("dobField").toDate());
}

void Patient_00::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
