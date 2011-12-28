/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

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
		ui->expireField->setDate(shipment->expiration);
		ui->lotField->setText(shipment->lot);
		ui->unitsField->setText(QString().setNum(shipment->product_count));
		ui->stockField->setText(QString().setNum(shipment->product_left));
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
}

AlterShipmentWizard::~AlterShipmentWizard()
{
    delete ui;
}

void AlterShipmentWizard::returnResults()
{
	shipment->expiration = ui->expireField->date();
	shipment->lot = ui->lotField->text();
	shipment->product_count = ui->unitsField->text().toInt();
	shipment->product_left = ui->stockField->text().toInt();
	qDebug() << "here";
	emit(wizardComplete(shipment));
}
