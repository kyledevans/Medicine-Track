/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QList>

#include "pharmacistframe.h"
#include "ui_pharmacistframe.h"

#include "alterpharmacistwizard.h"
#include "pharmacistrecord.h"
#include "alertinterface.h"

PharmacistFrame::PharmacistFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::PharmacistFrame),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->newAction, SIGNAL(triggered()), this, SLOT(initiateNew()));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

	ui->resultTable->addAction(ui->modifyAction);

	selectionChanged();
}

PharmacistFrame::~PharmacistFrame()
{
    delete ui;
}

/* SQL without C++:
SELECT id, last, first, initials
FROM pharmacists
WHERE last LIKE '%SOME_VAR%'
AND first LIKE '%SOME_VAR%'
AND active = '1';
*/
void PharmacistFrame::initiateSearch(int pharmID)
{
	QSqlQuery *model;		// DB interface
	QString query;			// Holds the SQL query
	AlertInterface alert;	// Handles problems
	int i;					// Increment var

	if (pharmID == SQL::Undefined_ID) {
		query = QString("SELECT id, last, first, initials FROM pharmacists WHERE last LIKE '%");
		query += SQL::cleanInput(ui->lastField->text()) + QString("%' AND first LIKE '%");
		query += SQL::cleanInput(ui->firstField->text()) + QString("%' AND active = '1';");
	} else {
		query = QString("SELECT id, last, first, initials FROM pharmacists WHERE id = '");
		query += QString().setNum(pharmID) + QString("';");
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

void PharmacistFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->modifyButton->setEnabled(true);
		ui->modifyAction->setEnabled(true);
	} else {
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
	}
}

void PharmacistFrame::initiateNew()
{
	AlterPharmacistWizard *wiz;
	PharmacistRecord *pharm = new PharmacistRecord();

	wiz = new AlterPharmacistWizard(pharm);
	connect(wiz, SIGNAL(wizardComplete(PharmacistRecord*)), this, SLOT(submitNew(PharmacistRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PharmacistRecord*)), this, SLOT(newCleanup(PharmacistRecord*)));
	wiz->exec();

	delete wiz;
}

void PharmacistFrame::initiateModify()
{
	unsigned int row;
	AlterPharmacistWizard *wiz;
	PharmacistRecord *pharm;

	if (db_queried) {
		if (!ui->resultTable->selectionModel()->hasSelection()) {
			return;
		}
	} else {
		return;
	}

	pharm = new PharmacistRecord();

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!pharm->retrieve(ids[row])) {
		delete pharm;
		return;
	}

	wiz = new AlterPharmacistWizard(pharm);
	connect(wiz, SIGNAL(wizardComplete(PharmacistRecord*)), this, SLOT(submitNew(PharmacistRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PharmacistRecord*)), this, SLOT(newCleanup(PharmacistRecord*)));
	wiz->exec();

	delete wiz;
}

void PharmacistFrame::submitNew(PharmacistRecord *pharm)
{
	pharm->commitRecord();
	initiateSearch(pharm->id);
	newCleanup(pharm);
}

void PharmacistFrame::newCleanup(PharmacistRecord *pharm)
{
	delete pharm;
}
