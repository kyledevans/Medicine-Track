/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientframe.h"
#include "ui_patientframe.h"

#include <QVariant>

#include "alertinterface.h"
#include "patientdisplay.h"
#include "patientwizard.h"
#include "prescriptionwizard.h"
#include "prescriptionlabel.h"

#include <QDebug>

PatientFrame::PatientFrame(QWidget *parent) :
	QFrame(parent),
    ui(new Ui::PatientFrame),
	db_queried(false)
{
	ui->setupUi(this);

	ui->dobField->setDate(DEFAULTS::Date);

	// Setup signals/slots
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->prescribeAction, SIGNAL(triggered()), this, SLOT(initiatePrescription()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModification()));
	connect(ui->newPatientAction, SIGNAL(triggered()), this, SLOT(initiateNewPatient()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->toggleAction, SIGNAL(triggered()), this, SLOT(toggleActive()));
	connect(ui->viewAction, SIGNAL(triggered()), this, SLOT(viewPatient()));

	// Add actions to resultTable right-click menu
	ui->resultTable->addAction(ui->prescribeAction);
	ui->resultTable->addAction(ui->modifyAction);
	ui->resultTable->addAction(ui->toggleAction);
	ui->resultTable->addAction(ui->viewAction);

	// Hide the column with internal id's from the user
	ui->resultTable->hideColumn(0);

	// Deactivate actions that require an item selected in resultTable
	selectionChanged();
}

PatientFrame::~PatientFrame()
{
	delete ui;
}

void PatientFrame::changeEvent(QEvent *e)
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

void PatientFrame::viewPatient()
{
	int row;
	PatientDisplay *display;

	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	display = new PatientDisplay(ui->resultTable->item(row, 0)->text().toInt(), this);
}

// TODO: this deals with ids[]
void PatientFrame::toggleActive()
{
	int row;
	PatientRecord patient;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	patient.retrieve(ui->resultTable->item(row, 0)->text().toInt());
	patient.toggleActive();
}

/* SQL without C++:
SELECT id, allscripts_id, last, first, dob
FROM patients
WHERE last LIKE ?
AND first LIKE ?
AND (<true if NOT searching by DOB> OR (dob = ?))
AND active = ?
*/
void PatientFrame::initiateSearch(int patientID)
{
	QSqlQuery *model;
	AlertInterface alert;
	int i;      // Increment var
	bool dont_search_dob = true;

	// If the text fields are empty, don't do anything.
	if (ui->firstNameField->text().isEmpty() &&
		ui->lastNameField->text().isEmpty() &&
		ui->firstNameField->text().isEmpty() &&
		(ui->dobField->date() == DEFAULTS::Date) &&
		(patientID == SQL::Undefined_ID))
	{
		if (ui->resultTable->rowCount() > 0) {
			ui->resetButton->click();
		}
		return;
	}

	model = new QSqlQuery;

	// Do a normal search when a specific patient ID hasn't been specified
	if (patientID == SQL::Undefined_ID) {
		model->prepare("SELECT id, allscripts_id, last, first, dob "
					   "FROM patients "
					   "WHERE last LIKE ? "
					   "AND first LIKE ? "
					   "AND (? OR (dob = ?)) "
					   "AND active = ?");
		model->bindValue(0, SQL::prepWildcards(ui->lastNameField->text()));
		model->bindValue(1, SQL::prepWildcards(ui->firstNameField->text()));
		if (ui->dobField->date() != DEFAULTS::Date) {
			dont_search_dob = false;
		}
		model->bindValue(2, QVariant(dont_search_dob));
		model->bindValue(3, QVariant(ui->dobField->date()));
		model->bindValue(4, QVariant(ui->activeField->isChecked()));
	} else {	// Otherwise search for the specific patient
		model->prepare("SELECT id, allscripts_id, last, first, dob "
					   "FROM patients "
					   "WHERE id = ?;");
		model->bindValue(0, QVariant(patientID));
	}

	if (!alert.attemptQuery(model)) {
		delete model;
		return;
	}

	ui->resultTable->clearContents();
	ui->resultTable->setSortingEnabled(false);
	ui->resultTable->setRowCount(model->size());
	for (i = 0; i < model->size(); i++) {
		model->next();
		ui->resultTable->setItem(i, 0, new QTableWidgetItem(model->value(0).toString()));
		ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(1).toString()));
		ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(2).toString()));
		ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(3).toString()));
		ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(4).toDate().toString(DEFAULTS::DateDisplayFormat)));
	}
	ui->resultTable->setSortingEnabled(true);
	ui->resultTable->sortByColumn(2, Qt::AscendingOrder);

	db_queried = true;	// Let other functions start accessing values in the table
	delete model;
}

