/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>

#include <QDebug>

#include "barcodelabel.h"
#include "ui_barcodelabel.h"

BarcodeLabel::BarcodeLabel(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::BarcodeLabel),
	has_barcode(false)
{
    ui->setupUi(this);
}

BarcodeLabel::~BarcodeLabel()
{
    delete ui;
}

void BarcodeLabel::setBarcode(QString new_barcode)
{
	barcode = QString("*") + (new_barcode.toUpper()) + QString("*");
	has_barcode = true;
}

QString BarcodeLabel::getBarcode()
{
	QString temp;

	temp = barcode;
	temp.remove('*');

	return temp;
}

bool BarcodeLabel::hasBarcode()
{
	return has_barcode;
}

void BarcodeLabel::print()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog *diag = new QPrintDialog(&printer);
	connect(diag, SIGNAL(accepted(QPrinter*)), this, SLOT(printLabel(QPrinter*)));
	diag->exec();

	if (!has_barcode) {
		return;
	}
}

void BarcodeLabel::printLabel(QPrinter *printer)
{
	QPainter painter;

	ui->barcodeLabel->setText(barcode);

	painter.begin(printer);
	painter.scale(7.0, 7.0);
	this->render(&painter);
	painter.end();
}
