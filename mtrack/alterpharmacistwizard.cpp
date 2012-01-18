/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "alterpharmacistwizard.h"
#include "ui_alterpharmacistwizard.h"

AlterPharmacistWizard::AlterPharmacistWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AlterPharmacistWizard)
{
    ui->setupUi(this);
}

AlterPharmacistWizard::~AlterPharmacistWizard()
{
    delete ui;
}
