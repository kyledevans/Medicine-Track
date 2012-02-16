/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
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
	ui->resultTable->addAction(ui->modifyAction);

	connect(ui->newAction, SIGNAL(triggered()), this, SLOT(initiateNew()));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

	selectionChanged();
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
	QSqlQuery *model;       // DB interface
	QString query;			// Holds the SQL query
	AlertInterface alert;	// Handles problems
	int i;                  // Increment var

	if (presID == SQL::Undefined_ID) {
		query = QString("SELECT id, last, first, full_name FROM prescribers WHERE last LIKE '%");
		query += SQL::cleanInput(ui->lastField->text()) + QString("%' AND first LIKE '%");
		query += SQL::cleanInput(ui->firstField->text()) + QString("%' AND active = '1';");
	} else {
		query = QString("SELECT id, last, first, full_name FROM prescribers WHERE id = '");
		query += QString().setNum(presID) + QString("';");
	}

	model = new QSqlQuery;

	if (!alert.attemptQuery(model, &query)) {	// On error, cleanup and exit
		delete model;
		return;
	}

	ids.clear();
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(model->size());
	for (i = 0; i < model->size(); i++) {
		model->next();
		ids.append(model->value(0).toInt());	// Retrieve the ID's before they get deleted
		ui->resultTable->setItem(i, 0, new QTableWidgetItem(model->value(1).toString()));
		ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(2).toString()));
		ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(3).toString()));
	}

	db_queried = true;
	delete model;
}

void PrescriberFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->modifyButton->setEnabled(true);
		ui->modifyAction->setEnabled(true);
	} else {
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
	}
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
