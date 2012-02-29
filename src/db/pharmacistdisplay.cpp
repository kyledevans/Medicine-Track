/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistdisplay.h"
#include "ui_pharmacistdisplay.h"

#include "../globals.h"

PharmacistDisplay::PharmacistDisplay(int new_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PharmacistDisplay)
{
	QString temp;
	ui->setupUi(this);

	pharmacist.retrieve(new_id);	// TODO: does not handle query failure
	setAttribute(Qt::WA_DeleteOnClose);

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

	this->show();
}

PharmacistDisplay::~PharmacistDisplay()
{
	delete ui;
}
