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
	ui->instructionsField->setPlainText(medication->getInstructions());
}

void Prescription_02::getResults()
{
	prescription->setInstructions(ui->instructionsField->toPlainText());
}

void Prescription_02::changeEvent(QEvent *e)
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
