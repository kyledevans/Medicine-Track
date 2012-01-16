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
	pres(NULL)
{
	ui->setupUi(this);
}

PrescriptionLabel::PrescriptionLabel(PrescriptionRecord *new_record, QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PrescriptionLabel)
{
	ui->setupUi(this);
	pres = new_record;

	setFixedWidth(700);
}

PrescriptionLabel::~PrescriptionLabel()
{
	delete ui;
}

void PrescriptionLabel::printLabel()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog *diag = new QPrintDialog(&printer);
	connect(diag, SIGNAL(accepted(QPrinter*)), this, SLOT(print(QPrinter*)));
	diag->exec();

	//printer.setOutputFileName("C:\\Users\\Evans\\Desktop\\simple_epc\\print.pdf");
	//printer.setOutputFormat(QPrinter::PdfFormat);
	//print(&printer);
}

void PrescriptionLabel::print(QPrinter *printer)
{
	QPainter painter;

	printer->setPageMargins(0.25, 0.0, 0.0, 0.6, QPrinter::Inch);
	printer->setOrientation(QPrinter::Landscape);

	// TODO: This needs to be updated to reflect current data structures
	/*ui->lastLabel->setText(pres->last);
	ui->firstLabel->setText(pres->first);
	ui->medicationLabel->setText(pres->drug);
	ui->manufacturerLabel->setText(pres->manufacturer);
	ui->formLabel->setText(pres->form);
	ui->strengthLabel->setText(pres->strength);
	ui->prescriberLabel->setText(pres->prescriber);
	ui->instructionsLabel->setText(pres->instructions);
	ui->filledLabel->setText(pres->filled.toString("MM/d/yyyy"));
	ui->writtenLabel->setText(pres->written.toString("MM/d/yyyy"));
	ui->ndcLabel->setText(pres->ndc);
	ui->genericLabel->setText(GENERIC::toTier2(pres->drug));
	ui->lotLabel->setText(pres->lot);
	ui->fillerLabel->setText(PHARM_STR::toAbr(pres->pharmacist));*/

	painter.begin(printer);
	this->render(&painter);
	painter.end();
}
