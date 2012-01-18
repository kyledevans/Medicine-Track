/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistframe.h"
#include "ui_pharmacistframe.h"

PharmacistFrame::PharmacistFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PharmacistFrame)
{
    ui->setupUi(this);
}

PharmacistFrame::~PharmacistFrame()
{
    delete ui;
}
