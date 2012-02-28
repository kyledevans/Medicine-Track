/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescription_02.h"
#include "ui_prescription_02.h"

Prescription_02::Prescription_02(QWidget *parent) :
	QWizardPage(parent),
    ui(new Ui::Prescription_02),
	prescription(0)
{
	ui->setupUi(this);

	// Setup UI strings and tooltips
	ui->instructionsLabel->setText(PrescriptionRecord::instructions_Label);
	ui->instructionsLabel->setToolTip(PrescriptionRecord::instructions_Tooltip);
	ui->instructionsField->setToolTip(PrescriptionRecord::instructions_Tooltip);
}

Prescription_02::~Prescription_02()
{
	delete ui;
}

void Prescription_02::setPrescription(PrescriptionRecord *new_prescription)
{
	prescription = new_prescription;
}

void Prescription_02::setMedication(DrugRecord *new_medication)
{
	medication = new_medication;
}

void Prescription_02::medUpdated()
{
	ui->instructionsField->setPlainText(medication->instructions);
}

void Prescription_02::getResults()
{
	prescription->instructions = ui->instructionsField->toPlainText();
}
