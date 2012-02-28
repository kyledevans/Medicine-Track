/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "medicationwizard.h"
#include "ui_medicationwizard.h"

#include "globals.h"

#include <QDebug>

MedicationWizard::MedicationWizard(DrugRecord *new_med, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::MedicationWizard)
{
	//int temp;
	ui->setupUi(this);
	med = new_med;

	// Fill in the information into the form if the wizard is modifying an existing record
	if (med->getExists()) {
		this->setWindowTitle("Alter Medication Wizard");
		setField("medicationField", med->getName());
		setField("genericField", med->getGeneric());
		setField("manufacturerField", med->getManufacturer());
		setField("ndcField", med->getNdc());
		setField("strengthField", med->getStrength());
		ui->page0->setForm(med->getForm());
		setField("dispenseUnitsField", med->getDispense_units());
		setField("unitSizeField", med->getUnit_size());
		setField("instructionsField", med->getInstructions());
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

MedicationWizard::~MedicationWizard()
{
	delete ui;
}

void MedicationWizard::returnResults()
{
	ui->page0->getResults(med);
	ui->page1->getResults(med);
	emit(wizardComplete(med));
}

void MedicationWizard::rejectedWizard()
{
	emit(wizardRejected(med));
}
