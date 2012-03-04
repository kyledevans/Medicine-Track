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

	setStrings();
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

void PrescriberGroupBox::setStrings()
{
	// Set UI strings
	ui->lastTitle->setText(ui->lastTitle->text() + ":");
	ui->firstTitle->setText(ui->firstTitle->text() + ":");
	ui->fullTitle->setText(ui->fullTitle->text() + ":");
	ui->activeTitle->setText(ui->activeTitle->text() + ":");
}

void PrescriberGroupBox::changeEvent(QEvent *e)
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
