/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "asw_page00.h"
#include "ui_asw_page00.h"

ASW_Page00::ASW_Page00(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ASW_Page00)
{
    ui->setupUi(this);
}

ASW_Page00::~ASW_Page00()
{
    delete ui;
}
