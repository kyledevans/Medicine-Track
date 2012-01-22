/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "altermedicationwizard.h"
#include "ui_altermedicationwizard.h"

#include "globals.h"

#include <QDebug>

AlterMedicationWizard::AlterMedicationWizard(QWidget *parent) :
    QWizard(parent),
	ui(new Ui::AlterMedicationWizard),
	med(NULL)
{
    ui->setupUi(this);
}

AlterMedicationWizard::AlterMedicationWizard(MedicationRecord *new_med, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::AlterMedicationWizard)
{
	//int temp;
	ui->setupUi(this);
	med = new_med;

	// Fill in the information into the form if the wizard is modifying an existing record
	/*if (med->exists) {

		// TODO: This will probably need to go elsewhere, but will be needed sometime
		// Code that fills in information when modifying an existing medication
		ui->medicationField->setText(med->name);
		ui->genericField->setText(med->generic);
		ui->manufacturerField->setText(med->manufacturer);
		ui->ndcField->setText(med->ndc);
		temp = ui->formBox->findData(QVariant(med->form));
		ui->formBox->setCurrentIndex(temp);
		ui->strengthField->setText(med->strength);
		ui->amountField->setText(med->amount);
		ui->activeCheckbox->setChecked(med->active);
		ui->instructionsField->setPlainText(med->instructions);

		if ((med->form == FORM::Elixir) || (med->form == FORM::Suspension)) {
			ui->amountLabel->setEnabled(true);
			ui->amountField->setEnabled(true);
		}
	}*/

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	//connect(ui->formBox, SIGNAL(currentIndexChanged(int)), this, SLOT(amountFieldCheck(int)));	TODO: This needs to be re-implemented
}

AlterMedicationWizard::~AlterMedicationWizard()
{
    delete ui;
}

void AlterMedicationWizard::returnResults()
{
	ui->page0->getResults(med);
	ui->page1->getResults(med);
	emit(wizardComplete(med));
}
