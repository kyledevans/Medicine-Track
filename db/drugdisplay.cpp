/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "drugdisplay.h"
#include "ui_drugdisplay.h"

#include "globals.h"

DrugDisplay::DrugDisplay(int new_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DrugDisplay)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	ui->drugBox->setId(new_id);

	this->show();
}

DrugDisplay::~DrugDisplay()
{
	delete ui;
}

void DrugDisplay::changeEvent(QEvent *e)
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
