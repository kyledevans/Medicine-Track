/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QAction>
#include <QMenu>
#include <QList>

#include "prescriberframe.h"
#include "ui_prescriberframe.h"

#include "alterprescriberwizard.h"
#include "prescriberrecord.h"
#include "alertinterface.h"
#include "globals.h"

PrescriberFrame::PrescriberFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::PrescriberFrame),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->newAction, SIGNAL(triggered()), this, SLOT(initiateNew()));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));

	ui->resultTable->addAction(ui->modifyAction);
}

PrescriberFrame::~PrescriberFrame()
{
    delete ui;
}

/* SQL without C++:
SELECT id, last, first, full_name
FROM prescribers
WHERE last LIKE '%SOME_VAR%'
AND first LIKE '%SOME_VAR%'
AND active = '1';
*/
void PrescriberFrame::initiateSearch(int presID)
{
	QSqlQueryModel *model;	// DB interface
	QString query;			// Holds the SQL query
	AlertInterface alert;	// Handles problems

	model = new QSqlQueryModel(ui->resultTable);

	if (presID == SQL::Undefined_ID) {
		query = QString("SELECT id, last, first, full_name FROM prescribers WHERE last LIKE '%");
		query += SQL::cleanInput(ui->lastField->text()) + QString("%' AND first LIKE '%");
		query += SQL::cleanInput(ui->firstField->text()) + QString("%' AND active = '1';");
	} else {
		query = QString("SELECT id, last, first, full_name FROM prescribers WHERE id = '");
		query += QString().setNum(presID) + QString("';");
	}

	if (!alert.attemptQuery(model, &query)) {	// On error, cleanup and exit
		delete model;
		return;
	}

	ids.clear();
	// Retrieve the ID's before we remove them from the display
	for (int i = 0; i < model->rowCount(); i++) {
		ids.append(model->record(i).value("id").toInt());
	}

	model->removeColumn(0);
	model->setHeaderData(0, Qt::Horizontal, tr("Last Name"));
	model->setHeaderData(1, Qt::Horizontal, tr("First Name"));
	model->setHeaderData(2, Qt::Horizontal, tr("Initials"));

	ui->resultTable->setModel(model);
	db_queried = true;
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
	initiateSearch(pres->id);
	newCleanup(pres);
}

void PrescriberFrame::newCleanup(PrescriberRecord *pres)
{
	delete pres;
}

void PrescriberFrame::initiateModify()
{
	unsigned int row;
	AlterPrescriberWizard *wiz;
	PrescriberRecord *pres;

	if (db_queried) {
		if (!ui->resultTable->selectionModel()->hasSelection()) {
			return;
		}
	} else {
		return;
	}

	pres = new PrescriberRecord();

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!pres->retrieve(ids[row])) {
		delete pres;
		return;
	}

	wiz = new AlterPrescriberWizard(pres);
	connect(wiz, SIGNAL(wizardComplete(PrescriberRecord*)), this, SLOT(submitNew(PrescriberRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PrescriberRecord*)), this, SLOT(newCleanup(PrescriberRecord*)));
	wiz->exec();

	delete wiz;
}
