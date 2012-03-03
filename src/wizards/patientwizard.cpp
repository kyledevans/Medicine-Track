/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientwizard.h"
#include "ui_patientwizard.h"

#include <QDebug>

PatientWizard::PatientWizard(PatientRecord *new_patient, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::PatientWizard)
{
	ui->setupUi(this);

	patient = new_patient;

	if (patient->getExists()) {
		this->setWindowTitle("Alter Patient Wizard");
		setField("mrNumberField", patient->getAllscripts_id());
		setField("lastField", patient->getLast());
		setField("firstField", patient->getFirst());
		setField("dobField", patient->getDob());
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

PatientWizard::~PatientWizard()
{
	delete ui;
}

void PatientWizard::returnResults()
{
	ui->page0->getResults(patient);
	emit(wizardComplete(patient));
}

void PatientWizard::rejectedWizard()
{
	emit(wizardRejected(patient));
}

void PatientWizard::changeEvent(QEvent *e)
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
