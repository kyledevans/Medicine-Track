/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberframe.h"
#include "ui_prescriberframe.h"

#include "alertinterface.h"
#include "prescriberdisplay.h"
#include "prescriberwizard.h"

PrescriberFrame::PrescriberFrame(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PrescriberFrame),
	db_queried(false)
{
	ui->setupUi(this);

	// Setup signals/slots
	connect(ui->newAction, SIGNAL(triggered()), this, SLOT(initiateNew()));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->toggleAction, SIGNAL(triggered()), this, SLOT(toggleActive()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->viewAction, SIGNAL(triggered()), this, SLOT(viewPrescriber()));

	// Add actions to the resultTable menu
	ui->resultTable->addAction(ui->modifyAction);
	ui->resultTable->addAction(ui->toggleAction);
	ui->resultTable->addAction(ui->viewAction);

	selectionChanged();
}

PrescriberFrame::~PrescriberFrame()
{
	delete ui;
}

void PrescriberFrame::changeEvent(QEvent *e)
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

void PrescriberFrame::viewPrescriber()
{
	unsigned int row;
	PrescriberDisplay *display;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	display = new PrescriberDisplay(ids[row]);
}

void PrescriberFrame::resetPressed()
{
	ui->activeField->setChecked(true);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
}

void PrescriberFrame::toggleActive()
{
	unsigned int row;
	PrescriberRecord prescriber;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	prescriber.retrieve(ids[row]);
	prescriber.toggleActive();
}

/* SQL without C++:
SELECT id, last, first, full_name
FROM prescribers
WHERE last LIKE ?
AND first LIKE ?
AND active = 1;
*/
void PrescriberFrame::initiateSearch(int presID)
{
	QSqlQuery *model;       // DB interface
	AlertInterface alert;	// Handles problems
	int i;                  // Increment var

	model = new QSqlQuery;

	if (presID == SQL::Undefined_ID) {
		model->prepare("SELECT id, last, first, full_name "
					   "FROM prescribers "
					   "WHERE last LIKE ? "
					   "AND first LIKE ? "
					   "AND active = ?;");
		model->bindValue(0, SQL::prepWildcards(ui->lastField->text()));
		model->bindValue(1, SQL::prepWildcards(ui->firstField->text()));
		model->bindValue(2, QVariant(ui->activeField->isChecked()));
	} else {
		model->prepare("SELECT id, last, first, full_name "
					   "FROM prescribers "
					   "WHERE id = ?;");
		model->bindValue(0, QVariant(presID));
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

void PrescriberFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->modifyButton->setEnabled(true);
		ui->modifyAction->setEnabled(true);
		ui->toggleAction->setEnabled(true);
		ui->viewAction->setEnabled(true);
	} else {
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
		ui->toggleAction->setEnabled(false);
		ui->viewAction->setEnabled(false);
	}
}

void PrescriberFrame::initiateNew()
{
	PrescriberWizard *wiz;
	PrescriberRecord *pres = new PrescriberRecord;

	wiz = new PrescriberWizard(pres);
	connect(wiz, SIGNAL(wizardComplete(PrescriberRecord*)), this, SLOT(submitPrescriber(PrescriberRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PrescriberRecord*)), this, SLOT(prescriberCleanup(PrescriberRecord*)));
	wiz->exec();

	delete wiz;
}

void PrescriberFrame::initiateModify()
{
	unsigned int row;
	PrescriberWizard *wiz;
	PrescriberRecord *pres;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	pres = new PrescriberRecord;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!pres->retrieve(ids[row])) {
		delete pres;
		return;
	}

	wiz = new PrescriberWizard(pres);
	connect(wiz, SIGNAL(wizardComplete(PrescriberRecord*)), this, SLOT(submitPrescriber(PrescriberRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PrescriberRecord*)), this, SLOT(prescriberCleanup(PrescriberRecord*)));
	wiz->exec();

	delete wiz;
}

void PrescriberFrame::submitPrescriber(PrescriberRecord *pres)
{
	pres->commitRecord();
	initiateSearch(pres->getId());
	delete pres;
}

void PrescriberFrame::prescriberCleanup(PrescriberRecord *pres)
{
	delete pres;
}
