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

		query = QString("SELECT id, last, first, dob FROM patients WHERE first LIKE '%");
		query += ui->firstNameField->text();
		query += QString("%' AND last LIKE '%");
		query += ui->lastNameField->text();
		query += QString("%'");
		if (ui->dobField->date().toString("yyyy-MM-dd") != QString("1970-01-31")) {
			query += QString(" AND dob = '");
			query += ui->dobField->date().toString("yyyy-MM-dd");
			query += QString("'");
		}
		query += QString(" AND active = '1';");

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

void PatientSearch::initiateModification()
{

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
	newPatientCleanup(patient);
}

void PatientSearch::newPatientCleanup(PatientRecord *patient)
{
	delete patient;
}

void PatientSearch::submitModify(PatientRecord *patient)
{

}
