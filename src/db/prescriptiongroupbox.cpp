/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptiongroupbox.h"
#include "ui_prescriptiongroupbox.h"

#include "drugrecord.h"
#include "../globals.h"

PrescriptionGroupBox::PrescriptionGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::PrescriptionGroupBox)
{
	ui->setupUi(this);

	// Set UI strings
	ui->amountTitle->setText(PrescriptionRecord::amount_Label + ":");
	ui->amountTitle->setToolTip(PrescriptionRecord::amount_Tooltip);
	ui->amountLabel->setToolTip(PrescriptionRecord::amount_Tooltip);

	ui->writtenTitle->setText(PrescriptionRecord::written_Label + ":");
	ui->writtenTitle->setToolTip(PrescriptionRecord::written_Tooltip);
	ui->writtenLabel->setToolTip(PrescriptionRecord::written_Tooltip);

	ui->filledTitle->setText(PrescriptionRecord::filled_Label + ":");
	ui->filledTitle->setToolTip(PrescriptionRecord::filled_Tooltip);
	ui->filledLabel->setToolTip(PrescriptionRecord::filled_Tooltip);

	ui->instructionsTitle->setText(PrescriptionRecord::instructions_Label + ":");
	ui->instructionsTitle->setToolTip(PrescriptionRecord::instructions_Tooltip);
	ui->instructionsLabel->setToolTip(PrescriptionRecord::instructions_Tooltip);

	ui->validTitle->setText(PrescriptionRecord::active_Label + ":");
	ui->validTitle->setToolTip(PrescriptionRecord::active_Tooltip);
	ui->validLabel->setToolTip(PrescriptionRecord::active_Tooltip);
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

void PrescriptionGroupBox::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
