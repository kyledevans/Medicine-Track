/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "npw_page00.h"
#include "ui_npw_page00.h"

NPW_Page00::NPW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NPW_Page00)
{
    ui->setupUi(this);
}

NPW_Page00::~NPW_Page00()
{
    delete ui;
}
