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
	header->setText(MedicationRecord::form_Label);
	header->setToolTip(MedicationRecord::form_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(2);
	header->setText(MedicationRecord::strength_Label);
	header->setToolTip(MedicationRecord::strength_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(3);
	header->setText(MedicationRecord::unit_size_Label);
	header->setToolTip(MedicationRecord::unit_size_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(4);
	header->setText(ShipmentRecord::product_left_Label);
	header->setToolTip(ShipmentRecord::product_left_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(5);
	header->setText(ShipmentRecord::expiration_Label);
	header->setToolTip(ShipmentRecord::expiration_Tooltip);

	ui->resultTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	// Connect signals/slots
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
SELECT shipments.id, shipments.drug_id, drugs.name, drugs.form, drugs.strength,
drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units), shipments.expiration
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
	int i;		// Increment variable

	model = new QSqlQuery;

	query = QString("SELECT shipments.id, shipments.drug_id, drugs.name, drugs.form, drugs.strength, drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units), shipments.expiration FROM shipments JOIN drugs ON drugs.id = shipments.drug_id WHERE shipments.active = '1' AND drugs.active = '1' AND shipments.expiration > CURDATE() AND drugs.name LIKE '%");
	query += SQL::cleanInput(ui->medicationField->text()) + QString("%';");

	if (!ui->medicationField->text().isEmpty()) {
		barcode.setBarcode(ui->medicationField->text());
		if (barcode.toID() != SQL::Undefined_ID) {
			query = QString("SELECT shipments.id, shipments.drug_id, drugs.name, drugs.form, drugs.strength, drugs.unit_size, CONCAT(shipments.product_left, ' ', drugs.dispense_units), shipments.expiration FROM shipments JOIN drugs ON drugs.id = shipments.drug_id WHERE shipments.id = '");
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
		ui->resultTable->setItem(i, 5, new QTableWidgetItem(model->value(7).toString()));
	}

	db_queried = true;
    delete model;
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
