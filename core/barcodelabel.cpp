﻿/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "barcodelabel.h"
#include "ui_barcodelabel.h"

#include <QPrintDialog>
#include <QPainter>
#include <QSizeF>

#include "../db/globals.h"

#include <QDebug>

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

void BarcodeLabel::setBarcode(QString new_barcode)	// TODO: Some usages of this might not add SID first
{
	barcode = "*" + new_barcode.toUpper() + "*";
	has_barcode = true;
}

bool BarcodeLabel::isValidSID(QString str)
{
	QRegExp reg("^SID");
	if (str.contains(reg)) {
		return true;
	}
	return false;
}

QString BarcodeLabel::getBarcode()
{
	QString temp;

	temp = barcode;
	temp.remove('*');

	return temp;
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

int BarcodeLabel::toID()
{
	QString temp;
	bool ok;
	int id;


	if (!has_barcode) {
		return SQL::Undefined_ID;
	}

	temp = barcode;
	temp.remove("SID");
	temp.remove('*');
	id = temp.toInt(&ok);
	if (!ok) {
		return SQL::Undefined_ID;
	}
	return id;
}

void BarcodeLabel::printLabel(QPrinter *printer)
{
	QPainter painter;
	QSizeF size(1.125, 3.5);

	setFixedSize(1125, 3500);

	printer->setPaperSize(size, QPrinter::Inch);
	printer->setOrientation(QPrinter::Landscape);

	ui->barcodeLabel->setText(barcode);
	ui->asciiLabel->setText(getBarcode());

	painter.begin(printer);
	painter.scale(3.0, 3.0);
	this->render(&painter);
	painter.end();
}
