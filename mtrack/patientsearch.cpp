/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QAction>
#include <QList>
#include <QTableWidgetItem>

#include <QDebug>

#include "patientsearch.h"
#include "ui_patientsearch.h"

#include "globals.h"
#include "patientrecord.h"
#include "newpatientwizard.h"
#include "alertinterface.h"
#include "alterprescriptionwizard.h"
#include "prescriptionlabel.h"

PatientSearch::PatientSearch(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PatientSearch),
	db_queried(false)
{
	QTableWidgetItem *header;
    ui->setupUi(this);

	// Setup the search UI strings and tooltips
	ui->lastNameLabel->setText(PatientRecord::last_Label);
	ui->lastNameLabel->setToolTip(PatientRecord::last_Tooltip);
	ui->lastNameField->setToolTip(PatientRecord::last_Tooltip);

	ui->firstNameLabel->setText(PatientRecord::first_Label);
	ui->firstNameLabel->setToolTip(PatientRecord::first_Tooltip);
	ui->firstNameField->setToolTip(PatientRecord::first_Tooltip);

	ui->dobLabel->setText(PatientRecord::dob_Label);
	ui->dobLabel->setToolTip(PatientRecord::dob_Tooltip);
	ui->dobField->setToolTip(PatientRecord::dob_Tooltip);

	// Setup the resultTable strings and tooltips
	header = ui->resultTable->horizontalHeaderItem(0);
	header->setText(PatientRecord::allscripts_id_Label);
	header->setToolTip(PatientRecord::allscripts_id_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(1);
	header->setText(PatientRecord::last_Label);
	header->setToolTip(PatientRecord::last_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(2);
	header->setText(PatientRecord::first_Label);
	header->setToolTip(PatientRecord::first_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(3);
	header->setText(PatientRecord::dob_Label);
	header->setToolTip(PatientRecord::dob_Tooltip);

	ui->dobField->setDate(DEFAULTS::Date);

	// Setup signals/slots
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->prescribeAction, SIGNAL(triggered()), this, SLOT(initiatePrescription()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModification()));
	connect(ui->newPatientAction, SIGNAL(triggered()), this, SLOT(initiateNewPatient()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

	// Add actions to resultTable right-click menu
	ui->resultTable->addAction(ui->prescribeAction);
	ui->resultTable->addAction(ui->modifyAction);

	// Deactivate actions that require an item selected in resultTable
	selectionChanged();
}

PatientSearch::~PatientSearch()
{
    delete ui;
}

/* SQL without C++:
SELECT id, allscripts_id, last, first, dob
FROM patients
WHERE last LIKE '%SOME_VAR%'
AND first LIKE '%SOME_VAR%'
AND dob = 'SOME_VAR'
AND active = '1';
*/
void PatientSearch::initiateSearch(int patientID)
{
	QSqlQuery *model;
	QString query;	// Holds the SQL query
	AlertInterface alert;
	int i;      // Increment var

	// If the text fields are empty, don't do anything.
	if (ui->firstNameField->text().isEmpty() &&
		ui->lastNameField->text().isEmpty() &&
		ui->firstNameField->text().isEmpty() &&
		(ui->dobField->date() == DEFAULTS::Date))
	{
		return;
	}

	model = new QSqlQuery;

	// Do a normal search when a specific patient ID hasn't been specified
	if (patientID == SQL::Undefined_ID) {
		query = QString("SELECT id, allscripts_id, last, first, dob FROM patients WHERE last LIKE '%");
		query += SQL::cleanInput(ui->lastNameField->text()) + QString("%' AND first LIKE '%");
		query += SQL::cleanInput(ui->firstNameField->text()) + QString("%' ");
		if (ui->dobField->date() != DEFAULTS::Date) {
			query += QString("AND dob = '");
			query += ui->dobField->date().toString("yyyy-MM-dd") + QString("' ");
		}
		query += QString("AND active = '1';");
	} else {	// Otherwise search for the specific patient
		query = QString("SELECT id, allscripts_id, last, first, dob FROM patients WHERE id = '");
		query += QString().setNum(patientID) + QString("';");
	}

	if (!alert.attemptQuery(model, &query)) {
		delete model;
		return;
	}

	ids.clear();
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(model->size());
	for (i = 0; i < model->size(); i++) {
		model->next();
		ids.append(model->value(0).toInt());
		ui->resultTable->setItem(i, 0, new QTableWidgetItem(model->value(1).toString()));
		ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(2).toString()));
		ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(3).toString()));
		ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(4).toDate().toString("MM-dd-yyyy")));
	}

	db_queried = true;	// Let other functions start accessing values in the table
	delete model;
}