void PatientFrame::selectionChanged()
{
	// Enable/disable buttons if there is a selection
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->prescribeButton->setEnabled(true);
		ui->prescribeAction->setEnabled(true);
		ui->modifyButton->setEnabled(true);
		ui->modifyAction->setEnabled(true);
		ui->toggleAction->setEnabled(true);
		ui->viewAction->setEnabled(true);
	} else {
		ui->prescribeButton->setEnabled(false);
		ui->prescribeAction->setEnabled(false);
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
		ui->toggleAction->setEnabled(false);
		ui->viewAction->setEnabled(false);
	}
}

void PatientFrame::resetPressed()
{
	ui->dobField->setDate(DEFAULTS::Date);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
}

// TODO: This deals with ids[]
void PatientFrame::initiatePrescription()
{
	unsigned int row;
	PrescriptionRecord *prescription;

	PrescriptionWizard *wiz;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	wiz = new PrescriptionWizard;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!wiz->getPatient(ui->resultTable->item(row, 0)->text().toInt())) {
		delete wiz;
		return;
	}

	prescription = new PrescriptionRecord;
	prescription->setPatient_id(ui->resultTable->item(row, 0)->text().toInt());
	wiz->setPrescription(prescription);

	connect(wiz, SIGNAL(wizardComplete(PrescriptionRecord*)), this, SLOT(submitNewPrescription(PrescriptionRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PrescriptionRecord*)), this, SLOT(newPrescriptionCleanup(PrescriptionRecord*)));
	wiz->exec();
}

void PatientFrame::initiateModification()
{
	unsigned int row;
    PatientWizard *wiz;
	PatientRecord *patient;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	patient = new PatientRecord;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!patient->retrieve(ui->resultTable->item(row, 0)->text().toInt())) {
		delete patient;
		return;
	}

    wiz = new PatientWizard(patient);
	connect(wiz, SIGNAL(wizardComplete(PatientRecord*)), this, SLOT(submitPatient(PatientRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PatientRecord*)), this, SLOT(patientCleanup(PatientRecord*)));
	wiz->exec();

	delete wiz;
}

void PatientFrame::initiateNewPatient()
{
    PatientWizard *wiz;
	PatientRecord *patient = new PatientRecord;

    wiz = new PatientWizard(patient);
	connect(wiz, SIGNAL(wizardComplete(PatientRecord*)), this, SLOT(submitPatient(PatientRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PatientRecord*)), this, SLOT(patientCleanup(PatientRecord*)));
	wiz->exec();

	delete wiz;
}

void PatientFrame::submitPrescription(PrescriptionRecord *prescription)
{
	PrescriptionLabel label(prescription);
	if (prescription->commitRecord()) {
		label.print();
	}
	prescriptionCleanup(prescription);
}

void PatientFrame::prescriptionCleanup(PrescriptionRecord *prescription)
{
	delete prescription;
}

void PatientFrame::submitPatient(PatientRecord *patient)
{
	patient->commitRecord();
	initiateSearch(patient->getId());
	delete patient;
}

void PatientFrame::patientCleanup(PatientRecord *patient)
{
	delete patient;
}
