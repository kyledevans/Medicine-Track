/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescribergroupbox.h"
#include "ui_prescribergroupbox.h"

#include "../globals.h"

PrescriberGroupBox::PrescriberGroupBox(QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::PrescriberGroupBox)
{
	ui->setupUi(this);

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
}

PrescriberGroupBox::~PrescriberGroupBox()
{
	delete ui;
}

void PrescriberGroupBox::setId(int new_id)
{
	QString temp;

	prescriber.retrieve(new_id);	// TODO: Does not handle query failure

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
}
