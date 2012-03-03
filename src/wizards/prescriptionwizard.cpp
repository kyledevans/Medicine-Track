/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptionwizard.h"
#include "ui_prescriptionwizard.h"

#include <QDebug>

PrescriptionWizard::PrescriptionWizard(QWidget *parent) :
	QWizard(parent),
	ui(new Ui::PrescriptionWizard)
{
	ui->setupUi(this);

	connect(ui->page0, SIGNAL(medicationChanged(int)), this, SLOT(changeMedication(int)));

	ui->page1->initCustom();

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));

	patient = new PatientRecord;
    medication = new DrugRecord;
	shipment = new ShipmentRecord;

	ui->page1->setMedication(medication);
	ui->page2->setMedication(medication);
}

PrescriptionWizard::~PrescriptionWizard()
{
	delete ui;

	delete patient;
	delete medication;
	delete shipment;
}

bool PrescriptionWizard::getPatient(int new_id)
{
	if (!patient->retrieve(new_id)) {
		return false;
	}

	ui->page0->setPatient(patient);
	ui->page1->setPatient(patient);
	return true;
}

void PrescriptionWizard::returnResults()
{
	ui->page1->getResults();
	ui->page2->getResults();
	emit(wizardComplete(prescription));
}

void PrescriptionWizard::rejectedWizard()
{
	emit(wizardRejected(prescription));
}

void PrescriptionWizard::changeMedication(int new_id)
{
	shipment->retrieve(new_id);
	medication->retrieve(shipment->getDrug_id());
	prescription->setShipment_id(shipment->getId());
	prescription->setDrug_id(medication->getId());
	ui->page1->medUpdated();
	ui->page2->medUpdated();
}

void PrescriptionWizard::setPrescription(PrescriptionRecord *new_prescription)
{
	prescription = new_prescription;
	ui->page0->setPrescription(prescription);
	ui->page1->setPrescription(prescription);
	ui->page2->setPrescription(prescription);
}

void PrescriptionWizard::changeEvent(QEvent *e)
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
