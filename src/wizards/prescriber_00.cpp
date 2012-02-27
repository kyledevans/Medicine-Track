/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriber_00.h"
#include "ui_prescriber_00.h"

Prescriber_00::Prescriber_00(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::Prescriber_00)
{
	ui->setupUi(this);

	// Setup UI strings and tooltips
	ui->lastLabel->setText(PrescriberRecord::last_Label);
	ui->lastLabel->setToolTip(PrescriberRecord::last_Tooltip);
	ui->lastField->setToolTip(PrescriberRecord::last_Tooltip);

	ui->firstLabel->setText(PrescriberRecord::first_Label);
	ui->firstLabel->setToolTip(PrescriberRecord::first_Tooltip);
	ui->firstField->setToolTip(PrescriberRecord::first_Tooltip);

	ui->fullLabel->setText(PrescriberRecord::full_name_Label);
	ui->fullLabel->setToolTip(PrescriberRecord::full_name_Tooltip);
	ui->fullField->setToolTip(PrescriberRecord::full_name_Tooltip);

	registerField("lastField", ui->lastField);
	registerField("firstField", ui->firstField);
	registerField("fullField", ui->fullField);
}

Prescriber_00::~Prescriber_00()
{
	delete ui;
}

void Prescriber_00::getResults(PrescriberRecord *pres)
{
	pres->last = ui->lastField->text();
	pres->first = ui->firstField->text();
	pres->full_name = ui->fullField->text();
}
