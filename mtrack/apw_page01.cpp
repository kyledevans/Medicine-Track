/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "apw_page01.h"
#include "ui_apw_page01.h"

APW_Page01::APW_Page01(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::APW_Page01)
{
    ui->setupUi(this);
}

APW_Page01::~APW_Page01()
{
    delete ui;
}
