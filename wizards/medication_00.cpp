/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "medication_00.h"
#include "ui_medication_00.h"

#include <QVariant>
#include <QDoubleValidator>

#include "../db/globals.h"

Medication_00::Medication_00(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::Medication_00)
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
	ui->formField->setCurrentIndex(-1);

	// Register all the fields
	registerField("medicationField*", ui->medicationField);
	registerField("genericField", ui->genericField);
	registerField("manufacturerField*", ui->manufacturerField);
	registerField("ndcField*", ui->ndcField);
	registerField("formField*", ui->formField);
	registerField("strengthField*", ui->strengthField);
	registerField("dispenseUnitsField*", ui->dispenseUnitsField);
	registerField("unitSizeField", ui->unitSizeField);
}

Medication_00::~Medication_00()
{
	delete ui;
}

void Medication_00::setForm(int new_form)
{
	ui->formField->setCurrentIndex(ui->formField->findData(new_form));
}

void Medication_00::getResults(DrugRecord *med)
{
	med->setName(ui->medicationField->text());
	med->setGeneric(ui->genericField->text());
	med->setManufacturer(ui->manufacturerField->text());
	med->setNdc(ui->ndcField->text());
	med->setForm(ui->formField->itemData(ui->formField->currentIndex()).toInt());
	med->setStrength(ui->strengthField->text());
	med->setDispense_units(ui->dispenseUnitsField->text());
	med->setUnit_size(ui->unitSizeField->text());
}

void Medication_00::changeEvent(QEvent *e)
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
