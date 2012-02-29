/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipmentdisplay.h"
#include "ui_shipmentdisplay.h"

ShipmentDisplay::ShipmentDisplay(int new_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ShipmentDisplay)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	ui->shipmentBox->setId(new_id);
	ui->drugBox->setId(ui->shipmentBox->getDrug_id());

	this->show();
}

ShipmentDisplay::~ShipmentDisplay()
{
	delete ui;
}
