/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "aprescriberpage_page00.h"
#include "ui_aprescriberpage_page00.h"

APrescriberPage_Page00::APrescriberPage_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::APrescriberPage_Page00)
{
    ui->setupUi(this);
}

APrescriberPage_Page00::~APrescriberPage_Page00()
{
    delete ui;
}
