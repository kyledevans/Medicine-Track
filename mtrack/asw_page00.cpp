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

	ui->expireLabel->setText(ShipmentRecord::expiration_Label);
	ui->expireLabel->setToolTip(ShipmentRecord::expiration_Tooltip);
	ui->expireField->setToolTip(ShipmentRecord::expiration_Tooltip);

	ui->lotLabel->setText(ShipmentRecord::lot_Label);
	ui->lotLabel->setToolTip(ShipmentRecord::lot_Tooltip);
	ui->lotField->setToolTip(ShipmentRecord::lot_Tooltip);

	ui->unitsLabel->setText(ShipmentRecord::product_count_Label);
	ui->unitsLabel->setToolTip(ShipmentRecord::product_count_Tooltip);
	ui->unitsField->setToolTip(ShipmentRecord::product_count_Tooltip);

	ui->stockLabel->setText(ShipmentRecord::product_left_Label);
	ui->stockLabel->setToolTip(ShipmentRecord::product_left_Tooltip);
	ui->stockField->setToolTip(ShipmentRecord::product_left_Tooltip);

	ui->writeOffLabel->setText(ShipmentRecord::write_off_Label_edit);
	ui->writeOffLabel->setToolTip(ShipmentRecord::write_off_Tooltip_edit);
	ui->writeOffField->setToolTip(ShipmentRecord::write_off_Tooltip_edit);

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
