/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "altershipmentwizard.h"
#include "ui_altershipmentwizard.h"

#include <QString>

#include <QDebug>

AlterShipmentWizard::AlterShipmentWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AlterShipmentWizard)
{
    ui->setupUi(this);
}

AlterShipmentWizard::AlterShipmentWizard(ShipmentRecord *new_shipment, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::AlterShipmentWizard)
{
	ui->setupUi(this);
	shipment = new_shipment;

	if (shipment->exists) {
		setField("expireField", shipment->expiration);
		setField("lotField", shipment->lot);
		setField("unitsField", shipment->product_count);
		setField("stockField", shipment->product_left);
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

AlterShipmentWizard::~AlterShipmentWizard()
{
    delete ui;
}

void AlterShipmentWizard::returnResults()
{
	ui->page00->getResults(shipment);
	emit(wizardComplete(shipment));
}

void AlterShipmentWizard::rejectedWizard()
{
	emit(wizardRejected(shipment));
}
