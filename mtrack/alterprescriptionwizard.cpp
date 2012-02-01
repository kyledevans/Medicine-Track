/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "alterprescriptionwizard.h"
#include "ui_alterprescriptionwizard.h"

#include <QDebug>

AlterPrescriptionWizard::AlterPrescriptionWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AlterPrescriptionWizard)
{
    ui->setupUi(this);

	connect(ui->page0, SIGNAL(medicationChanged(int)), this, SLOT(changeMedication(int)));

	ui->page1->initCustom();

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

AlterPrescriptionWizard::~AlterPrescriptionWizard()
{
    delete ui;
}

void AlterPrescriptionWizard::returnResults()
{
	ui->page1->getResults();
	ui->page2->getResults();
	emit(wizardComplete(prescription));
}

void AlterPrescriptionWizard::rejectedWizard()
{
	emit(wizardRejected(prescription));
}

void AlterPrescriptionWizard::changeMedication(int new_id)
{
	shipment->retrieve(new_id);
	medication->retrieve(shipment->drug_id);
	prescription->shipment_id = shipment->id;
	prescription->drug_id = medication->id;
	ui->page1->medUpdated();
}

void AlterPrescriptionWizard::setPatient(PatientRecord *new_patient)
{
	patient = new_patient;
	ui->page0->setPatient(patient);
	ui->page1->setPatient(patient);
}

void AlterPrescriptionWizard::setMedication(MedicationRecord *new_medication)
{
	medication = new_medication;
	ui->page1->setMedication(medication);
}

void AlterPrescriptionWizard::setPrescription(PrescriptionRecord *new_prescription)
{
	prescription = new_prescription;
	ui->page0->setPrescription(prescription);
	ui->page1->setPrescription(prescription);
	ui->page2->setPrescription(prescription);
}

void AlterPrescriptionWizard::setShipment(ShipmentRecord *new_shipment)
{
	shipment = new_shipment;
}
