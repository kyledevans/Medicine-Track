/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

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
	int temp;
	ui->setupUi(this);
	med = new_med;

	// Add entries to the "form" field
	ui->formBox->addItem("Capsule", QVariant(FORM::Capsule));
	ui->formBox->addItem("Cream", QVariant(FORM::Cream));
	ui->formBox->addItem("Elixir", QVariant(FORM::Elixir));
	ui->formBox->addItem("Ointment", QVariant(FORM::Ointment));
	ui->formBox->addItem("Suspension", QVariant(FORM::Suspension));
	ui->formBox->addItem("Syrup", QVariant(FORM::Syrup));
	ui->formBox->addItem("Tablet", QVariant(FORM::Tablet));

	// Fill in the information into the form if the wizard is modifying an existing record
	if (med->exists) {
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
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(ui->formBox, SIGNAL(currentIndexChanged(int)), this, SLOT(amountFieldCheck(int)));
}

AlterMedicationWizard::~AlterMedicationWizard()
{
    delete ui;
}

void AlterMedicationWizard::returnResults()
{
	med->name = ui->medicationField->text();
	med->generic = ui->genericField->text();
	med->manufacturer = ui->manufacturerField->text();
	med->ndc = ui->ndcField->text();
	med->form = ui->formBox->itemData(ui->formBox->currentIndex()).toInt();
	med->strength = ui->strengthField->text();
	/* write some code that retrieves data from amount field only if 'form' is compatible */
	if ((ui->formBox->currentIndex() == FORM::Elixir) || (ui->formBox->currentIndex() == FORM::Suspension)) {
		med->amount = ui->amountField->text();
	} else {
		med->amount = QString("");
	}
	med->active = ui->activeCheckbox->isChecked();
	med->instructions = ui->instructionsField->toPlainText();

	emit(wizardComplete(med));
}

void AlterMedicationWizard::amountFieldCheck(int index)
{
	if ((index == FORM::Elixir) || (index == FORM::Suspension)) {
		ui->amountLabel->setEnabled(true);
		ui->amountField->setEnabled(true);
	} else {
		ui->amountLabel->setEnabled(false);
		ui->amountField->setEnabled(false);
	}
}
