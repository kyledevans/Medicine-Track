/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientdisplay.h"
#include "ui_patientdisplay.h"

#include "../globals.h"

PatientDisplay::PatientDisplay(int new_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PatientDisplay)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	ui->patientBox->setId(new_id);

	this->show();
}

PatientDisplay::~PatientDisplay()
{
	delete ui;
}
