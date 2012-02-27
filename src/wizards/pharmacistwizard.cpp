/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistwizard.h"
#include "ui_pharmacistwizard.h"

PharmacistWizard::PharmacistWizard(QWidget *parent) :
	QWizard(parent),
	ui(new Ui::PharmacistWizard),
	pharm(0)
{
	ui->setupUi(this);
}

PharmacistWizard::PharmacistWizard(PharmacistRecord *new_pharm, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::PharmacistWizard)
{
	ui->setupUi(this);

	pharm = new_pharm;

	if (pharm->exists) {
		this->setWindowTitle("Alter Pharmacist Wizard");
		setField("lastField", pharm->last);
		setField("firstField", pharm->first);
		setField("initialsField", pharm->initials);
		setField("activeField", pharm->active);
	}

	connect(this, SIGNAL(accepted()), this, SLOT(returnResults()));
	connect(this, SIGNAL(rejected()), this, SLOT(rejectedWizard()));
}

PharmacistWizard::~PharmacistWizard()
{
	delete ui;
}

void PharmacistWizard::returnResults()
{
	ui->page0->getResults(pharm);

	emit(wizardComplete(pharm));
}

void PharmacistWizard::rejectedWizard()
{
	emit(wizardRejected(pharm));
}
