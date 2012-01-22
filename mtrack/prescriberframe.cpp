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

	connect(ui->newAction, SIGNAL(triggered()), this, SLOT(initiateNew()));
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
	connect(wiz, SIGNAL(wizardComplete(PrescriberRecord*)), this, SLOT(submitNew(PrescriberRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PrescriberRecord*)), this, SLOT(newCleanup(PrescriberRecord*)));
	wiz->exec();

	delete wiz;
}

void PrescriberFrame::submitNew(PrescriberRecord *pres)
{
	pres->commitRecord();
	newCleanup(pres);
}

void PrescriberFrame::newCleanup(PrescriberRecord *pres)
{
	delete pres;
}
