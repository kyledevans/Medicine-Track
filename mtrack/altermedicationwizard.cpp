/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "altermedicationwizard.h"
#include "ui_altermedicationwizard.h"

#include "globals.h"

AlterMedicationWizard::AlterMedicationWizard(QWidget *parent) :
    QWizard(parent),
	ui(new Ui::AlterMedicationWizard),
	med(NULL)
{
    ui->setupUi(this);

	ui->formBox->addItem("Capsule", QVariant(FORM::Capsule));
	ui->formBox->addItem("Cream", QVariant(FORM::Cream));
	ui->formBox->addItem("Elixir", QVariant(FORM::Elixir));
	ui->formBox->addItem("Ointment", QVariant(FORM::Ointment));
	ui->formBox->addItem("Suspension", QVariant(FORM::Suspension));
	ui->formBox->addItem("Syrup", QVariant(FORM::Syrup));
	ui->formBox->addItem("Tablet", QVariant(FORM::Tablet));

	//connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
}

AlterMedicationWizard::AlterMedicationWizard(MedicationRecord *new_med, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::AlterMedicationWizard)
{
	int temp;
	ui->setupUi(this);
	med = new_med;

	ui->formBox->addItem("Capsule", QVariant(FORM::Capsule));
	ui->formBox->addItem("Cream", QVariant(FORM::Cream));
	ui->formBox->addItem("Elixir", QVariant(FORM::Elixir));
	ui->formBox->addItem("Ointment", QVariant(FORM::Ointment));
	ui->formBox->addItem("Suspension", QVariant(FORM::Suspension));
	ui->formBox->addItem("Syrup", QVariant(FORM::Syrup));
	ui->formBox->addItem("Tablet", QVariant(FORM::Tablet));

	// Fill in the information into the form if the wizard is modifying an existing record
	if (med->exists) {
		// TODO
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
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
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
	med->active = ui->activeCheckbox->isChecked();
	med->instructions = ui->instructionsField->toPlainText();

	emit(wizardComplete(med));
}
