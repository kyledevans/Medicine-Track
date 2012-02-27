/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "medication_01.h"
#include "ui_medication_01.h"

#include "medicationrecord.h"

Medication_01::Medication_01(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::Medication_01)
{
	ui->setupUi(this);

	// Setup UI strings and tooltips
	ui->instructionsField->setToolTip(MedicationRecord::instructions_Tooltip);

	registerField("instructionsField", ui->instructionsField, "plainText", SIGNAL(textChanged()));
}

Medication_01::~Medication_01()
{
	delete ui;
}

void Medication_01::getResults(MedicationRecord *med)
{
	med->instructions = ui->instructionsField->toPlainText();
}
