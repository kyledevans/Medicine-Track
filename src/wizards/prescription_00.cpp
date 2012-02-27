/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableWidgetItem>

#include "prescription_00.h"
#include "ui_prescription_00.h"

#include "db/alertinterface.h"
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
	QTableWidgetItem *header;
	ui->setupUi(this);

	// Setup UI strings and tooltips
	ui->medicationLabel->setText(ShipmentRecord::name_barcode_Label);
	ui->medicationLabel->setToolTip(ShipmentRecord::name_barcode_Tooltip);
	ui->medicationField->setToolTip(ShipmentRecord::name_barcode_Tooltip);

	// Set the various strings and tooltips for the resultTable
	header = ui->resultTable->horizontalHeaderItem(0);
	header->setText(MedicationRecord::name_Label);
	header->setToolTip(MedicationRecord::name_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(1);
	header->setText(ShipmentRecord::lot_Label);
	header->setToolTip(ShipmentRecord::lot_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(2);
	header->setText(MedicationRecord::form_Label);
	header->setToolTip(MedicationRecord::form_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(3);
	header->setText(MedicationRecord::strength_Label);
	header->setToolTip(MedicationRecord::strength_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(4);
	header->setText(MedicationRecord::unit_size_Label);
	header->setToolTip(MedicationRecord::unit_size_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(5);
	header->setText(ShipmentRecord::product_left_Label);
	header->setToolTip(ShipmentRecord::product_left_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(6);
	header->setText(ShipmentRecord::expiration_Label);
	header->setToolTip(ShipmentRecord::expiration_Tooltip);

	ui->resultTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	// Connect signals/slots
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SIGNAL(completeChanged()));
	connect(ui->resultTable, SIGNAL(clicked(QModelIndex)), this, SLOT(resultSelected()));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
}

Prescription_00::~Prescription_00()
{
	delete ui;
}

void Prescription_00::setPatient(PatientRecord *new_patient)
{
	patient = new_patient;
	ui->lastLabel->setText(patient->last);
	ui->firstLabel->setText(patient->first);
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
AND drugs.name LIKE ?;
*/
void Prescription_00::initiateSearch()
{
	QSqlQuery *model;
	AlertInterface alert;
	BarcodeLabel barcode;
	int i;					// Increment variable

	model = new QSqlQuery;
	barcode.setBarcode(ui->medicationField->text());

	if (barcode.toID() == SQL::Undefined_ID) {	// Normal search
		model->prepare("SELECT shipments.id, shipments.drug_id, drugs.name, shipments.lot, drugs.form, drugs.strength, "
					   "drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units), shipments.expiration "
					   "FROM shipments "
					   "JOIN drugs ON drugs.id = shipments.drug_id "
					   "WHERE shipments.active = 1 "
					   "AND drugs.active = 1 "
					   "AND shipments.expiration > CURDATE() "
					   "AND shipments.product_left >= 1 "
					   "AND drugs.name LIKE ?;");
		model->bindValue(0, SQL::prepWildcards(ui->medicationField->text()));
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
		ui->resultTable->setItem(i, 5, new QTableWidgetItem(model->value(7).toString()));
		ui->resultTable->setItem(i, 6, new QTableWidgetItem(model->value(8).toDate().toString(DEFAULTS::DateDisplayFormat)));
	}

	db_queried = true;
	delete model;
}

void Prescription_00::resultSelected()
{
	emit(medicationChanged(ids[ui->resultTable->selectionModel()->selectedRows()[0].row()]));
}

bool Prescription_00::isComplete() const
{
	if (!db_queried) {
		return false;
	}

	if (ui->resultTable->selectionModel()->hasSelection()) {
		return true;
	}

	return false;
}
