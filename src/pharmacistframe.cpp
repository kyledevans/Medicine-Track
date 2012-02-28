/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistframe.h"
#include "ui_pharmacistframe.h"

#include "db/alertinterface.h"
#include "pharmacistwizard.h"

#include <QDebug>

PharmacistFrame::PharmacistFrame(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PharmacistFrame),
	db_queried(false)
{
	QTableWidgetItem *header;
	ui->setupUi(this);

	// Setup the search UI strings and tooltips
	ui->lastLabel->setText(PharmacistRecord::last_Label);
	ui->lastLabel->setToolTip(PharmacistRecord::last_Tooltip);
	ui->lastField->setToolTip(PharmacistRecord::last_Tooltip);

	ui->firstLabel->setText(PharmacistRecord::first_Label);
	ui->firstLabel->setToolTip(PharmacistRecord::first_Tooltip);
	ui->firstField->setToolTip(PharmacistRecord::first_Tooltip);

	// Setup the resultTable strings and tooltips
	header = ui->resultTable->horizontalHeaderItem(0);
	header->setText(PharmacistRecord::last_Label);
	header->setToolTip(PharmacistRecord::last_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(1);
	header->setText(PharmacistRecord::first_Label);
	header->setToolTip(PharmacistRecord::first_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(2);
	header->setText(PharmacistRecord::initials_Label);
	header->setToolTip(PharmacistRecord::initials_Tooltip);

	// Setup signals/slots
	connect(ui->newAction, SIGNAL(triggered()), this, SLOT(initiateNew()));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->toggleAction, SIGNAL(triggered()), this, SLOT(toggleActive()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));

	// Add items to the resultTable right-click menu
	ui->resultTable->addAction(ui->modifyAction);
	ui->resultTable->addAction(ui->toggleAction);

	selectionChanged();
}

PharmacistFrame::~PharmacistFrame()
{
	delete ui;
}

void PharmacistFrame::resetPressed()
{
	ui->activeField->setChecked(true);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
}

void PharmacistFrame::toggleActive()
{
	unsigned int row;
	PharmacistRecord pharmacist;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	pharmacist.retrieve(ids[row]);
	pharmacist.toggleActive();
}

/* SQL without C++:
SELECT id, last, first, initials
FROM pharmacists
WHERE last LIKE ?
AND first LIKE ?
AND active = ?;
*/
void PharmacistFrame::initiateSearch(int pharmID)
{
	QSqlQuery *model;		// DB interface
	AlertInterface alert;	// Handles problems
	int i;					// Increment var

	model = new QSqlQuery;

	if (pharmID == SQL::Undefined_ID) {
		model->prepare("SELECT id, last, first, initials "
					   "FROM pharmacists "
					   "WHERE last LIKE ? "
					   "AND first LIKE ? "
					   "AND active = ?;");
		model->bindValue(0, SQL::prepWildcards(ui->lastField->text()));
		model->bindValue(1, SQL::prepWildcards(ui->firstField->text()));
		model->bindValue(2, QVariant(ui->activeField->isChecked()));
	} else {
		model->prepare("SELECT id, last, first, initials "
					   "FROM pharmacists "
					   "WHERE id = ?;");
		model->bindValue(0, QVariant(pharmID));
	}

	if (!alert.attemptQuery(model)) {	// On error, cleanup and exit
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
		ui->toggleAction->setEnabled(true);
	} else {
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
		ui->toggleAction->setEnabled(false);
	}
}

void PharmacistFrame::initiateNew()
{
	PharmacistWizard *wiz;
	PharmacistRecord *pharm = new PharmacistRecord;

	wiz = new PharmacistWizard(pharm);
	connect(wiz, SIGNAL(wizardComplete(PharmacistRecord*)), this, SLOT(submitPharmacist(PharmacistRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PharmacistRecord*)), this, SLOT(pharmacistCleanup(PharmacistRecord*)));
	wiz->exec();

	delete wiz;
}

void PharmacistFrame::initiateModify()
{
	unsigned int row;
	PharmacistWizard *wiz;
	PharmacistRecord *pharm;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	pharm = new PharmacistRecord;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!pharm->retrieve(ids[row])) {
		delete pharm;
		return;
	}

	wiz = new PharmacistWizard(pharm);
	connect(wiz, SIGNAL(wizardComplete(PharmacistRecord*)), this, SLOT(submitPharmacist(PharmacistRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PharmacistRecord*)), this, SLOT(pharmacistCleanup(PharmacistRecord*)));
	wiz->exec();

	delete wiz;
}

void PharmacistFrame::submitPharmacist(PharmacistRecord *pharm)
{
	pharm->commitRecord();
	initiateSearch(pharm->getId());
	delete pharm;
}

void PharmacistFrame::pharmacistCleanup(PharmacistRecord *pharm)
{
	delete pharm;
}
