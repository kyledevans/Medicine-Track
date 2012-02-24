/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "apharmp_page00.h"
#include "ui_apharmp_page00.h"

#include "pharmacistrecord.h"

APharmP_Page00::APharmP_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::APharmP_Page00)
{
    ui->setupUi(this);

	// Setup UI strings and tooltips
	ui->lastLabel->setText(PharmacistRecord::last_Label);
	ui->lastLabel->setToolTip(PharmacistRecord::last_Tooltip);
	ui->lastField->setToolTip(PharmacistRecord::last_Tooltip);

	ui->firstLabel->setText(PharmacistRecord::first_Label);
	ui->firstLabel->setToolTip(PharmacistRecord::first_Tooltip);
	ui->firstField->setToolTip(PharmacistRecord::first_Tooltip);

	ui->initialsLabel->setText(PharmacistRecord::initials_Label);
	ui->initialsLabel->setToolTip(PharmacistRecord::initials_Tooltip);
	ui->initialsField->setToolTip(PharmacistRecord::initials_Tooltip);

	registerField("lastField", ui->lastField);
	registerField("firstField", ui->firstField);
	registerField("initialsField", ui->initialsField);
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
}
