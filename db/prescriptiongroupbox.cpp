/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptiongroupbox.h"
#include "ui_prescriptiongroupbox.h"

#include "drugrecord.h"
#include "globals.h"

PrescriptionGroupBox::PrescriptionGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::PrescriptionGroupBox)
{
	ui->setupUi(this);

	setStrings();
}

PrescriptionGroupBox::~PrescriptionGroupBox()
{
	delete ui;
}

void PrescriptionGroupBox::setId(int new_id)
{
	QString temp;
	DrugRecord drug;

	prescription.retrieve(new_id);				// TODO: This does not handle query failure
	drug.retrieve(prescription.getDrug_id());	// TODO: This does not handle query failure

	ui->amountLabel->setText(QString().setNum(prescription.getAmount()) + " " + drug.getDispense_units());
	ui->writtenLabel->setText(prescription.getWritten().toString(DEFAULTS::DateDisplayFormat));
	ui->filledLabel->setText(prescription.getFilled().toString(DEFAULTS::DateDisplayFormat));
	ui->instructionsLabel->setText(prescription.getInstructions());
	if (prescription.getActive()) {
		ui->validLabel->setText("Valid");
	} else {
		temp = QString("<font color='#%1'>%2</font>");
		ui->validLabel->setText(temp.arg(MTCOLORS::Problem, QString("Invalid")));
	}
}

int PrescriptionGroupBox::getId()
{
	return prescription.getId();
}

int PrescriptionGroupBox::getPatient_id()
{
	return prescription.getPatient_id();
}

int PrescriptionGroupBox::getDrug_id()
{
	return prescription.getDrug_id();
}

int PrescriptionGroupBox::getShipment_id()
{
	return prescription.getShipment_id();
}

int PrescriptionGroupBox::getPrescriber_id()
{
	return prescription.getPrescriber_id();
}

int PrescriptionGroupBox::getPharmacist_id()
{
	return prescription.getPharmacist_id();
}

void PrescriptionGroupBox::setStrings()
{
	// Set UI strings
	ui->amountTitle->setText(ui->amountTitle->text() + ":");
	ui->writtenTitle->setText(ui->writtenTitle->text() + ":");
	ui->filledTitle->setText(ui->filledTitle->text() + ":");
	ui->instructionsTitle->setText(ui->instructionsTitle->text() + ":");
	ui->validTitle->setText(ui->validTitle->text() + ":");
}

void PrescriptionGroupBox::changeEvent(QEvent *e)
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
