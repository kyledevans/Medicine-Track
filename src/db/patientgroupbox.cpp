/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientgroupbox.h"
#include "ui_patientgroupbox.h"

#include "../globals.h"

PatientGroupBox::PatientGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::PatientGroupBox)
{
	ui->setupUi(this);

	// Set UI strings
	ui->allscriptsTitle->setText(PatientRecord::allscripts_id_Label + ":");
	ui->allscriptsTitle->setToolTip(PatientRecord::allscripts_id_Tooltip);
	ui->allscriptsLabel->setToolTip(PatientRecord::allscripts_id_Tooltip);

	ui->lastTitle->setText(PatientRecord::last_Label + ":");
	ui->lastTitle->setToolTip(PatientRecord::last_Tooltip);
	ui->lastLabel->setToolTip(PatientRecord::last_Tooltip);

	ui->firstTitle->setText(PatientRecord::first_Label + ":");
	ui->firstTitle->setToolTip(PatientRecord::first_Tooltip);
	ui->firstLabel->setToolTip(PatientRecord::first_Tooltip);

	ui->dobTitle->setText(PatientRecord::dob_Label + ":");
	ui->dobTitle->setToolTip(PatientRecord::dob_Tooltip);
	ui->dobLabel->setToolTip(PatientRecord::dob_Tooltip);

	ui->activeTitle->setText(PatientRecord::active_Label + ":");
	ui->activeTitle->setToolTip(PatientRecord::active_Tooltip);
	ui->activeLabel->setToolTip(PatientRecord::active_Tooltip);
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

void PatientGroupBox::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
