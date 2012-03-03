/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptiondisplay.h"
#include "ui_prescriptiondisplay.h"

#include "../globals.h"

PrescriptionDisplay::PrescriptionDisplay(int new_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PrescriptionDisplay)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	ui->prescriptionBox->setId(new_id);
	ui->patientBox->setId(ui->prescriptionBox->getPatient_id());
	ui->drugBox->setId(ui->prescriptionBox->getDrug_id());
	ui->shipmentBox->setId(ui->prescriptionBox->getShipment_id());
	ui->prescriberBox->setId(ui->prescriptionBox->getPrescriber_id());
	ui->pharmacistBox->setId(ui->prescriptionBox->getPharmacist_id());

	this->show();
}

PrescriptionDisplay::~PrescriptionDisplay()
{
	delete ui;
}

void PrescriptionDisplay::changeEvent(QEvent *e)
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
