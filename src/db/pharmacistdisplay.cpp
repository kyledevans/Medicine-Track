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
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	ui->pharmacistBox->setId(new_id);

	this->show();
}

PharmacistDisplay::~PharmacistDisplay()
{
	delete ui;
}

void PharmacistDisplay::changeEvent(QEvent *e)
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
