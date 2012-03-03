/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistgroupbox.h"
#include "ui_pharmacistgroupbox.h"

#include "../globals.h"

PharmacistGroupBox::PharmacistGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::PharmacistGroupBox)
{
	ui->setupUi(this);

	// Set UI strings
	ui->lastTitle->setText(PharmacistRecord::last_Label + ":");
	ui->lastTitle->setToolTip(PharmacistRecord::last_Tooltip);
	ui->lastLabel->setToolTip(PharmacistRecord::last_Tooltip);

	ui->firstTitle->setText(PharmacistRecord::first_Label + ":");
	ui->firstTitle->setToolTip(PharmacistRecord::first_Tooltip);
	ui->firstLabel->setToolTip(PharmacistRecord::first_Tooltip);

	ui->initialsTitle->setText(PharmacistRecord::initials_Label + ":");
	ui->initialsTitle->setToolTip(PharmacistRecord::initials_Tooltip);
	ui->initialsLabel->setToolTip(PharmacistRecord::initials_Tooltip);

	ui->activeTitle->setText(PharmacistRecord::active_Label + ":");
	ui->activeTitle->setToolTip(PharmacistRecord::active_Tooltip);
	ui->activeLabel->setToolTip(PharmacistRecord::active_Tooltip);
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

void PharmacistGroupBox::changeEvent(QEvent *e)
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
