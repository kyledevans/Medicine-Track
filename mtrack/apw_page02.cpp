/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "apw_page02.h"
#include "ui_apw_page02.h"

APW_Page02::APW_Page02(QWidget *parent) :
    QWizardPage(parent),
	ui(new Ui::APW_Page02),
	prescription(0)
{
    ui->setupUi(this);
}

APW_Page02::~APW_Page02()
{
    delete ui;
}

void APW_Page02::setPrescription(PrescriptionRecord *new_prescription)
{
	prescription = new_prescription;
}

void APW_Page02::getResults()
{
	prescription->instructions = ui->instructionsField->toPlainText();
}
