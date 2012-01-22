/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "asw_page00.h"
#include "ui_asw_page00.h"

#include "globals.h"

ASW_Page00::ASW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ASW_Page00)
{
    ui->setupUi(this);

	ui->expireField->setDate(DEFAULTS::Date);

	registerField("expireField", ui->expireField);
	registerField("lotField", ui->lotField);
	registerField("unitsField", ui->unitsField);
	registerField("stockField", ui->stockField);
}

ASW_Page00::~ASW_Page00()
{
    delete ui;
}

void ASW_Page00::getResults(ShipmentRecord *shipment)
{
	shipment->expiration = ui->expireField->date();
	shipment->lot = ui->lotField->text();
	shipment->product_count = ui->unitsField->text().toInt();
	shipment->product_left = ui->stockField->text().toInt();
}
