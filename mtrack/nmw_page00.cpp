/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QVariant>

#include "nmw_page00.h"
#include "ui_nmw_page00.h"

#include "medicationrecord.h"
#include "globals.h"

NMW_Page00::NMW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NMW_Page00)
{
    ui->setupUi(this);

	// Add entries to the "form" field
	ui->formField->addItem("Capsule", QVariant(FORM_INT::Capsule));
	ui->formField->addItem("Cream", QVariant(FORM_INT::Cream));
	ui->formField->addItem("Drops", QVariant(FORM_INT::Drops));
	ui->formField->addItem("Elixir", QVariant(FORM_INT::Elixir));
	ui->formField->addItem("Ointment", QVariant(FORM_INT::Ointment));
	ui->formField->addItem("Solution", QVariant(FORM_INT::Solution));
	ui->formField->addItem("Suspension", QVariant(FORM_INT::Suspension));
	ui->formField->addItem("Syrup", QVariant(FORM_INT::Syrup));
	ui->formField->addItem("Tablet", QVariant(FORM_INT::Tablet));

	// Makes sure that the "amountField" is only usable when it makes sense
	connect(ui->formField, SIGNAL(currentIndexChanged(int)), this, SLOT(amountFieldCheck(int)));

	registerField("medicationField", ui->medicationField);
	registerField("genericField", ui->genericField);
	registerField("manufacturerField", ui->manufacturerField);
	registerField("ndcField", ui->ndcField);
	registerField("formField", ui->formField);
	registerField("strengthField", ui->strengthField);
	registerField("strUnitsField", ui->strUnitsField);
	registerField("amountField", ui->amountField);
	registerField("amUnitsField", ui->amUnitsField);
	registerField("activeField", ui->activeField);
}

NMW_Page00::~NMW_Page00()
{
    delete ui;
}

void NMW_Page00::setFormAmount(MedicationRecord *med)
{
	int temp;

	temp = ui->formField->findData(QVariant(med->form));
	ui->formField->setCurrentIndex(temp);

	amountFieldCheck();
	if (formHasFixedAmount()) {
		ui->amountField->setText(QString().setNum(med->amount));
		ui->amUnitsField->setText(med->am_units);
	}
}

void NMW_Page00::getResults(MedicationRecord *med)
{
	med->name = ui->medicationField->text();
	med->generic = ui->genericField->text();
	med->manufacturer = ui->manufacturerField->text();
	med->ndc = ui->ndcField->text();
	med->form = ui->formField->itemData(ui->formField->currentIndex()).toInt();
	med->strength = ui->strengthField->text().toInt();
	med->str_units = ui->strUnitsField->text();

	if (formHasFixedAmount()) {	// Only need to get a value from amount if it makes sense
		med->amount = ui->amountField->text().toInt();
		med->am_units = ui->amUnitsField->text();
	} else {
		med->amount = 1;
		med->am_units = QString("");
	}
	med->active = ui->activeField->isChecked();
}

void NMW_Page00::amountFieldCheck(int index)
{
	index++;	// TODO: this makes compile warning about unused variable go away
	if (formHasFixedAmount()) {
		ui->amountLabel->setEnabled(true);
		ui->amountField->setEnabled(true);
		ui->amUnitsLabel->setEnabled(true);
		ui->amUnitsField->setEnabled(true);
	} else {
		ui->amountLabel->setEnabled(false);
		ui->amountField->setEnabled(false);
		ui->amUnitsLabel->setEnabled(false);
		ui->amUnitsField->setEnabled(false);
	}
}

bool NMW_Page00::formHasFixedAmount()
{
	if (ui->formField->itemData(ui->formField->currentIndex()).toInt() == FORM_INT::Elixir) {
		return true;
	}
	if (ui->formField->itemData(ui->formField->currentIndex()).toInt() == FORM_INT::Suspension) {
		return true;
	}
	return false;
}
