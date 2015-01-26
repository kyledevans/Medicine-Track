/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipmentgroupbox.h"
#include "ui_shipmentgroupbox.h"

#include "drugrecord.h"
#include "globals.h"

ShipmentGroupBox::ShipmentGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::ShipmentGroupBox)
{
	ui->setupUi(this);
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
	if (shipment.getExpiration() >= QDate::currentDate()) {
		ui->expirationLabel->setText(shipment.getExpiration().toString(DEFAULTS::DateDisplayFormat));
	} else {
		temp = QString("<font color='#%1'>%2</font>");
		ui->expirationLabel->setText(temp.arg(MTCOLORS::Problem, shipment.getExpiration().toString(DEFAULTS::DateDisplayFormat)));
	}
	ui->lotLabel->setText(shipment.getLot());
	ui->countLabel->setText(QString().setNum(shipment.getProduct_count()) + " " + drug.getDispense_units());
	if (shipment.getProduct_left() > 0) {	// Items are still in stock
		ui->leftLabel->setText(QString().setNum(shipment.getProduct_left()) + " " + drug.getDispense_units());
	} else {	// No units in stock, make it red
		temp = QString("<font color='#%1'>%2</font>");
		ui->leftLabel->setText(temp.arg(MTCOLORS::Problem, QString().setNum(shipment.getProduct_left())));
	}
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
