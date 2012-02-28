/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipment_00.h"
#include "ui_shipment_00.h"

#include <QIntValidator>

#include "globals.h"

Shipment_00::Shipment_00(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::Shipment_00)
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

Shipment_00::~Shipment_00()
{
	delete ui;
}

void Shipment_00::setMedication(DrugRecord *med)
{
	// Show medication that was selected
	ui->medicationLabel->setText(med->getName());
	ui->strengthLabel->setText(med->getStrength());
	ui->formLabel->setText(FORM_STR::intToStr(med->getForm()));
	ui->unitSizeLabel->setText(med->getUnit_size());
	ui->dispenseUnitLabel->setText(med->getDispense_units());
}

void Shipment_00::getResults(ShipmentRecord *shipment)
{
	shipment->setExpiration(ui->expireField->date());
	shipment->setLot(ui->lotField->text());
	shipment->setProduct_count(ui->unitsField->text().toInt());
	shipment->setProduct_left(shipment->getProduct_count());
	shipment->setWrite_off(0);
}
