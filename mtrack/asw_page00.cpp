/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QIntValidator>

#include "asw_page00.h"
#include "ui_asw_page00.h"

#include "globals.h"

ASW_Page00::ASW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ASW_Page00)
{
    ui->setupUi(this);

	QValidator *numbers = new QIntValidator(this);
	ui->stockField->setValidator(numbers);
	ui->unitsField->setValidator(numbers);
	ui->writeOffField->setValidator(numbers);

	ui->expireField->setDate(DEFAULTS::Date);

	registerField("expireField", ui->expireField);
	registerField("lotField", ui->lotField);
	registerField("unitsField", ui->unitsField);
	registerField("stockField", ui->stockField);
	registerField("writeOffField", ui->writeOffField);
	registerField("activeField", ui->activeField);
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
	shipment->write_off = ui->writeOffField->text().toInt();
	shipment->active = ui->activeField->isChecked();
}
