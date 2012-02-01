/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>

#include "apw_page00.h"
#include "ui_apw_page00.h"

#include "alertinterface.h"
#include "globals.h"

#include <QDebug>

APW_Page00::APW_Page00(QWidget *parent) :
    QWizardPage(parent),
	ui(new Ui::APW_Page00),
	patient(0),
	prescription(0),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->resultTable, SIGNAL(clicked(QModelIndex)), this, SIGNAL(completeChanged()));
	connect(ui->resultTable, SIGNAL(clicked(QModelIndex)), this, SLOT(resultSelected()));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
}

APW_Page00::~APW_Page00()
{
    delete ui;
}

void APW_Page00::setPatient(PatientRecord *new_patient)
{
	patient = new_patient;
	ui->lastLabel->setText(patient->last);
	ui->firstLabel->setText(patient->first);
}

void APW_Page00::setPrescription(PrescriptionRecord *new_prescription)
{
	prescription = new_prescription;
}

/* SQL without C++:
SELECT shipments.id, shipments.drug_id, drugs.name, drugs.form, drugs.strength,
drugs.str_units, drugs.amount, drugs.am_units, shipments.product_left
FROM shipments
JOIN drugs ON drugs.id = shipments.drug_id
WHERE shipments.active = '1'
AND drugs.active = 1
AND shipments.expiration < CURDATE()
AND drugs.name LIKE '%SOME_VAL%';
*/
void APW_Page00::initiateSearch()
{
	QString query;
	QSqlQueryModel *model;
	AlertInterface alert;

	model = new QSqlQueryModel(ui->resultTable);

	query = QString("SELECT shipments.id, shipments.drug_id, drugs.name, drugs.form, drugs.strength, drugs.str_units, drugs.amount, drugs.am_units, shipments.product_left FROM shipments JOIN drugs ON drugs.id = shipments.drug_id WHERE shipments.active = '1' AND drugs.active = 1 AND shipments.expiration < CURDATE() AND drugs.name LIKE '%");
	query += SQL::cleanInput(ui->medicationField->text()) + QString("%';");

	if (!alert.attemptQuery(model, &query)) {
		delete model;
		return;
	}

	ids.clear();
	// Retrieve the ID's before we remove them from the display
	for (int i = 0; i < model->rowCount(); i++) {
		ids.append(model->record(i).value(0).toInt());
	}

	ui->resultTable->setModel(model);

	db_queried = true;
}

void APW_Page00::resultSelected()
{
	emit(medicationChanged(ids[ui->resultTable->selectionModel()->selectedRows()[0].row()]));
}

bool APW_Page00::isComplete() const
{
	if (!db_queried) {
		return false;
	}

	if (ui->resultTable->selectionModel()->hasSelection()) {
		return true;
	}

	return false;
}
