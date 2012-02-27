/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacist_00.h"
#include "ui_pharmacist_00.h"

#include "pharmacistrecord.h"

Pharmacist_00::Pharmacist_00(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::Pharmacist_00)
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

Pharmacist_00::~Pharmacist_00()
{
	delete ui;
}

void Pharmacist_00::getResults(PharmacistRecord *pharm)
{
	pharm->last = ui->lastField->text();
	pharm->first = ui->firstField->text();
	pharm->initials = ui->initialsField->text();
}
