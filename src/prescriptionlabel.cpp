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
	prescriber->retrieve(prescription->getPrescriber_id());
	pharmacist->retrieve(prescription->getPharmacist_id());
	patient->retrieve(prescription->getPatient_id());
	medication->retrieve(prescription->getDrug_id());
	shipment->retrieve(prescription->getShipment_id());

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

	ui->lastLabel->setText(patient->getLast());
	ui->firstLabel->setText(patient->getFirst());
	ui->medicationLabel->setText(medication->getName());
	ui->manufacturerLabel->setText(medication->getManufacturer());
	ui->formLabel->setText(FORM_STR::intToStr(medication->getForm()));
	ui->strengthLabel->setText(medication->getStrength());
	ui->prescriberLabel->setText(prescriber->getFull_name());
	ui->instructionsLabel->setText(prescription->getInstructions());
	ui->filledLabel->setText(prescription->getFilled().toString(DEFAULTS::DateDisplayFormat));
	ui->writtenLabel->setText(prescription->getWritten().toString(DEFAULTS::DateDisplayFormat));
	ui->ndcLabel->setText(medication->getNdc());
	if (medication->getGeneric().isEmpty()) {	// Don't display "generic for..." if it doesn't make sense
		ui->genericHeaderLabel->setText(QString(" "));
		ui->genericLabel->setText(QString(" "));
	} else {
		ui->genericLabel->setText(medication->getGeneric());
	}
	if (medication->getUnit_size().isEmpty()) {	// Display the amount dispensed
		ui->dispensedLabel->setText(QString().setNum(prescription->getAmount()) + QString(" ") + medication->getDispense_units());
	} else {	// Display the size of the bottle, tube, etc. If they come in fixed sizes instead
		if (prescription->getAmount() > 1) {	// Indicate how many units were given and their size
			ui->dispensedLabel->setText(QString().setNum(prescription->getAmount()) + QString(" x ") + medication->getUnit_size());
		} else {	// Only 1 unit was given, just show the size
			ui->dispensedLabel->setText(medication->getUnit_size());
		}
	}

	ui->fillerLabel->setText(pharmacist->getInitials());

	painter.begin(printer);
	this->render(&painter);
	painter.end();
}
