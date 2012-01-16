/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "npw_page00.h"
#include "ui_npw_page00.h"

#include "patientrecord.h"

NPW_Page00::NPW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NPW_Page00)
{
    ui->setupUi(this);

	registerField("mrNumberField", ui->mrNumberField);
	registerField("lastField", ui->lastField);
	registerField("firstField", ui->firstField);
	registerField("dobField", ui->dobField);
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
