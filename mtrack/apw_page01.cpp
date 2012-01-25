/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "apw_page01.h"
#include "ui_apw_page01.h"

APW_Page01::APW_Page01(QWidget *parent) :
    QWizardPage(parent),
	ui(new Ui::APW_Page01),
	patient(0),
	medication(0),
	prescription(0),
	shipment(0)
{
    ui->setupUi(this);
}

APW_Page01::~APW_Page01()
{
    delete ui;
}

void APW_Page01::initCustom()
{
	ui->lastLabel->setText(patient->last);
	ui->firstLabel->setText(patient->first);
}

void APW_Page01::setPatient(PatientRecord *new_patient)
{
	patient = new_patient;
}

void APW_Page01::setMedication(MedicationRecord *new_medication)
{
	medication = new_medication;
}

void APW_Page01::setPrescription(PrescriptionRecord *new_prescription)
{
	prescription = new_prescription;
}

void APW_Page01::setShipment(ShipmentRecord *new_shipment)
{
	shipment = new_shipment;
}
