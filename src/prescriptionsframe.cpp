/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

#include <QDebug>

#include "prescriptionsframe.h"
#include "ui_prescriptionsframe.h"

#include "prescriptionlabel.h"
#include "globals.h"
#include "db/alertinterface.h"
#include "db/prescriptionrecord.h"
#include "shipmentrecord.h"

PrescriptionsFrame::PrescriptionsFrame(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::PrescriptionsFrame),
	db_queried(false)
{
	QTableWidgetItem *header;
	ui->setupUi(this);

	// Setup the search UI strings and tooltips
	ui->medicationNameLabel->setText(MedicationRecord::name_Label);
	ui->medicationNameLabel->setToolTip(MedicationRecord::name_Tooltip);
	ui->medicationNameField->setToolTip(MedicationRecord::name_Tooltip);

	ui->lotLabel->setText(ShipmentRecord::lot_Label);
	ui->lotLabel->setToolTip(ShipmentRecord::lot_Tooltip);
	ui->lotField->setToolTip(ShipmentRecord::lot_Tooltip);

	ui->filledLabel->setText(PrescriptionRecord::filled_Label);
	ui->filledLabel->setToolTip(PrescriptionRecord::filled_Tooltip);
	ui->filledField->setToolTip(PrescriptionRecord::filled_Tooltip);

	ui->lastLabel->setText(PatientRecord::last_Label);
	ui->lastLabel->setToolTip(PatientRecord::last_Tooltip);
	ui->lastField->setToolTip(PatientRecord::last_Tooltip);

	ui->firstLabel->setText(PatientRecord::first_Label);
	ui->firstLabel->setToolTip(PatientRecord::first_Tooltip);
	ui->firstField->setToolTip(PatientRecord::first_Tooltip);

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

	header = ui->resultTable->horizontalHeaderItem(4);
	header->setText(MedicationRecord::name_Label);
	header->setToolTip(MedicationRecord::name_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(5);
	header->setText(MedicationRecord::form_Label);
	header->setToolTip(MedicationRecord::form_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(6);
	header->setText(MedicationRecord::strength_Label);
	header->setToolTip(MedicationRecord::strength_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(7);
	header->setText(PrescriptionRecord::amount_Label);
	header->setToolTip(PrescriptionRecord::amount_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(8);
	header->setText(PrescriptionRecord::filled_Label);
	header->setToolTip(PrescriptionRecord::filled_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(9);
	header->setText(PrescriptionRecord::written_Label);
	header->setToolTip(PrescriptionRecord::written_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(10);
	header->setText(ShipmentRecord::lot_Label);
	header->setToolTip(ShipmentRecord::lot_Tooltip);

	// Setup signals/slots
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->printAction, SIGNAL(triggered()), this, SLOT(initiatePrint()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->invalidateAction, SIGNAL(triggered()), this, SLOT(invalidatePrescription()));

	// Add items to resultTable right-click menu
	ui->resultTable->addAction(ui->printAction);
	ui->resultTable->addAction(ui->invalidateAction);

	// Disable actions that require an item selected in the resultTable
	selectionChanged();
}

PrescriptionsFrame::~PrescriptionsFrame()
{
	delete ui;
}

void PrescriptionsFrame::invalidatePrescription()
{
	QMessageBox msg;
	PrescriptionRecord prescription;
	int val;
	unsigned int row;

	msg.setText("Verify prescription removal");
	msg.setInformativeText("Are you sure you want to remove this prescription permanently?");
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msg.setDefaultButton(QMessageBox::No);
	val = msg.exec();

	if (val == QMessageBox::Yes) {	// Yes pressed
		if (!db_queried) {
			return;
		}
		if (!ui->resultTable->selectionModel()->hasSelection()) {
			return;
		}

		// This line finds the top row that was selected by the user
		row = ui->resultTable->selectionModel()->selectedRows()[0].row();

		// Retrieve and invalidate prescription
		prescription.retrieve(ids[row]);
		prescription.invalidate();
		initiateSearch();
	}
}

void PrescriptionsFrame::resetPressed()
{
	ui->filledField->setDate(DEFAULTS::Date);
	ui->dobField->setDate(DEFAULTS::Date);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
	ui->invalidField->setChecked(false);
}

/* SQL query without C++:
SELECT prescriptions.id, patients.allscripts_id, patients.last,
patients.first, patients.dob, drugs.name, drugs.form, drugs.strength,
CONCAT(prescriptions.amount, ' ', drugs.dispense_units),
prescriptions.written, prescriptions.filled, shipments.lot
FROM prescriptions
JOIN patients ON prescriptions.patient_id = patients.id
JOIN drugs ON prescriptions.drug_id = drugs.id
JOIN shipments ON prescriptions.shipment_id = shipments.id
WHERE drugs.name LIKE ?
AND shipments.lot LIKE ?
AND (<true if NOT searching by filled date> OR (prescriptions.filled = ?))
AND patients.last LIKE ?
AND patients.first LIKE ?
AND (<true if NOT searching by dob> OR (patients.dob = ?))
AND prescriptions.active = ?;
*/
void PrescriptionsFrame::initiateSearch()
{
	QSqlQuery *model;		// SQL interface
	AlertInterface alert;	// Submits query and handles errors
	int i;					// Increment var
	bool dont_search_filled = true;
	bool dont_search_dob = true;

	// Test if any field has been altered
	if (ui->medicationNameField->text().isEmpty() &&
		ui->lotField->text().isEmpty() &&
		ui->lastField->text().isEmpty() &&
		ui->firstField->text().isEmpty() &&
		(ui->filledField->date() == DEFAULTS::Date) &&
		(ui->dobField->date() == DEFAULTS::Date)) {

		if (ui->resultTable->rowCount() > 0) {
			ui->resetButton->click();
		}
		return;
	}

	model = new QSqlQuery;

	model->prepare("SELECT prescriptions.id, patients.allscripts_id, patients.last, "
				   "patients.first, patients.dob, drugs.name, drugs.form, drugs.strength, "
				   "CONCAT(prescriptions.amount, ' ', drugs.dispense_units), "
				   "prescriptions.written, prescriptions.filled, shipments.lot "
				   "FROM prescriptions "
				   "JOIN patients ON prescriptions.patient_id = patients.id \n"
				   "JOIN drugs ON prescriptions.drug_id = drugs.id "
				   "JOIN shipments ON prescriptions.shipment_id = shipments.id "
				   "WHERE drugs.name LIKE ? "
				   "AND shipments.lot LIKE ? "
				   "AND (? OR (prescriptions.filled = ?)) "
				   "AND patients.last LIKE ? "
				   "AND patients.first LIKE ? "
				   "AND (? OR (patients.dob = ?)) "
				   "AND patients.active = ? "
				   "AND prescriptions.active = ?;");
	model->bindValue(0, SQL::prepWildcards(ui->medicationNameField->text()));
    model->bindValue(1, SQL::prepWildcards(ui->lotField->text()));
	if (ui->filledField->date() != DEFAULTS::Date) {    // Enables searching by filled date if the user made a change
		dont_search_filled = false;
	}
	model->bindValue(2, QVariant(dont_search_filled));
	model->bindValue(3, QVariant(ui->filledField->date()));
	model->bindValue(4, SQL::prepWildcards(ui->lastField->text()));
	model->bindValue(5, SQL::prepWildcards(ui->firstField->text()));
	if (ui->dobField->date() != DEFAULTS::Date) {       // Enables searching by dob if the user made a change
		dont_search_dob = false;
	}
	model->bindValue(6, QVariant(dont_search_dob));
	model->bindValue(7, QVariant(ui->dobField->date()));
	model->bindValue(8, QVariant(ui->activeField->isChecked()));
	model->bindValue(9, QVariant(!ui->invalidField->isChecked()));

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
		ui->resultTable->setItem(i, 0, new QTableWidgetItem(model->value(1).toString()));
		ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(2).toString()));
		ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(3).toString()));
		ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(4).toDate().toString(DEFAULTS::DateDisplayFormat)));
		ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(5).toString()));
		ui->resultTable->setItem(i, 5, new QTableWidgetItem(model->value(6).toString()));
		ui->resultTable->setItem(i, 6, new QTableWidgetItem(model->value(7).toString()));
		ui->resultTable->setItem(i, 7, new QTableWidgetItem(model->value(8).toString()));
		ui->resultTable->setItem(i, 8, new QTableWidgetItem(model->value(9).toDate().toString(DEFAULTS::DateDisplayFormat)));
		ui->resultTable->setItem(i, 9, new QTableWidgetItem(model->value(10).toDate().toString(DEFAULTS::DateDisplayFormat)));
		ui->resultTable->setItem(i, 10, new QTableWidgetItem(model->value(11).toString()));
	}

	db_queried = true;
	delete model;
}

void PrescriptionsFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->modifyAction->setEnabled(true);
		ui->printAction->setEnabled(true);
		ui->invalidateAction->setEnabled(true);
	} else {
		ui->modifyAction->setEnabled(false);
		ui->printAction->setEnabled(false);
		ui->invalidateAction->setEnabled(false);
	}
}

// TODO: This doesn't do anything
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

