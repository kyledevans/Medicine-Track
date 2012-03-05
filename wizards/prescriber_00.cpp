/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriber_00.h"
#include "ui_prescriber_00.h"

Prescriber_00::Prescriber_00(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::Prescriber_00)
{
	ui->setupUi(this);

	registerField("lastField", ui->lastField);
	registerField("firstField", ui->firstField);
	registerField("fullField", ui->fullField);
}

Prescriber_00::~Prescriber_00()
{
	delete ui;
}

void Prescriber_00::getResults(PrescriberRecord *pres)
{
	pres->setLast(ui->lastField->text());
	pres->setFirst(ui->firstField->text());
	pres->setFull_name(ui->fullField->text());
}

void Prescriber_00::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
