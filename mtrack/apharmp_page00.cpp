/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "apharmp_page00.h"
#include "ui_apharmp_page00.h"

APharmP_Page00::APharmP_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::APharmP_Page00)
{
    ui->setupUi(this);

	registerField("lastField", ui->lastField);
	registerField("firstField", ui->firstField);
	registerField("initialsField", ui->initialsField);
	registerField("activeField", ui->activeField);
}

APharmP_Page00::~APharmP_Page00()
{
    delete ui;
}

void APharmP_Page00::getResults(PharmacistRecord *pharm)
{
	pharm->last = ui->lastField->text();
	pharm->first = ui->firstField->text();
	pharm->initials = ui->initialsField->text();
	pharm->active = ui->activeField->isChecked();
}
