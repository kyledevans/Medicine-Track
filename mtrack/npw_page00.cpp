/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "npw_page00.h"
#include "ui_npw_page00.h"

#include "patientrecord.h"
#include "globals.h"

NPW_Page00::NPW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NPW_Page00)
{
    ui->setupUi(this);

	// Setup UI strings and tooltips
	ui->mrNumberLabel->setText(PatientRecord::allscripts_id_Label);
	ui->mrNumberLabel->setToolTip(PatientRecord::allscripts_id_Tooltip);
	ui->mrNumberField->setToolTip(PatientRecord::allscripts_id_Tooltip);

	ui->lastLabel->setText(PatientRecord::last_Label);
	ui->lastLabel->setToolTip(PatientRecord::last_Tooltip);
	ui->lastField->setToolTip(PatientRecord::last_Tooltip);

	ui->firstLabel->setText(PatientRecord::first_Label);
	ui->firstLabel->setToolTip(PatientRecord::first_Tooltip);
	ui->firstField->setToolTip(PatientRecord::first_Tooltip);

	ui->dobLabel->setText(PatientRecord::dob_Label);
	ui->dobLabel->setToolTip(PatientRecord::dob_Tooltip);
	ui->dobField->setToolTip(PatientRecord::dob_Tooltip);

	ui->dobField->setDate(DEFAULTS::Date);

	registerField("mrNumberField*", ui->mrNumberField);
	registerField("lastField*", ui->lastField);
	registerField("firstField*", ui->firstField);
	registerField("dobField*", ui->dobField);
}

NPW_Page00::~NPW_Page00()
{
    delete ui;
}

void NPW_Page00::getResults(PatientRecord *patient)
{
	patient->allscripts_id = field("mrNumberField").toInt();
	patient->last = field("lastField").toString();
	patient->first = field("firstField").toString();
	patient->dob = field("dobField").toDate();
}
