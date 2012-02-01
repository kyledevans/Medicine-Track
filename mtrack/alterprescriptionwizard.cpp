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
}

AlterPrescriptionWizard::~AlterPrescriptionWizard()
{
    delete ui;
}

void AlterPrescriptionWizard::changeMedication(int new_id)
{
	shipment->retrieve(new_id);
	medication->retrieve(shipment->drug_id);
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
}

void AlterPrescriptionWizard::setShipment(ShipmentRecord *new_shipment)
{
	shipment = new_shipment;
}
