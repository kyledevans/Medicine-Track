/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptionlabel.h"
#include "ui_prescriptionlabel.h"

#include <QPrintDialog>
#include <QPainter>

#include "globals.h"

#include <QDebug>

PrescriptionLabel::PrescriptionLabel(PrescriptionRecord *new_prescription, QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PrescriptionLabel)
{
	ui->setupUi(this);

	prescriber = new PrescriberRecord;
	pharmacist = new PharmacistRecord;
	patient = new PatientRecord;
    medication = new DrugRecord;
	shipment = new ShipmentRecord;

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

	delete prescriber;
	delete pharmacist;
	delete patient;
	delete medication;
	delete shipment;
}

void PrescriptionLabel::print()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog *diag = new QPrintDialog(&printer);
	connect(diag, SIGNAL(accepted(QPrinter*)), this, SLOT(printLabel(QPrinter*)));
	diag->exec();

	/*printer.setOutputFileName("C:\\Users\\Evans\\Desktop\\print.pdf");
	printer.setOutputFormat(QPrinter::PdfFormat);
	printLabel(&printer);*/
}

void PrescriptionLabel::printLabel(QPrinter *printer)
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
	if (medication->generic.isEmpty()) {	// Don't display "generic for..." if it doesn't make sense
		ui->genericHeaderLabel->setText(QString(" "));
		ui->genericLabel->setText(QString(" "));
	} else {
		ui->genericLabel->setText(medication->generic);
	}
	if (medication->unit_size.isEmpty()) {	// Display the amount dispensed
		ui->dispensedLabel->setText(QString().setNum(prescription->amount) + QString(" ") + medication->dispense_units);
	} else {	// Display the size of the bottle, tube, etc. If they come in fixed sizes instead
		if (prescription->amount > 1) {	// Indicate how many units were given and their size
			ui->dispensedLabel->setText(QString().setNum(prescription->amount) + QString(" x ") + medication->unit_size);
		} else {	// Only 1 unit was given, just show the size
			ui->dispensedLabel->setText(medication->unit_size);
		}
	}

	ui->fillerLabel->setText(pharmacist->initials);

	painter.begin(printer);
	this->render(&painter);
	painter.end();
}
