/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "druggroupbox.h"
#include "ui_druggroupbox.h"

#include "globals.h"

DrugGroupBox::DrugGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::DrugGroupBox)
{
	ui->setupUi(this);

	setStrings();
}

void DrugGroupBox::setId(int new_id)
{
	QString temp;

	drug.retrieve(new_id);	// TODO: doesn't handle failed queries

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
}

DrugGroupBox::~DrugGroupBox()
{
	delete ui;
}

void DrugGroupBox::setStrings()
{
	// Set UI strings
	ui->medicationTitle->setText(ui->medicationTitle->text() + ":");
	ui->genericTitle->setText(ui->genericTitle->text() + ":");
	ui->manufacturerTitle->setText(ui->manufacturerTitle->text() + ":");
	ui->ndcTitle->setText(ui->ndcTitle->text() + ":");
	ui->formTitle->setText(ui->formTitle->text() + ":");
	ui->strengthTitle->setText(ui->strengthTitle->text() + ":");
	ui->dispenseTitle->setText(ui->dispenseTitle->text() + ":");
	ui->unitTitle->setText(ui->unitTitle->text() + ":");
	ui->instructionsTitle->setText(ui->instructionsTitle->text() + ":");
	ui->activeTitle->setText(ui->activeTitle->text() + ":");
}

void DrugGroupBox::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		setStrings();
		break;
	default:
		break;
	}
}
