/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescription_00.h"
#include "ui_prescription_00.h"

#include "alertinterface.h"
#include "drugrecord.h"
#include "shipmentrecord.h"
#include "globals.h"
#include "barcodelabel.h"

#include <QDebug>

Prescription_00::Prescription_00(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::Prescription_00),
	patient(0),
	prescription(0),
	db_queried(false)
{
	ui->setupUi(this);

	ui->resultTable->postSetup();

	// Connect signals/slots
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SIGNAL(completeChanged()));
	connect(ui->resultTable, SIGNAL(clicked(QModelIndex)), this, SLOT(resultSelected()));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));

	// Hide the column with internal id's from the user
	ui->resultTable->hideColumn(0);
}

Prescription_00::~Prescription_00()
{
	delete ui;
}

void Prescription_00::resizeHeaders()
{
	int i;
	int size;

	// Start at 1 because the first column is the 'flags' column
	for (i = 1; i < ui->resultTable->columnCount(); i++) {
		size = ui->resultTable->columnWidth(0);
		ui->resultTable->resizeColumnsToContents();
		ui->resultTable->setColumnWidth(0, size);
	}
}

void Prescription_00::setPatient(PatientRecord *new_patient)
{
	patient = new_patient;
	ui->lastLabel->setText(patient->getLast());
	ui->firstLabel->setText(patient->getFirst());
}

void Prescription_00::setPrescription(PrescriptionRecord *new_prescription)
{
	prescription = new_prescription;
}

/* SQL without C++:
SELECT shipments.id, shipments.drug_id, drugs.name, shipments.lot, drugs.form, drugs.strength,
drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units), shipments.expiration
FROM shipments
JOIN drugs ON drugs.id = shipments.drug_id
WHERE shipments.active = 1
AND drugs.active = 1
AND shipments.expiration > CURDATE()
AND shipments.product_left >= 1
AND (drugs.name LIKE ? OR shipments.lot LIKE ?);
*/
void Prescription_00::initiateSearch()
{
	QSqlQuery *model;
	AlertInterface alert;
	BarcodeLabel barcode;
	int i;					// Increment variable

	model = new QSqlQuery;
	barcode.setBarcode(ui->medicationField->text());

	if (!barcode.isValidSID(ui->medicationField->text())) {	// Normal search
		model->prepare("SELECT shipments.id, shipments.drug_id, drugs.name, shipments.lot, drugs.form, drugs.strength, "
					   "drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units), shipments.expiration "
					   "FROM shipments "
					   "JOIN drugs ON drugs.id = shipments.drug_id "
					   "WHERE shipments.active = 1 "
					   "AND drugs.active = 1 "
					   "AND shipments.expiration > CURDATE() "
					   "AND shipments.product_left >= 1 "
					   "AND (drugs.name LIKE ? "
					   "OR shipments.lot LIKE ?);");
		model->bindValue(0, SQL::prepWildcards(ui->medicationField->text()));
		model->bindValue(1, SQL::prepWildcards(ui->medicationField->text()));
	} else {		// Searching by barcode
		model->prepare("SELECT shipments.id, shipments.drug_id, drugs.name, shipments.lot, drugs.form, drugs.strength, "
					   "drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units), shipments.expiration "
					   "FROM shipments "
					   "JOIN drugs ON drugs.id = shipments.drug_id "
					   "WHERE shipments.active = 1 "
					   "AND drugs.active = 1 "
					   "AND shipments.expiration > CURDATE() "
					   "AND shipments.product_left >= 1 "
					   "AND shipments.id = ?;");
		model->bindValue(0, QVariant(barcode.toID()));
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
		ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(2).toString()));
		ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(3).toString()));
		ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(4).toString()));
		ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(5).toString()));
		ui->resultTable->setItem(i, 5, new QTableWidgetItem(model->value(6).toString()));
		ui->resultTable->setItem(i, 6, new QTableWidgetItem(model->value(7).toString()));
		ui->resultTable->setItem(i, 7, new QTableWidgetItem(model->value(8).toDate().toString(DEFAULTS::DateDisplayFormat)));
	}
	ui->resultTable->setSortingEnabled(true);
	ui->resultTable->sortByColumn(1, Qt::AscendingOrder);
	resizeHeaders();

	db_queried = true;
	delete model;
}

void Prescription_00::resultSelected()
{
	int row;

	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	emit(medicationChanged(ui->resultTable->item(row, 0)->text().toInt()));
}

bool Prescription_00::isComplete() const
{
	if (!db_queried) {
		return false;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return false;
	}

	return true;
}

void Prescription_00::changeEvent(QEvent *e)
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
