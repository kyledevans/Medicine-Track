/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "altershipmentwizard.h"
#include "ui_altershipmentwizard.h"

#include <QString>

#include <QDebug>

#include "medicationrecord.h"

AlterShipmentWizard::AlterShipmentWizard(ShipmentRecord *new_shipment, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::AlterShipmentWizard)
{
	MedicationRecord *med;
	shipment = new_shipment;
	med = new MedicationRecord;
	med->retrieve(shipment->drug_id);
	ui->setupUi(this);

	ui->page00->setMedication(med);

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
