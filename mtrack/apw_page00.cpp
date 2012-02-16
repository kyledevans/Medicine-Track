/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableWidgetItem>

#include "apw_page00.h"
#include "ui_apw_page00.h"

#include "alertinterface.h"
#include "globals.h"
#include "barcodelabel.h"

#include <QDebug>

APW_Page00::APW_Page00(QWidget *parent) :
    QWizardPage(parent),
	ui(new Ui::APW_Page00),
	patient(0),
	prescription(0),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SIGNAL(completeChanged()));
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
SELECT shipments.id, shipments.drug_id, drugs.name, drugs.form, CONCAT(drugs.strength, ' ', drugs.str_units),
drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units)
FROM shipments
JOIN drugs ON drugs.id = shipments.drug_id
WHERE shipments.active = '1'
AND drugs.active = '1'
AND shipments.expiration > CURDATE()
AND drugs.name LIKE '%SOME_VAL%';
*/
void APW_Page00::initiateSearch()
{
	QString query;
	QSqlQuery *model;
	AlertInterface alert;
	BarcodeLabel barcode;
	QTableWidgetItem *item;
	int i;		// Increment variable

	model = new QSqlQuery;

    query = QString("SELECT shipments.id, shipments.drug_id, drugs.name, drugs.form, CONCAT(drugs.strength, ' ', drugs.str_units), drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units) FROM shipments JOIN drugs ON drugs.id = shipments.drug_id WHERE shipments.active = '1' AND drugs.active = '1' AND shipments.expiration > CURDATE() AND drugs.name LIKE '%");
	query += SQL::cleanInput(ui->medicationField->text()) + QString("%';");

	if (!ui->medicationField->text().isEmpty()) {
		barcode.setBarcode(ui->medicationField->text());
		if (barcode.toID() != SQL::Undefined_ID) {
            query = QString("SELECT shipments.id, shipments.drug_id, drugs.name, drugs.form, CONCAT(drugs.strength, ' ', drugs.str_units), drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units) FROM shipments JOIN drugs ON drugs.id = shipments.drug_id WHERE shipments.id = '");
			query += QString().setNum(barcode.toID()) + QString("';");
		}
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
		ui->resultTable->setItem(i, 0, new QTableWidgetItem(model->value(2).toString()));
		ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(3).toString()));
		ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(4).toString()));
		ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(5).toString()));
		ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(6).toString()));
	}

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
