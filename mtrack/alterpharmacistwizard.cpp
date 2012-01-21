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
