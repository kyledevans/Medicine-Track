/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QDebug>

#include "prescriptionsframe.h"
#include "ui_prescriptionsframe.h"

#include "prescriptionlabel.h"
#include "globals.h"
#include "alertinterface.h"

PrescriptionsFrame::PrescriptionsFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::PrescriptionsFrame),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->printAction, SIGNAL(triggered()), this, SLOT(initiatePrint()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

	ui->resultTable->addAction(ui->modifyAction);
	ui->resultTable->addAction(ui->printAction);

	selectionChanged();
}

PrescriptionsFrame::~PrescriptionsFrame()
{
    delete ui;
}

void PrescriptionsFrame::resetPressed()
{
	ui->filledField->setDate(QDate(1970, 1, 31));	// Default date is 1/31/1970
	ui->dobField->setDate(QDate(1970, 1, 31));
}

/* SQL query without C++:
SELECT prescriptions.id, patients.allscripts_id, patients.last, patients.first, patients.dob, drugs.name,
drugs.form, drugs.strength, prescriptions.amount, prescriptions.written, prescriptions.filled, shipments.lot
FROM prescriptions
JOIN patients ON prescriptions.patient_id = patients.id
JOIN drugs ON prescriptions.drug_id = drugs.id
JOIN shipments ON prescriptions.shipment_id = shipments.id
WHERE drugs.name LIKE SOMEVAR
AND shipments.lot LIKE SOMEVAR
AND prescriptions.filled = SOMEVAR
AND patients.last LIKE SOMEVAR
AND patients.first LIKE SOMEVAR
AND patients.dob = SOMEVAR;
*/
void PrescriptionsFrame::initiateSearch()
{
	QSqlQuery *model;		// SQL interface
	QString query;			// SQL query string
	AlertInterface alert;	// Submits query and handles errors
	int i;					// Increment var

	// Test if any field has been altered
	if (ui->medicationNameField->text().isEmpty() &&
		ui->lotField->text().isEmpty() &&
		ui->lastField->text().isEmpty() &&
		ui->firstField->text().isEmpty() &&
		(ui->filledField->date() == DEFAULTS::Date) &&
		(ui->dobField->date() == DEFAULTS::Date)) {

		return;
	}

	query = QString("SELECT prescriptions.id, patients.allscripts_id, patients.last, patients.first, patients.dob, drugs.name, drugs.form, drugs.strength, prescriptions.amount, prescriptions.written, prescriptions.filled, shipments.lot FROM prescriptions JOIN patients ON prescriptions.patient_id = patients.id JOIN drugs ON prescriptions.drug_id = drugs.id JOIN shipments ON prescriptions.shipment_id = shipments.id WHERE drugs.name LIKE '%");
	query += SQL::cleanInput(ui->medicationNameField->text()) + QString("%' AND shipments.lot LIKE '%");
	query += SQL::cleanInput(ui->lotField->text()) + QString("%'");
	if (ui->filledField->date() != DEFAULTS::Date) {	// Search by filled date if necessary
		query += QString(" AND prescriptions.filled = '");
		query += ui->filledField->date().toString("yyyy-MM-dd") + QString("'");
	}
	query += QString(" AND patients.last LIKE '%");
	query += SQL::cleanInput(ui->lastField->text()) + QString("%' AND patients.first LIKE '%");
	query += SQL::cleanInput(ui->firstField->text()) + QString("%'");
	if (ui->dobField->date() != DEFAULTS::Date) {		// Search by dob if necessary
		query += QString(" AND patients.dob = '");
		query += ui->dobField->date().toString("yyyy-MM-dd") + QString("'");
	}
	query += QString(";");

	model = new QSqlQuery;
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
		ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(4).toDate().toString("M/d/yyyy")));
		ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(5).toString()));
		ui->resultTable->setItem(i, 5, new QTableWidgetItem(model->value(6).toString()));
		ui->resultTable->setItem(i, 6, new QTableWidgetItem(model->value(7).toString()));
		ui->resultTable->setItem(i, 7, new QTableWidgetItem(model->value(8).toString()));
		ui->resultTable->setItem(i, 8, new QTableWidgetItem(model->value(9).toDate().toString("M/d/yyyy")));
		ui->resultTable->setItem(i, 9, new QTableWidgetItem(model->value(10).toDate().toString("M/d/yyyy")));
		ui->resultTable->setItem(i, 10, new QTableWidgetItem(model->value(11).toString()));
	}

	db_queried = true;
	delete model;
}

void PrescriptionsFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->modifyButton->setEnabled(true);
		ui->modifyAction->setEnabled(true);
		ui->printAction->setEnabled(true);
	} else {
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
		ui->printAction->setEnabled(false);
	}
}

void PrescriptionsFrame::initiateModify()
{
	unsigned int row;

	if (db_queried) {
		if (ui->resultTable->selectionModel()->hasSelection()) {
			// This line finds the top row that was selected by the user
			row = ui->resultTable->selectionModel()->selectedRows()[0].row();
//			qDebug() << "prescriptions.id = " << presIds[row] << " | patients.id = " << patientIds[row] << " | drugs.id = " << drugIds[row] << " | shipments.id = " << shipmentIds[row];
		}
	}
}

void PrescriptionsFrame::initiatePrint()
{
	PrescriptionLabel *label;
	unsigned int row;
	PrescriptionRecord *pres;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	pres = new PrescriptionRecord;
	pres->retrieve(ids[row]);
	label = new PrescriptionLabel(pres);
	label->printLabel();

	delete label;
}

