/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "nmw_page01.h"
#include "ui_nmw_page01.h"

NMW_Page01::NMW_Page01(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::NMW_Page01)
{
    ui->setupUi(this);

	registerField("instructionsField", ui->instructionsField, "plainText", SIGNAL(textChanged()));
}

NMW_Page01::~NMW_Page01()
{
    delete ui;
}

void NMW_Page01::getResults(MedicationRecord *med)
{
	med->instructions = ui->instructionsField->toPlainText();
}
