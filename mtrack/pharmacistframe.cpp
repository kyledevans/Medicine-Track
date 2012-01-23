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

	ui->resultTable->addAction(ui->modifyAction);
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
	QSqlQueryModel *model;	// DB interface
	QString query;			// Holds the SQL query
	AlertInterface alert;	// Handles problems

	model = new QSqlQueryModel(ui->resultTable);

	if (pharmID == SQL::Undefined_ID) {
		query = QString("SELECT id, last, first, initials FROM pharmacists WHERE last LIKE '%");
		query += SQL::cleanInput(ui->lastField->text()) + QString("%' AND first LIKE '%");
		query += SQL::cleanInput(ui->firstField->text()) + QString("%' AND active = '1';");
	} else {
		query = QString("SELECT id, last, first, initials FROM pharmacists WHERE id = '");
		query += QString().setNum(pharmID) + QString("';");
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
