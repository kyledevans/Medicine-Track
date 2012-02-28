/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberframe.h"
#include "ui_prescriberframe.h"

#include "db/alertinterface.h"
#include "prescriberwizard.h"

PrescriberFrame::PrescriberFrame(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PrescriberFrame),
	db_queried(false)
{
	QTableWidgetItem *header;
	ui->setupUi(this);

	// Setup the search UI strings and tooltips
	ui->lastLabel->setText(PrescriberRecord::last_Label);
	ui->lastLabel->setToolTip(PrescriberRecord::last_Tooltip);
	ui->lastField->setToolTip(PrescriberRecord::last_Tooltip);

	ui->firstLabel->setText(PrescriberRecord::first_Label);
	ui->firstLabel->setToolTip(PrescriberRecord::first_Tooltip);
	ui->firstField->setToolTip(PrescriberRecord::first_Tooltip);

	// Setup resultTable UI strings and tooltips
	header = ui->resultTable->horizontalHeaderItem(0);
	header->setText(PrescriberRecord::last_Label);
	header->setToolTip(PrescriberRecord::last_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(1);
	header->setText(PrescriberRecord::first_Label);
	header->setToolTip(PrescriberRecord::first_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(2);
	header->setText(PrescriberRecord::full_name_Label);
	header->setToolTip(PrescriberRecord::full_name_Tooltip);

	// Setup signals/slots
	connect(ui->newAction, SIGNAL(triggered()), this, SLOT(initiateNew()));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->toggleAction, SIGNAL(triggered()), this, SLOT(toggleActive()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));

	// Add actions to the resultTable menu
	ui->resultTable->addAction(ui->modifyAction);
	ui->resultTable->addAction(ui->toggleAction);

	selectionChanged();
}

PrescriberFrame::~PrescriberFrame()
{
	delete ui;
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
	} else {
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
		ui->toggleAction->setEnabled(false);
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
	initiateSearch(pres->id);
	delete pres;
}

void PrescriberFrame::prescriberCleanup(PrescriberRecord *pres)
{
	delete pres;
}
