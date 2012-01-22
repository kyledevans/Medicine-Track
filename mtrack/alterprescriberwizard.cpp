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

AlterPrescriberWizard::AlterPrescriberWizard(PrescriberRecord *new_pres, QWidget *parent) :
	QWizard(parent),
	ui(new Ui::AlterPrescriberWizard)
{
	ui->setupUi(this);

	pres = new_pres;
}

AlterPrescriberWizard::~AlterPrescriberWizard()
{
    delete ui;
}

void AlterPrescriberWizard::returnResults()
{
	ui->page00->getResults(pres);
}
