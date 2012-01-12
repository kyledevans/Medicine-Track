/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "nmw_page00.h"
#include "ui_nmw_page00.h"

NMW_Page00::NMW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NMW_Page00)
{
    ui->setupUi(this);
}

NMW_Page00::~NMW_Page00()
{
    delete ui;
}
