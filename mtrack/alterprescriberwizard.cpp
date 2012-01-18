/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "alterprescriberwizard.h"
#include "ui_alterprescriberwizard.h"

AlterPrescriberWizard::AlterPrescriberWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::AlterPrescriberWizard)
{
    ui->setupUi(this);
}

AlterPrescriberWizard::~AlterPrescriberWizard()
{
    delete ui;
}
