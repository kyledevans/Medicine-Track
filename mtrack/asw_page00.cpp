/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QIntValidator>

#include "asw_page00.h"
#include "ui_asw_page00.h"

#include "globals.h"

ASW_Page00::ASW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ASW_Page00)
{
    ui->setupUi(this);

	// Set UI strings and tooltips
	ui->expireLabel->setText(ShipmentRecord::expiration_Label);
	ui->expireLabel->setToolTip(ShipmentRecord::expiration_Tooltip);
	ui->expireField->setToolTip(ShipmentRecord::expiration_Tooltip);

	ui->lotLabel->setText(ShipmentRecord::lot_Label);
	ui->lotLabel->setToolTip(ShipmentRecord::lot_Tooltip);
	ui->lotField->setToolTip(ShipmentRecord::lot_Tooltip);

	ui->unitsLabel->setText(ShipmentRecord::product_count_Label);
	ui->unitsLabel->setToolTip(ShipmentRecord::product_count_Tooltip);
	ui->unitsField->setToolTip(ShipmentRecord::product_count_Tooltip);

	// Setup validators
	QValidator *numbers = new QIntValidator(this);
	ui->unitsField->setValidator(numbers);

	// Set default values
	ui->expireField->setDate(DEFAULTS::Date);

	// Register fields
	registerField("expireField*", ui->expireField);
	registerField("lotField*", ui->lotField);
	registerField("unitsField*", ui->unitsField);
}

ASW_Page00::~ASW_Page00()
{
    delete ui;
}

void ASW_Page00::setMedication(MedicationRecord *med)
{
	// Show medication that was selected
	ui->medicationLabel->setText(med->name);
	ui->strengthLabel->setText(med->strength);
	ui->formLabel->setText(FORM_STR::intToStr(med->form));
	ui->unitSizeLabel->setText(med->unit_size);
	ui->dispenseUnitLabel->setText(med->dispense_units);
}

void ASW_Page00::getResults(ShipmentRecord *shipment)
{
	shipment->expiration = ui->expireField->date();
	shipment->lot = ui->lotField->text();
	shipment->product_count = ui->unitsField->text().toInt();
    shipment->product_left = shipment->product_count;
    shipment->write_off = 0;
}
