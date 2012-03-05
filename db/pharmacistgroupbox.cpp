/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistgroupbox.h"
#include "ui_pharmacistgroupbox.h"

#include "globals.h"

PharmacistGroupBox::PharmacistGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::PharmacistGroupBox)
{
	ui->setupUi(this);

	setStrings();
}

PharmacistGroupBox::~PharmacistGroupBox()
{
	delete ui;
}

void PharmacistGroupBox::setId(int new_id)
{
	QString temp;

	pharmacist.retrieve(new_id);	// TODO: does not handle query failure

	// Set values from pharmacist record
	ui->lastLabel->setText(pharmacist.getLast());
	ui->firstLabel->setText(pharmacist.getFirst());
	ui->initialsLabel->setText(pharmacist.getInitials());
	if (pharmacist.getActive()) {
		ui->activeLabel->setText("Active");
	} else {
		temp = QString("<font color='#%1'>%2</font>");
		ui->activeLabel->setText(temp.arg(MTCOLORS::Problem, QString("Inactive")));
	}
}

void PharmacistGroupBox::setStrings()
{
	// Set UI strings
	ui->lastTitle->setText(ui->lastTitle->text() + ":");
	ui->firstTitle->setText(ui->firstTitle->text() + ":");
	ui->initialsTitle->setText(ui->initialsTitle->text() + ":");
	ui->activeTitle->setText(ui->activeTitle->text() + ":");
}

void PharmacistGroupBox::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		setStrings();
		break;
	default:
		break;
	}
}
