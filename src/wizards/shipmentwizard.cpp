/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipmentwizard.h"
#include "ui_shipmentwizard.h"

#include <QString>

#include "db/drugrecord.h"

#include <QDebug>

ShipmentWizard::ShipmentWizard(ShipmentRecord *new_shipment, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::ShipmentWizard)
{
    DrugRecord *med;
	shipment = new_shipment;
    med = new DrugRecord;
	med->retrieve(shipment->drug_id);
	ui->setupUi(this);

	ui->page0->setMedication(med);

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

ShipmentWizard::~ShipmentWizard()
{
	delete ui;
}

void ShipmentWizard::returnResults()
{
	ui->page0->getResults(shipment);
	emit(wizardComplete(shipment));
}

void ShipmentWizard::rejectedWizard()
{
	emit(wizardRejected(shipment));
}
