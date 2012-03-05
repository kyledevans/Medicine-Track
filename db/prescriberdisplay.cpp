/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberdisplay.h"
#include "ui_prescriberdisplay.h"

#include "globals.h"

PrescriberDisplay::PrescriberDisplay(int new_id, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PrescriberDisplay)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
	ui->prescriberBox->setId(new_id);

	this->show();
}

PrescriberDisplay::~PrescriberDisplay()
{
	delete ui;
}

void PrescriberDisplay::changeEvent(QEvent *e)
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
