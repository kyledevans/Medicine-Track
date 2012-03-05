/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientgroupbox.h"
#include "ui_patientgroupbox.h"

#include "globals.h"

PatientGroupBox::PatientGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::PatientGroupBox)
{
	ui->setupUi(this);

	setStrings();
}

void PatientGroupBox::setId(int new_id)
{
	QString temp;

	patient.retrieve(new_id);	// TODO: Doesn't handle errors in retrieve

	// Set values from the patient
	ui->allscriptsLabel->setText(QString().setNum(patient.getAllscripts_id()));
	ui->lastLabel->setText(patient.getLast());
	ui->firstLabel->setText(patient.getFirst());
	ui->dobLabel->setText(patient.getDob().toString(DEFAULTS::DateDisplayFormat));
	if (patient.getActive()) {
		ui->activeLabel->setText("Active");
	} else {
		temp = QString("<font color='#%1'>%2</font>");
		ui->activeLabel->setText(temp.arg(MTCOLORS::Problem, QString("Inactive")));
	}
}

PatientGroupBox::~PatientGroupBox()
{
	delete ui;
}

void PatientGroupBox::setStrings()
{
	// Set UI strings
	ui->allscriptsTitle->setText(ui->allscriptsTitle->text() + ":");
	ui->lastTitle->setText(ui->lastTitle->text() + ":");
	ui->firstTitle->setText(ui->firstTitle->text() + ":");
	ui->dobTitle->setText(ui->dobTitle->text() + ":");
	ui->activeTitle->setText(ui->activeTitle->text() + ":");
}

void PatientGroupBox::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		setStrings();
		break;
	default:
		break;
	}
}
