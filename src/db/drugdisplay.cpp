/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "drugdisplay.h"
#include "ui_drugdisplay.h"

#include "globals.h"

DrugDisplay::DrugDisplay(int new_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DrugDisplay)
{
	QString temp;
	ui->setupUi(this);

	drug.retrieve(new_id);	// TODO: doesn't handle failed queries
	setAttribute(Qt::WA_DeleteOnClose);

	// Set UI strings
	ui->medicationTitle->setText(DrugRecord::name_Label + ":");
	ui->medicationTitle->setToolTip(DrugRecord::name_Tooltip);
	ui->medicationLabel->setToolTip(DrugRecord::name_Tooltip);

	ui->genericTitle->setText(DrugRecord::generic_Label + ":");
	ui->genericTitle->setToolTip(DrugRecord::generic_Tooltip);
	ui->genericLabel->setToolTip(DrugRecord::generic_Tooltip);

	ui->manufacturerTitle->setText(DrugRecord::manufacturer_Label + ":");
	ui->manufacturerTitle->setToolTip(DrugRecord::manufacturer_Tooltip);
	ui->manufacturerLabel->setToolTip(DrugRecord::manufacturer_Tooltip);

	ui->ndcTitle->setText(DrugRecord::ndc_Label + ":");
	ui->ndcTitle->setToolTip(DrugRecord::ndc_Tooltip);
	ui->ndcLabel->setToolTip(DrugRecord::ndc_Tooltip);

	ui->formTitle->setText(DrugRecord::form_Label + ":");
	ui->formTitle->setToolTip(DrugRecord::form_Label);
	ui->formLabel->setToolTip(DrugRecord::form_Tooltip);

	ui->strengthTitle->setText(DrugRecord::strength_Label + ":");
	ui->strengthTitle->setToolTip(DrugRecord::strength_Tooltip);
	ui->strengthLabel->setToolTip(DrugRecord::strength_Tooltip);

	ui->dispenseTitle->setText(DrugRecord::dispense_units_Label + ":");
	ui->dispenseTitle->setToolTip(DrugRecord::dispense_units_Tooltip);
	ui->dispenseLabel->setToolTip(DrugRecord::dispense_units_Tooltip);

	ui->unitTitle->setText(DrugRecord::unit_size_Label + ":");
	ui->unitTitle->setToolTip(DrugRecord::unit_size_Tooltip);
	ui->unitLabel->setToolTip(DrugRecord::unit_size_Tooltip);

	ui->instructionsTitle->setText(DrugRecord::instructions_Label + ":");
	ui->instructionsTitle->setToolTip(DrugRecord::instructions_Tooltip);
	ui->instructionsLabel->setToolTip(DrugRecord::instructions_Tooltip);

	ui->activeTitle->setText(DrugRecord::active_Label + ":");
	ui->activeTitle->setToolTip(DrugRecord::active_Tooltip);
	ui->activeLabel->setToolTip(DrugRecord::active_Tooltip);

	// Set the values from the medication record
	ui->medicationLabel->setText(drug.getName());
	ui->genericLabel->setText(drug.getGeneric());
	ui->manufacturerLabel->setText(drug.getManufacturer());
	ui->ndcLabel->setText(drug.getNdc());
	ui->formLabel->setText(FORM_STR::intToStr(drug.getForm()));
	ui->strengthLabel->setText(drug.getStrength());
	ui->dispenseLabel->setText(drug.getDispense_units());
	ui->unitLabel->setText(drug.getUnit_size());
	ui->instructionsLabel->setText(drug.getInstructions());
	if (drug.getActive()) {
		ui->activeLabel->setText("Active");
	} else {
		temp = QString("<font color='#%1'>%2</font>");
		ui->activeLabel->setText(temp.arg(MTCOLORS::Problem, QString("Inactive")));
	}

	this->show();
}

DrugDisplay::~DrugDisplay()
{
	delete ui;
}
