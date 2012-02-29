/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipmentdisplay.h"
#include "ui_shipmentdisplay.h"

#include "db/drugrecord.h"
#include "../globals.h"

ShipmentDisplay::ShipmentDisplay(int new_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ShipmentDisplay)
{
	QString temp;
	DrugRecord drug;
	ui->setupUi(this);

	shipment.retrieve(new_id);	// TODO: Does not handle query failure
	drug.retrieve(shipment.getDrug_id());
	setAttribute(Qt::WA_DeleteOnClose);

	// Set UI strings for Shipment group
	ui->expirationTitle->setText(ShipmentRecord::expiration_Label + ":");
	ui->expirationTitle->setToolTip(ShipmentRecord::expiration_Tooltip);
	ui->expirationLabel->setToolTip(ShipmentRecord::expiration_Tooltip);

	ui->lotTitle->setText(ShipmentRecord::lot_Label + ":");
	ui->lotTitle->setToolTip(ShipmentRecord::lot_Tooltip);
	ui->lotLabel->setToolTip(ShipmentRecord::lot_Tooltip);

	ui->countTitle->setText(ShipmentRecord::product_count_Label + ":");
	ui->countTitle->setToolTip(ShipmentRecord::product_count_Tooltip);
	ui->countLabel->setToolTip(ShipmentRecord::product_count_Tooltip);

	ui->leftTitle->setText(ShipmentRecord::product_left_Label + ":");
	ui->leftTitle->setToolTip(ShipmentRecord::product_left_Tooltip);
	ui->leftLabel->setToolTip(ShipmentRecord::product_left_Tooltip);

	ui->activeTitle->setText(ShipmentRecord::active_Label + ":");
	ui->activeTitle->setToolTip(ShipmentRecord::active_Tooltip);
	ui->activeLabel->setToolTip(ShipmentRecord::active_Tooltip);

	// Set UI strings for Drug group
	ui->medicationTitle->setText(DrugRecord::name_Label + ":");
	ui->medicationTitle->setToolTip(DrugRecord::name_Tooltip);
	ui->medicationLabel->setToolTip(DrugRecord::name_Tooltip);

	ui->genericTitle->setText(DrugRecord::generic_Label + ":");
	ui->genericTitle->setToolTip(DrugRecord::generic_Tooltip);
	ui->genericLabel->setToolTip(DrugRecord::generic_Tooltip);

	ui->manufacturerTitle->setText(DrugRecord::manufacturer_Label + ":");
	ui->manufacturerTitle->setToolTip(DrugRecord::manufacturer_Tooltip);
	ui->manufacturerLabel->setToolTip(DrugRecord::manufacturer_Tooltip);

	ui->ndcTitle->setText(DrugRecord::ndc_Label + ":");
	ui->ndcTitle->setToolTip(DrugRecord::ndc_Tooltip);
	ui->ndcLabel->setToolTip(DrugRecord::ndc_Tooltip);

	ui->formTitle->setText(DrugRecord::form_Label + ":");
	ui->formTitle->setToolTip(DrugRecord::form_Label);
	ui->formLabel->setToolTip(DrugRecord::form_Tooltip);

	ui->strengthTitle->setText(DrugRecord::strength_Label + ":");
	ui->strengthTitle->setToolTip(DrugRecord::strength_Tooltip);
	ui->strengthLabel->setToolTip(DrugRecord::strength_Tooltip);

	ui->dispenseTitle->setText(DrugRecord::dispense_units_Label + ":");
	ui->dispenseTitle->setToolTip(DrugRecord::dispense_units_Tooltip);
	ui->dispenseLabel->setToolTip(DrugRecord::dispense_units_Tooltip);

	ui->unitTitle->setText(DrugRecord::unit_size_Label + ":");
	ui->unitTitle->setToolTip(DrugRecord::unit_size_Tooltip);
	ui->unitLabel->setToolTip(DrugRecord::unit_size_Tooltip);

	ui->instructionsTitle->setText(DrugRecord::instructions_Label + ":");
	ui->instructionsTitle->setToolTip(DrugRecord::instructions_Tooltip);
	ui->instructionsLabel->setToolTip(DrugRecord::instructions_Tooltip);

	ui->activeTitle_2->setText(DrugRecord::active_Label + ":");
	ui->activeTitle_2->setToolTip(DrugRecord::active_Tooltip);
	ui->activeLabel_2->setToolTip(DrugRecord::active_Tooltip);

	// Set values from shipment record
	ui->expirationLabel->setText(shipment.getExpiration().toString(DEFAULTS::DateDisplayFormat));
	ui->lotLabel->setText(shipment.getLot());
	ui->countLabel->setText(QString().setNum(shipment.getProduct_count()));
	ui->leftLabel->setText(QString().setNum(shipment.getProduct_left()));
	if (shipment.getActive()) {
		ui->activeLabel->setText("Active");
	} else {
		temp = QString("<font color='#%1'>%2</font>");
		ui->activeLabel->setText(temp.arg(MTCOLORS::Problem, QString("Inactive")));
	}

	// Set values from drug record
	ui->medicationLabel->setText(drug.getName());
	ui->genericLabel->setText(drug.getGeneric());
	ui->manufacturerLabel->setText(drug.getManufacturer());
	ui->ndcLabel->setText(drug.getNdc());
	ui->formLabel->setText(FORM_STR::intToStr(drug.getForm()));
	ui->strengthLabel->setText(drug.getStrength());
	ui->dispenseLabel->setText(drug.getDispense_units());
	ui->unitLabel->setText(drug.getUnit_size());
	ui->instructionsLabel->setText(drug.getInstructions());
	if (drug.getActive()) {
		ui->activeLabel_2->setText("Active");
	} else {
		temp = QString("<font color='#%1'>%2</font>");
		ui->activeLabel_2->setText(temp.arg(MTCOLORS::Problem, QString("Inactive")));
	}

	this->show();
}

ShipmentDisplay::~ShipmentDisplay()
{
	delete ui;
}
