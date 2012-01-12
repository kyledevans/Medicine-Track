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

#include "patientrecord.h"
#include "newpatientwizard.h"

PatientSearch::PatientSearch(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PatientSearch),
	db_queried(false)
{
    ui->setupUi(this);

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

void PatientSearch::initiateSearch()
{
	QString query;	// Holds the SQL query
	ids.clear();

	// If the text fields are empty, don't do anything.
	if (		(ui->firstNameField->text() != QString(""))
			||	(ui->lastNameField->text() != QString(""))
			||	(ui->dobField->date().toString("yyyy-MM-dd") != QString("1970-01-31"))) {
		QSqlQueryModel *model = new QSqlQueryModel(ui->resultTable);

		query = QString("SELECT id, last, first, dob FROM patients WHERE first LIKE '%")
				+= ui->firstNameField->text()
				+= QString("%' AND last LIKE '%")
				+= ui->lastNameField->text()
				+= QString("%'");
		if (ui->dobField->date().toString("yyyy-MM-dd") != QString("1970-01-31")) {
			query += QString(" AND dob = '")
				+= ui->dobField->date().toString("yyyy-MM-dd")
				+= QString("';");
		} else {
			query += QString(";");
		}

		model->setQuery(query);

		// Retrieve the ID's before we remove them from the display
		for (int i = 0; i < model->rowCount(); i++) {
			ids.append(model->record(i).value("id").toInt());
		}

		model->removeColumn(0);
		model->setHeaderData(0, Qt::Horizontal, tr("Last name"));
		model->setHeaderData(1, Qt::Horizontal, tr("First name"));
		model->setHeaderData(2, Qt::Horizontal, tr("D.O.B."));

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
}

void PatientSearch::resetPressed()
{
	ui->dobField->setDate(QDate(1970, 1, 31));	// Default date is 1/31/1970
}

void PatientSearch::initiatePrescription()
{
	unsigned int row, id;

	if (db_queried) {
		if (ui->resultTable->selectionModel()->hasSelection()) {
			// This line finds the top row that was selected by the user
			row = ui->resultTable->selectionModel()->selectedRows()[0].row();
			id = ids[row];
			qDebug() << id;
		}
	}
}

/* SQL without C++.  Numbers in () indicate the column number and are not SQL:
SELECT id(0), last(1), first(2), dob(3)
FROM patients
WHERE id = 'SOME_VAR';
*/
void PatientSearch::initiateModification()
{
	unsigned int row;
	PatientRecord *patient;
	NewPatientWizard *wiz;
	QSqlQueryModel *model;
	QString query;

	if (db_queried) {
		if (ui->resultTable->selectionModel()->hasSelection()) {
			patient = new PatientRecord;
			model = new QSqlQueryModel();

			// This line finds the top row that was selected by the user
			row = ui->resultTable->selectionModel()->selectedRows()[0].row();
			patient->id = ids[row];

			patient->exists = true;

			query = QString("SELECT id, last, first, dob FROM patients WHERE id = '");
			query += QString().setNum(patient->id) + QString("';");

			model->setQuery(query);

			patient->last = model->record(0).value(1).toString();
			patient->first = model->record(0).value(2).toString();
			patient->dob = QDate().fromString(model->record(0).value(3).toString(), "yyyy-MM-dd");

			delete model;

			wiz = new NewPatientWizard(patient);
			connect(wiz, SIGNAL(wizardComplete(PatientRecord*)), this, SLOT(submitModify(PatientRecord*)));

			wiz->exec();
			delete wiz;
		}
	}
}

void PatientSearch::initiateNewPatient()
{
	NewPatientWizard *wiz;
	PatientRecord *patient = new PatientRecord;

	wiz = new NewPatientWizard(patient);

	connect(wiz, SIGNAL(wizardComplete(PatientRecord*)), this, SLOT(newPatient(PatientRecord*)));

	wiz->exec();

	delete wiz;
}

/* SQL code without C++:
INSERT INTO patients (last, first, dob)
VALUES ('SOME_VAR', 'SOME_VAR', 'SOME_VAR');
*/
void PatientSearch::newPatient(PatientRecord *new_patient)
{
	QString query;
	QSqlQueryModel *model = new QSqlQueryModel();	// TODO: garbage collection for this

	query = QString("INSERT INTO patients (last, first, dob) VALUES ('");
	query += new_patient->last;
	query += QString("', '");
	query += new_patient->first;
	query += QString("', '");
	query += new_patient->dob.toString("yyyy-MM-dd");
	query += QString("');");

	model->setQuery(query);

	ui->lastNameField->setText(new_patient->last);
	ui->firstNameField->setText(new_patient->first);
	ui->dobField->setDate(new_patient->dob);

	initiateSearch();

	delete model;
	delete new_patient;
}

/* SQL without C++:
UPDATE patients
SET last = 'SOME_VAR', first = 'SOME_VAR', dob = 'SOME_VAR'
WHERE id = 'SOME_VAR';
*/
void PatientSearch::submitModify(PatientRecord *new_patient)
{
	QString query;
	QSqlQueryModel *model = new QSqlQueryModel();

	query = QString("UPDATE patients SET last = '");
	query += new_patient->last + QString("', first = '");
	query += new_patient->first + QString("', dob = '");
	query += new_patient->dob.toString("yyyy-MM-dd") + QString("' WHERE id = '");
	query += QString().setNum(new_patient->id) + QString("';");

	model->setQuery(query);

	ui->lastNameField->setText(new_patient->last);
	ui->firstNameField->setText(new_patient->first);
	ui->dobField->setDate(new_patient->dob);

	initiateSearch();

	delete model;
	delete new_patient;
}
