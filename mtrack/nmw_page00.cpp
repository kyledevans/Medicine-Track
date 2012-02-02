/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QVariant>
#include <QIntValidator>

#include "nmw_page00.h"
#include "ui_nmw_page00.h"

#include "medicationrecord.h"
#include "globals.h"

NMW_Page00::NMW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NMW_Page00)
{
    ui->setupUi(this);

	QValidator *numbers = new QIntValidator(this);
	ui->strengthField->setValidator(numbers);

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

	// Register all the fields
	registerField("medicationField", ui->medicationField);
	registerField("genericField", ui->genericField);
	registerField("manufacturerField", ui->manufacturerField);
	registerField("ndcField", ui->ndcField);
	registerField("formField", ui->formField);
	registerField("strengthField", ui->strengthField);
	registerField("strUnitsField", ui->strUnitsField);
	registerField("amUnitsField", ui->amUnitsField);
	registerField("activeField", ui->activeField);
}

NMW_Page00::~NMW_Page00()
{
    delete ui;
}

void NMW_Page00::setForm(int new_form)
{
	ui->formField->setCurrentIndex(ui->formField->findData(new_form));
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
	med->am_units = ui->amUnitsField->text();
	med->active = ui->activeField->isChecked();
}
