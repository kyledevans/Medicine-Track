/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "alterprescriptionwizard.h"
#include "ui_alterprescriptionwizard.h"

AlterPrescriptionWizard::AlterPrescriptionWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AlterPrescriptionWizard)
{
    ui->setupUi(this);
}

AlterPrescriptionWizard::~AlterPrescriptionWizard()
{
    delete ui;
}
