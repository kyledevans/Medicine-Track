/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>

#include <QDebug>

#include "prescriptionlabel.h"
#include "ui_prescriptionlabel.h"

#include "globals.h"

PrescriptionLabel::PrescriptionLabel(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PrescriptionLabel),
	prescription(NULL)
{
	ui->setupUi(this);
}

PrescriptionLabel::PrescriptionLabel(PrescriptionRecord *new_prescription, QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PrescriptionLabel)
{
	ui->setupUi(this);

	prescriber = new PrescriberRecord(this);
	pharmacist = new PharmacistRecord(this);
	patient = new PatientRecord(this);
	medication = new MedicationRecord(this);
	shipment = new ShipmentRecord(this);

	prescription = new_prescription;
	prescriber->retrieve(prescription->prescriber_id);
	pharmacist->retrieve(prescription->pharmacist_id);
	patient->retrieve(prescription->patient_id);
	medication->retrieve(prescription->drug_id);
	shipment->retrieve(prescription->shipment_id);

	setFixedWidth(700);
}

PrescriptionLabel::~PrescriptionLabel()
{
	delete ui;
}

void PrescriptionLabel::printLabel()
{
	QPrinter printer(QPrinter::HighResolution);
	/*QPrintDialog *diag = new QPrintDialog(&printer);
	connect(diag, SIGNAL(accepted(QPrinter*)), this, SLOT(print(QPrinter*)));
	diag->exec();*/

	printer.setOutputFileName("C:\\Users\\Evans\\Desktop\\epc_db\\print.pdf");
	printer.setOutputFormat(QPrinter::PdfFormat);
	print(&printer);
}

void PrescriptionLabel::print(QPrinter *printer)
{
	QPainter painter;

	printer->setPageMargins(0.25, 0.0, 0.0, 0.6, QPrinter::Inch);
	printer->setOrientation(QPrinter::Landscape);

	ui->lastLabel->setText(patient->last);
	ui->firstLabel->setText(patient->first);
	ui->medicationLabel->setText(medication->name);
	ui->manufacturerLabel->setText(medication->manufacturer);
	ui->formLabel->setText(FORM_STR::intToStr(medication->form));
	ui->strengthLabel->setText(medication->strength);
	ui->prescriberLabel->setText(prescriber->full_name);
	ui->instructionsLabel->setText(prescription->instructions);
	ui->filledLabel->setText(prescription->filled.toString(DEFAULTS::DateDisplayFormat));
	ui->writtenLabel->setText(prescription->written.toString(DEFAULTS::DateDisplayFormat));
	ui->ndcLabel->setText(medication->ndc);
	ui->genericLabel->setText(medication->generic);
	ui->lotLabel->setText(shipment->lot);
	ui->fillerLabel->setText(pharmacist->initials);

	painter.begin(printer);
	this->render(&painter);
	painter.end();
}
