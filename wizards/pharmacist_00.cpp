/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacist_00.h"
#include "ui_pharmacist_00.h"

Pharmacist_00::Pharmacist_00(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::Pharmacist_00)
{
	ui->setupUi(this);

	registerField("lastField", ui->lastField);
	registerField("firstField", ui->firstField);
	registerField("initialsField", ui->initialsField);
}

Pharmacist_00::~Pharmacist_00()
{
	delete ui;
}

void Pharmacist_00::getResults(PharmacistRecord *pharm)
{
	pharm->setLast(ui->lastField->text());
	pharm->setFirst(ui->firstField->text());
	pharm->setInitials(ui->initialsField->text());
}

void Pharmacist_00::changeEvent(QEvent *e)
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
