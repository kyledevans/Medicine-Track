/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "altermedicationwizard.h"
#include "ui_altermedicationwizard.h"

#include "globals.h"

#include <QDebug>

AlterMedicationWizard::AlterMedicationWizard(QWidget *parent) :
    QWizard(parent),
	ui(new Ui::AlterMedicationWizard),
	med(NULL)
{
    ui->setupUi(this);
}

AlterMedicationWizard::AlterMedicationWizard(MedicationRecord *new_med, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::AlterMedicationWizard)
{
	//int temp;
	ui->setupUi(this);
	med = new_med;

	// Fill in the information into the form if the wizard is modifying an existing record
	if (med->exists) {
		setField("medicationField", med->name);
		setField("genericField", med->generic);
		setField("manufacturerField", med->manufacturer);
		setField("ndcField", med->ndc);
		setField("strengthField", med->strength);
		setField("strUnitsField", med->str_units);
		setField("instructionsField", med->instructions);
		setField("activeField", med->active);
		setField("amUnitsField", med->am_units);
		ui->page0->setForm(med->form);
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

AlterMedicationWizard::~AlterMedicationWizard()
{
    delete ui;
}

void AlterMedicationWizard::returnResults()
{
	ui->page0->getResults(med);
	ui->page1->getResults(med);
	emit(wizardComplete(med));
}

void AlterMedicationWizard::rejectedWizard()
{
	emit(wizardRejected(med));
}