void PatientSearch::selectionChanged()
{
	// Enable/disable buttons if there is a selection
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->prescribeButton->setEnabled(true);
		ui->modifyButton->setEnabled(true);
		ui->prescribeAction->setEnabled(true);
		ui->modifyAction->setEnabled(true);
	} else {
		ui->prescribeButton->setEnabled(false);
		ui->modifyButton->setEnabled(false);
		ui->prescribeAction->setEnabled(false);
		ui->modifyAction->setEnabled(false);
	}
}

void PatientSearch::resetPressed()
{
	ui->dobField->setDate(DEFAULTS::Date);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
}

void PatientSearch::initiatePrescription()
{
	unsigned int row;
	PrescriptionRecord *prescription;

	AlterPrescriptionWizard *wiz;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	wiz = new AlterPrescriptionWizard();

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!wiz->getPatient(ids[row])) {
		delete wiz;
		return;
	}

	prescription = new PrescriptionRecord;
	prescription->patient_id = ids[row];
	wiz->setPrescription(prescription);

	connect(wiz, SIGNAL(wizardComplete(PrescriptionRecord*)), this, SLOT(submitNewPrescription(PrescriptionRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PrescriptionRecord*)), this, SLOT(newPrescriptionCleanup(PrescriptionRecord*)));
	wiz->exec();
}

void PatientSearch::initiateModification()
{
	unsigned int row;
	NewPatientWizard *wiz;
	PatientRecord *patient;

	if (db_queried) {
		if (!ui->resultTable->selectionModel()->hasSelection()) {
			return;
		}
	} else {
		return;
	}

	patient = new PatientRecord();

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!patient->retrieve(ids[row])) {
		delete patient;
		return;
	}

	wiz = new NewPatientWizard(patient);
	connect(wiz, SIGNAL(wizardComplete(PatientRecord*)), this, SLOT(submitNewPatient(PatientRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PatientRecord*)), this, SLOT(newPatientCleanup(PatientRecord*)));
	wiz->exec();

	delete wiz;
}

void PatientSearch::initiateNewPatient()
{
	NewPatientWizard *wiz;
	PatientRecord *patient = new PatientRecord();

	wiz = new NewPatientWizard(patient);
	connect(wiz, SIGNAL(wizardComplete(PatientRecord*)), this, SLOT(submitNewPatient(PatientRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PatientRecord*)), this, SLOT(newPatientCleanup(PatientRecord*)));
	wiz->exec();

	delete wiz;
}

void PatientSearch::submitNewPrescription(PrescriptionRecord *prescription)
{
	prescription->commitRecord();
	//initiatePrint(prescription);
	newPrescriptionCleanup(prescription);
}

void PatientSearch::newPrescriptionCleanup(PrescriptionRecord *prescription)
{
	delete prescription;
}

void PatientSearch::submitNewPatient(PatientRecord *patient)
{
	patient->commitRecord();
	initiateSearch(patient->id);
	newPatientCleanup(patient);
}

void PatientSearch::newPatientCleanup(PatientRecord *patient)
{
	delete patient;
}

void PatientSearch::submitModify(PatientRecord *patient)
{
	// TODO: Implement this
}

void PatientSearch::initiatePrint(PrescriptionRecord *prescription)
{
	PrescriptionLabel *label;
	unsigned int row;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	label = new PrescriptionLabel(prescription);
	label->printLabel();

	delete label;
}
