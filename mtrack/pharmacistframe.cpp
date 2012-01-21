/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistframe.h"
#include "ui_pharmacistframe.h"

#include "alterpharmacistwizard.h"
#include "pharmacistrecord.h"

PharmacistFrame::PharmacistFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PharmacistFrame)
{
    ui->setupUi(this);

	connect(ui->newAction, SIGNAL(triggered()), this, SLOT(initiateNew()));
}

PharmacistFrame::~PharmacistFrame()
{
    delete ui;
}

void PharmacistFrame::initiateNew()
{
	AlterPharmacistWizard *wiz;
	PharmacistRecord *pharm = new PharmacistRecord();

	wiz = new AlterPharmacistWizard(pharm);
	wiz->exec();

	wiz->returnResults();
	pharm->print();

	delete wiz;
	delete pharm;
}
