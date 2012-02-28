/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberwizard.h"
#include "ui_prescriberwizard.h"

PrescriberWizard::PrescriberWizard(PrescriberRecord *new_pres, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::PrescriberWizard)
{
	ui->setupUi(this);

	pres = new_pres;

	if (pres->exists) {
		this->setWindowTitle("Alter Prescriber Wizard");
		setField("lastField", pres->last);
		setField("firstField", pres->first);
		setField("fullField", pres->full_name);
		setField("activeField", pres->active);
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

PrescriberWizard::~PrescriberWizard()
{
	delete ui;
}

void PrescriberWizard::returnResults()
{
	ui->page0->getResults(pres);

	emit(wizardComplete(pres));
}

void PrescriberWizard::rejectedWizard()
{
	emit(wizardRejected(pres));
}
