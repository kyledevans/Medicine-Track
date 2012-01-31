/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QAction>
#include <QMenu>
#include <QList>

#include <QDebug>

#include "patientsearch.h"
#include "ui_patientsearch.h"

#include "globals.h"
#include "patientrecord.h"
#include "newpatientwizard.h"
#include "alertinterface.h"
#include "alterprescriptionwizard.h"

PatientSearch::PatientSearch(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PatientSearch),
	db_queried(false)
{
    ui->setupUi(this);

	ui->dobField->setDate(DEFAULTS::Date);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->prescribeAction, SIGNAL(triggered()), this, SLOT(initiatePrescription()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModification()));
	connect(ui->newPatientAction, SIGNAL(triggered()), this, SLOT(initiateNewPatient()));

	ui->resultTable->addAction(ui->prescribeAction);
	ui->resultTable->addAction(ui->modifyAction);
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
	QSqlQueryModel *model;
	QString query;	// Holds the SQL query
	AlertInterface alert;

	// If the text fields are empty, don't do anything.
	if ((ui->firstNameField->text().isEmpty()) &&
		(ui->lastNameField->text().isEmpty()) &&
		(ui->dobField->date() == DEFAULTS::Date))
	{
		return;
	}

	model = new QSqlQueryModel(ui->resultTable);

	// Do a normal search when a specific patient ID hasn't been specified
	if (patientID == SQL::Undefined_ID) {
		query = QString("SELECT id, allscripts_id, last, first, dob FROM patients WHERE last LIKE '%");
		query += SQL::cleanInput(ui->firstNameField->text()) + QString("%' AND last LIKE '%");
		query += SQL::cleanInput(ui->lastNameField->text()) + QString("%' ");
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
	// Retrieve the ID's before we remove them from the display
	for (int i = 0; i < model->rowCount(); i++) {
		ids.append(model->record(i).value("id").toInt());
	}

	model->removeColumn(0);
	model->setHeaderData(0, Qt::Horizontal, tr("MR Number"));
	model->setHeaderData(1, Qt::Horizontal, tr("Last Name"));
	model->setHeaderData(2, Qt::Horizontal, tr("First Name"));
	model->setHeaderData(3, Qt::Horizontal, tr("D.O.B."));

	ui->resultTable->setModel(model);

	db_queried = true;	// Let other functions start accessing values in the table

	// Enable/disable buttons depending on if there were any hits in the search
	if (model->rowCount() > 0) {
		ui->prescribeButton->setEnabled(true);
		ui->modifyButton->setEnabled(true);
	} else {
		ui->prescribeButton->setEnabled(false);
		ui->modifyButton->setEnabled(false);
	}
}

void PatientSearch::resetPressed()
{
	ui->dobField->setDate(DEFAULTS::Date);
}

void PatientSearch::initiatePrescription()
{
	unsigned int row;
	PatientRecord *patient;
	MedicationRecord *medication;
	PrescriptionRecord *prescription;
	ShipmentRecord *shipment;

	AlterPrescriptionWizard *wiz;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	patient = new PatientRecord();
	medication = new MedicationRecord();
	prescription = new PrescriptionRecord();
	shipment = new ShipmentRecord();

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	patient->retrieve(ids[row]);

	wiz = new AlterPrescriptionWizard();
	wiz->setPatient(patient);
	wiz->setMedication(medication);
	wiz->setPrescription(prescription);
	wiz->setShipment(shipment);
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

}
