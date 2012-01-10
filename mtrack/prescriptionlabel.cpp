/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptionlabel.h"
#include "ui_prescriptionlabel.h"

PrescriptionLabel::PrescriptionLabel(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PrescriptionLabel)
{
    ui->setupUi(this);
}

PrescriptionLabel::~PrescriptionLabel()
{
    delete ui;
}
