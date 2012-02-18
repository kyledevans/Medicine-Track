/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "alterpharmacistwizard.h"
#include "ui_alterpharmacistwizard.h"

AlterPharmacistWizard::AlterPharmacistWizard(QWidget *parent) :
    QWizard(parent),
	ui(new Ui::AlterPharmacistWizard),
	pharm(0)
{
    ui->setupUi(this);
}

AlterPharmacistWizard::AlterPharmacistWizard(PharmacistRecord *new_pharm, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::AlterPharmacistWizard)
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

AlterPharmacistWizard::~AlterPharmacistWizard()
{
    delete ui;
}

void AlterPharmacistWizard::returnResults()
{
	ui->page0->getResults(pharm);

	emit(wizardComplete(pharm));
}

void AlterPharmacistWizard::rejectedWizard()
{
	emit(wizardRejected(pharm));
}
