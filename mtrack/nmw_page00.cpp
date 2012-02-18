/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QVariant>
#include <QDoubleValidator>

#include "nmw_page00.h"
#include "ui_nmw_page00.h"

#include "medicationrecord.h"
#include "globals.h"

NMW_Page00::NMW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NMW_Page00)
{
    ui->setupUi(this);

	// Set the various UI strings and tooltips
	ui->medicationLabel->setText(MedicationRecord::name_Label);
	ui->medicationLabel->setToolTip(MedicationRecord::name_Tooltip);
	ui->medicationField->setToolTip(MedicationRecord::name_Tooltip);

	ui->genericLabel->setText(MedicationRecord::generic_Label);
	ui->genericLabel->setToolTip(MedicationRecord::generic_Tooltip);
	ui->genericField->setToolTip(MedicationRecord::generic_Tooltip);

	ui->manufacturerLabel ->setText(MedicationRecord::manufacturer_Label);
	ui->manufacturerLabel->setToolTip(MedicationRecord::manufacturer_Tooltip);
	ui->manufacturerField->setToolTip(MedicationRecord::manufacturer_Tooltip);

	ui->ndcLabel->setText(MedicationRecord::ndc_Label);
	ui->ndcLabel->setToolTip(MedicationRecord::ndc_Tooltip);
	ui->ndcField->setToolTip(MedicationRecord::ndc_Tooltip);

	ui->strengthLabel->setText(MedicationRecord::strength_Label);
	ui->strengthLabel->setToolTip(MedicationRecord::strength_Tooltip);
    ui->strengthField->setToolTip(MedicationRecord::strength_Tooltip_edit);

	ui->strUnitsLabel->setText(MedicationRecord::str_units_Label);
	ui->strUnitsLabel->setToolTip(MedicationRecord::str_units_Tooltip);
	ui->strUnitsField->setToolTip(MedicationRecord::str_units_Tooltip);

	ui->formLabel->setText(MedicationRecord::form_Label);
	ui->formLabel->setToolTip(MedicationRecord::form_Tooltip);
	ui->formField->setToolTip(MedicationRecord::form_Tooltip);

	ui->dispenseLabel->setText(MedicationRecord::dispense_units_Label);
	ui->dispenseLabel->setToolTip(MedicationRecord::dispense_units_Tooltip);
	ui->dispenseUnitsField->setToolTip(MedicationRecord::dispense_units_Tooltip);

	ui->unitSizeLabel->setText(MedicationRecord::unit_size_Label);
	ui->unitSizeLabel->setToolTip(MedicationRecord::unit_size_Tooltip);
	ui->unitSizeField->setToolTip(MedicationRecord::unit_size_Tooltip);

	// Set validators
	QValidator *numbers = new QDoubleValidator(this);
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
	registerField("dispenseUnitsField", ui->dispenseUnitsField);
	registerField("unitSizeField", ui->unitSizeField);
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
	med->strength = ui->strengthField->text().toDouble();
	med->str_units = ui->strUnitsField->text();
	med->dispense_units = ui->dispenseUnitsField->text();
	med->unit_size = ui->unitSizeField->text();
	med->active = ui->activeField->isChecked();
}
