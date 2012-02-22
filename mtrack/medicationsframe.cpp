/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableWidgetItem>

#include <QDebug>

#include "medicationsframe.h"
#include "ui_medicationsframe.h"

#include "altermedicationwizard.h"
#include "altershipmentwizard.h"
#include "globals.h"
#include "alertinterface.h"
#include "shipmentrecord.h"
#include "barcodelabel.h"
#include "medicationrecord.h"

MedicationsFrame::MedicationsFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::MedicationsFrame),
	db_queried(false)
{
	QTableWidgetItem *header;
    ui->setupUi(this);

	// Set the search UI strings and tooltips
	ui->nameLabel->setText(ShipmentRecord::name_barcode_Label);
	ui->nameLabel->setToolTip(ShipmentRecord::name_barcode_Tooltip);
	ui->nameField->setToolTip(ShipmentRecord::name_barcode_Tooltip);

	// Set the various strings and tooltips for the resultTable
	header = ui->resultTable->horizontalHeaderItem(0);
	header->setText(MedicationRecord::name_Label);
	header->setToolTip(MedicationRecord::name_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(1);
	header->setText(MedicationRecord::ndc_Label);
	header->setToolTip(MedicationRecord::ndc_Tooltip);

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

	ui->resultTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	// Setup various signal/slot connections
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->newMedicationAction, SIGNAL(triggered()), this, SLOT(initiateNewMed()));
	connect(ui->newStockAction, SIGNAL(triggered()), this, SLOT(initiateNewShipment()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->toggleAction, SIGNAL(triggered()), this, SLOT(toggleActive()));

	// Add items to resultTable right-click menu
	ui->resultTable->addAction(ui->newStockAction);
	ui->resultTable->addAction(ui->modifyAction);
	ui->resultTable->addAction(ui->toggleAction);

	selectionChanged();
}

MedicationsFrame::~MedicationsFrame()
{
    delete ui;
}

void MedicationsFrame::toggleActive()
{
	unsigned int row;
	MedicationRecord medication;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	medication.retrieve(ids[row]);
	medication.toggleActive();
}

/* SQL command without C++
SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size,
CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units)
FROM drugs
LEFT JOIN (
	SELECT shipments.id, shipments.drug_id, shipments.product_left
	FROM shipments
	WHERE shipments.active = 1
	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id
WHERE drugs.name LIKE ?
AND drugs.active = ?
GROUP BY drugs.id;

SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size,
CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units)
FROM drugs
LEFT JOIN (
	SELECT shipments.id, shipments.drug_id, shipments.product_left
	FROM shipments
	WHERE shipments.active = 1
	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id
WHERE drugs.id = (
	SELECT shipments.drug_id
	FROM shipments
	WHERE shipments.id = ?)
GROUP BY drugs.id;
*/
void MedicationsFrame::initiateSearch(int medID)
{
    QSqlQuery *model;
	AlertInterface alert;
	BarcodeLabel barcode;
    int i;      // Increment var

	barcode.setBarcode(ui->nameField->text());
	model = new QSqlQuery;

	if (medID != SQL::Undefined_ID) {	// Searching for a specific drug ID
		model->prepare("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size, "
					   "CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units) "
					   "FROM drugs "
					   "LEFT JOIN ("
					   "	SELECT shipments.id, shipments.drug_id, shipments.product_left "
					   "	FROM shipments "
					   "	WHERE shipments.active = 1 "
					   "	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id "
					   "WHERE drugs.id = ? "
					   "GROUP BY drugs.id;");
		model->bindValue(0, medID);
	} else if (barcode.toID() != SQL::Undefined_ID) {	// There was a barcode, so search only for a matching barcode
		model->prepare("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size, "
					   "CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units) "
					   "FROM drugs "
					   "LEFT JOIN ( "
					   "	SELECT shipments.id, shipments.drug_id, shipments.product_left "
					   "	FROM shipments "
					   "	WHERE shipments.active = 1 "
					   "	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id "
					   "WHERE drugs.id = ( "
					   "	SELECT shipments.drug_id "
					   "	FROM shipments "
					   "	WHERE shipments.id = ?) "
					   "GROUP BY drugs.id;");
		model->bindValue(0, QVariant(barcode.toID()));
	} else {		// Searching based on user input
		model->prepare("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size, "
					   "CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units) "
					   "FROM drugs "
					   "LEFT JOIN ( "
					   "	SELECT shipments.id, shipments.drug_id, shipments.product_left "
					   "	FROM shipments "
					   "	WHERE shipments.active = 1 "
					   "	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id "
					   "WHERE drugs.name LIKE ? "
					   "AND drugs.active = ? "
					   "GROUP BY drugs.id;");
		model->bindValue(0, SQL::prepWildcards(ui->nameField->text()));
		model->bindValue(1, QVariant(ui->activeCheckbox->isChecked()));
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
		ids.append(model->value(0).toInt());	// Retrieve the ID's before they get deleted
        ui->resultTable->setItem(i, 0, new QTableWidgetItem(model->value(1).toString()));
        ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(2).toString()));
        ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(3).toString()));
        ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(4).toString()));
        ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(5).toString()));
        ui->resultTable->setItem(i, 5, new QTableWidgetItem(model->value(6).toString()));
    }

	db_queried = true;
    delete model;
}

void MedicationsFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->newStockButton->setEnabled(true);
		ui->newStockAction->setEnabled(true);
		ui->modifyButton->setEnabled(true);
		ui->modifyAction->setEnabled(true);
	} else {
		ui->newStockButton->setEnabled(false);
		ui->newStockAction->setEnabled(false);
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
	}
}

void MedicationsFrame::initiateNewMed()
{
	AlterMedicationWizard *wiz;
	MedicationRecord *med;

	med = new MedicationRecord();

	wiz = new AlterMedicationWizard(med);
	connect(wiz, SIGNAL(wizardComplete(MedicationRecord*)), this, SLOT(submitNewMed(MedicationRecord*)));
	connect(wiz, SIGNAL(wizardRejected(MedicationRecord*)), this, SLOT(medCleanup(MedicationRecord*)));
	wiz->exec();

	delete wiz;
}

void MedicationsFrame::initiateModify()
{
	unsigned int row;
	AlterMedicationWizard *wiz;
	MedicationRecord *med;

	if (db_queried) {
		if (!ui->resultTable->selectionModel()->hasSelection()) {
			return;
		}
	} else {
		return;
	}

	med = new MedicationRecord();

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!med->retrieve(ids[row])) {
		delete med;
		return;
	}

	wiz = new AlterMedicationWizard(med);
	connect(wiz, SIGNAL(wizardComplete(MedicationRecord*)), this, SLOT(submitModify(MedicationRecord*)));
	connect(wiz, SIGNAL(wizardRejected(MedicationRecord*)), this, SLOT(medCleanup(MedicationRecord*)));
	wiz->exec();

	delete wiz;
}

void MedicationsFrame::submitModify(MedicationRecord *med)
{
	med->commitRecord();
	initiateSearch(med->id);
	medCleanup(med);
}

void MedicationsFrame::submitNewMed(MedicationRecord *med)
{
	med->commitRecord();
	initiateSearch(med->id);
	medCleanup(med);
}

void MedicationsFrame::medCleanup(MedicationRecord *med)
{
	delete med;
}

void MedicationsFrame::initiateNewShipment()
{
	unsigned int row;
	ShipmentRecord *ship;
	AlterShipmentWizard *wiz;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	ship = new ShipmentRecord();

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	ship->drug_id = ids[row];

	wiz = new AlterShipmentWizard(ship);

	connect(wiz, SIGNAL(wizardComplete(ShipmentRecord*)), this, SLOT(submitNewShipment(ShipmentRecord*)));
	connect(wiz, SIGNAL(wizardRejected(ShipmentRecord*)), this, SLOT(shipmentCleanup(ShipmentRecord*)));

	wiz->exec();

	delete wiz;
}

void MedicationsFrame::submitNewShipment(ShipmentRecord *shipment)
{
	BarcodeLabel barcode;
	shipment->commitRecord();
	initiateSearch(shipment->drug_id);
	barcode.setBarcode(QString("SID") + QString().setNum(shipment->id));
	barcode.print();
	shipmentCleanup(shipment);
}

void MedicationsFrame::shipmentCleanup(ShipmentRecord *shipment)
{
	delete shipment;
}
