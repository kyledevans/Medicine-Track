/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "altermedicationwizard.h"
#include "ui_altermedicationwizard.h"

AlterMedicationWizard::AlterMedicationWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AlterMedicationWizard)
{
    ui->setupUi(this);
}

AlterMedicationWizard::~AlterMedicationWizard()
{
    delete ui;
}
