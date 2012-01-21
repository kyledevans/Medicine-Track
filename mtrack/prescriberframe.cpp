/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberframe.h"
#include "ui_prescriberframe.h"

#include "alterprescriberwizard.h"
#include "prescriberrecord.h"

PrescriberFrame::PrescriberFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PrescriberFrame)
{
    ui->setupUi(this);
}

PrescriberFrame::~PrescriberFrame()
{
    delete ui;
}

void PrescriberFrame::initiateNew()
{
	AlterPrescriberWizard *wiz;
	PrescriberRecord *pres = new PrescriberRecord();

	wiz = new AlterPrescriberWizard(pres);
	wiz->exec();

	wiz->returnResults();
	pres->print();

	delete wiz;
	delete pres;
}
