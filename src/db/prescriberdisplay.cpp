/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberdisplay.h"
#include "ui_prescriberdisplay.h"

#include "../globals.h"

PrescriberDisplay::PrescriberDisplay(int new_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PrescriberDisplay)
{
	QString temp;
	ui->setupUi(this);

	prescriber.retrieve(new_id);	// TODO: Does not handle query failure
	setAttribute(Qt::WA_DeleteOnClose);

	// Set UI strings
	ui->lastTitle->setText(PrescriberRecord::last_Label + ":");
	ui->lastTitle->setToolTip(PrescriberRecord::last_Tooltip);
	ui->lastLabel->setToolTip(PrescriberRecord::last_Tooltip);

	ui->firstTitle->setText(PrescriberRecord::first_Label + ":");
	ui->firstTitle->setToolTip(PrescriberRecord::first_Tooltip);
	ui->firstLabel->setToolTip(PrescriberRecord::first_Tooltip);

	ui->fullTitle->setText(PrescriberRecord::full_name_Label + ":");
	ui->fullTitle->setToolTip(PrescriberRecord::full_name_Tooltip);
	ui->fullLabel->setToolTip(PrescriberRecord::full_name_Tooltip);

	ui->activeTitle->setText(PrescriberRecord::active_Label + ":");
	ui->activeTitle->setToolTip(PrescriberRecord::active_Tooltip);
	ui->activeLabel->setToolTip(PrescriberRecord::active_Tooltip);

	// Set fields from prescriber record
	ui->lastLabel->setText(prescriber.getLast());
	ui->firstLabel->setText(prescriber.getFirst());
	ui->fullLabel->setText(prescriber.getFull_name());
	if (prescriber.getActive()) {
		ui->activeLabel->setText("Active");
	} else {
		temp = QString("<font color='#%1'>%2</font>");
		ui->activeLabel->setText(temp.arg(MTCOLORS::Problem, QString("Inactive")));
	}

	this->show();
}

PrescriberDisplay::~PrescriberDisplay()
{
	delete ui;
}
