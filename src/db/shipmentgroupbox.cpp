/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipmentgroupbox.h"
#include "ui_shipmentgroupbox.h"

#include "drugrecord.h"
#include "../globals.h"

ShipmentGroupBox::ShipmentGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::ShipmentGroupBox)
{
	ui->setupUi(this);

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
}

ShipmentGroupBox::~ShipmentGroupBox()
{
	delete ui;
}

void ShipmentGroupBox::setId(int new_id)
{
	QString temp;
	DrugRecord drug;

	shipment.retrieve(new_id);	// TODO: Does not handle query failure
	drug.retrieve(shipment.getDrug_id());

	// Set values from shipment record
	ui->expirationLabel->setText(shipment.getExpiration().toString(DEFAULTS::DateDisplayFormat));
	ui->lotLabel->setText(shipment.getLot());
	ui->countLabel->setText(QString().setNum(shipment.getProduct_count()) + " " + drug.getDispense_units());
	ui->leftLabel->setText(QString().setNum(shipment.getProduct_left()) + " " + drug.getDispense_units());
	if (shipment.getActive()) {
		ui->activeLabel->setText("Active");
	} else {
		temp = QString("<font color='#%1'>%2</font>");
		ui->activeLabel->setText(temp.arg(MTCOLORS::Problem, QString("Inactive")));
	}
}

int ShipmentGroupBox::getId()
{
	return shipment.getId();
}

int ShipmentGroupBox::getDrug_id()
{
	return shipment.getDrug_id();
}

void ShipmentGroupBox::changeEvent(QEvent *e)
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
