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

	setStrings();
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

void ShipmentGroupBox::setStrings()
{
	// Set UI strings for Shipment group
	ui->expirationTitle->setText(ShipmentRecord::expiration_Label + ":");
	ui->lotTitle->setText(ShipmentRecord::lot_Label + ":");
	ui->countTitle->setText(ShipmentRecord::product_count_Label + ":");
	ui->leftTitle->setText(ShipmentRecord::product_left_Label + ":");
	ui->activeTitle->setText(ShipmentRecord::active_Label + ":");
}

void ShipmentGroupBox::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		setStrings();
		break;
	default:
		break;
	}
}
