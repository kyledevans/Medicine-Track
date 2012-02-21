/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QInputDialog>
#include <QTableWidgetItem>

#include <QDebug>

#include "inventoryframe.h"
#include "ui_inventoryframe.h"

#include "altershipmentwizard.h"
#include "alertinterface.h"
#include "barcodelabel.h"
#include "shipmentrecord.h"
#include "medicationrecord.h"

InventoryFrame::InventoryFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::InventoryFrame),
	db_queried(false)
{
	QTableWidgetItem *header;
    ui->setupUi(this);

	// Set the various strings and tooltips for the search interface
	ui->nameLabel->setText(ShipmentRecord::name_barcode_Label);
	ui->nameLabel->setToolTip(ShipmentRecord::name_barcode_Tooltip);
	ui->nameField->setToolTip(ShipmentRecord::name_barcode_Tooltip);

	ui->lotLabel->setText(ShipmentRecord::lot_Label);
	ui->lotLabel->setToolTip(ShipmentRecord::lot_Tooltip);
	ui->lotField->setToolTip(ShipmentRecord::lot_Tooltip);

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
	header->setText(ShipmentRecord::expiration_Label);
	header->setToolTip(ShipmentRecord::expiration_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(4);
	header->setText(ShipmentRecord::lot_Label);
	header->setToolTip(ShipmentRecord::lot_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(5);
	header->setText(ShipmentRecord::product_count_Label);
	header->setToolTip(ShipmentRecord::product_count_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(6);
	header->setText(ShipmentRecord::product_left_Label);
	header->setToolTip(ShipmentRecord::product_left_Tooltip);

	header = ui->resultTable->horizontalHeaderItem(7);
	header->setText(ShipmentRecord::write_off_Label);
	header->setToolTip(ShipmentRecord::write_off_Tooltip);

	ui->resultTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);

	// Connecting various signals/slots...
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->writeOffAction, SIGNAL(triggered()), this, SLOT(initiateWriteOff()));
	connect(ui->printBarcodeAction, SIGNAL(triggered()), this, SLOT(initiatePrintBarcode()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));

	// Add items to the resultTable right-click menu
	ui->resultTable->addAction(ui->writeOffAction);
	ui->resultTable->addAction(ui->printBarcodeAction);

	// Disable actions that require a selection in the resultTable
	selectionChanged();
}

InventoryFrame::~InventoryFrame()
{
    delete ui;
}

void InventoryFrame::resetPressed()
{
	ui->activeCheckbox->setChecked(true);
	ui->notExpiredCheckbox->setChecked(true);
	ui->stockCheckbox->setChecked(true);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
}

/*  SQL without C++:
SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot,
shipments.product_count, shipments.product_left, shipments.write_off
FROM shipments
JOIN drugs ON shipments.drug_id = drugs.id
WHERE drugs.name LIKE ?
AND drugs.active = 1
AND shipments.lot LIKE ?
AND (<true if NOT searching by active> OR (shipments.active = ?))
AND (<true if NOT searching by expired> OR (shipments.expiration > CURDATE()))
AND (<true if NOT searching by 'in stock'> OR (shipments.product_left > 0));
*/
void InventoryFrame::initiateSearch(int shipID)
{
	QSqlQuery *model;
	AlertInterface alert;
	BarcodeLabel barcode;
	int i;					// Increment var

	model = new QSqlQuery;
	barcode.setBarcode(ui->nameField->text());

	if (shipID != SQL::Undefined_ID) {	// TODO: Implement or remove this case

	} else if (barcode.toID() != SQL::Undefined_ID) {	// Doing a barcode search
		model->prepare("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot, "
					   "shipments.product_count, shipments.product_left, shipments.write_off "
					   "FROM shipments "
					   "JOIN drugs ON shipments.drug_id = drugs.id "
					   "WHERE shipments.id = ?;");
		model->bindValue(0, QVariant(barcode.toID()));
	} else {	// Doing a normal search
		model->prepare("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot, "
					   "shipments.product_count, shipments.product_left, shipments.write_off "
					   "FROM shipments "
					   "JOIN drugs ON shipments.drug_id = drugs.id "
					   "WHERE drugs.name LIKE ? "
					   "AND drugs.active = 1 "
					   "AND shipments.lot LIKE ? "
					   "AND shipments.active = ? "
					   "AND (? OR (shipments.expiration > CURDATE())) "
					   "AND (? OR (shipments.product_left > 0));");
		model->bindValue(0, SQL::prepWildcards(ui->nameField->text()));
		model->bindValue(1, SQL::prepWildcards(ui->lotField->text()));
		model->bindValue(2, QVariant(ui->activeCheckbox->isChecked()));
		model->bindValue(3, QVariant(!ui->notExpiredCheckbox->isChecked()));
		model->bindValue(4, QVariant(!ui->stockCheckbox->isChecked()));
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
		ids.append(model->value(0).toInt());    // Retrieve the ID's before they get deleted
		ui->resultTable->setItem(i, 0, new QTableWidgetItem(model->value(1).toString()));
		ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(2).toString()));
		ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(3).toString()));
		ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(4).toString()));
		ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(5).toString()));
		ui->resultTable->setItem(i, 5, new QTableWidgetItem(model->value(6).toString()));
		ui->resultTable->setItem(i, 6, new QTableWidgetItem(model->value(7).toString()));
		ui->resultTable->setItem(i, 7, new QTableWidgetItem(model->value(8).toString()));
	}

	db_queried = true;
}

void InventoryFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->printBarcodeAction->setEnabled(true);
		ui->writeOffAction->setEnabled(true);
	} else {
		ui->printBarcodeAction->setEnabled(false);
		ui->writeOffAction->setEnabled(false);
	}
}

void InventoryFrame::initiateModify()
{
	unsigned int row;
	AlterShipmentWizard *wiz;
	ShipmentRecord *shipment;

	shipment = new ShipmentRecord();

	if (db_queried) {
		if (!ui->resultTable->selectionModel()->hasSelection()) {
			return;
		}
	} else {
		return;
	}

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!shipment->retrieve(ids[row])) {
		return;
	}

	wiz = new AlterShipmentWizard(shipment);
	connect(wiz, SIGNAL(wizardComplete(ShipmentRecord*)), this, SLOT(submitModify(ShipmentRecord*)));
	connect(wiz, SIGNAL(wizardRejected(ShipmentRecord*)), this, SLOT(shipmentCleanup(ShipmentRecord*)));
	wiz->exec();

	delete wiz;
}

void InventoryFrame::submitModify(ShipmentRecord *shipment)
{
	shipment->commitRecord();
	initiateSearch(shipment->id);
	shipmentCleanup(shipment);
}

void InventoryFrame::shipmentCleanup(ShipmentRecord *shipment)
{
	delete shipment;
}

void InventoryFrame::initiateWriteOff()
{
	unsigned int row, wo_amount;
	ShipmentRecord *shipment;
	MedicationRecord *medication;
	bool ok;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	shipment = new ShipmentRecord;
	medication = new MedicationRecord;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!shipment->retrieve(ids[row])) {
		delete shipment;
		delete medication;
		return;
	}
	if (!medication->retrieve(shipment->drug_id)) {
		delete shipment;
		delete medication;
		return;
	}

	// Get from the user how many to write off
	wo_amount = QInputDialog::getInt(this,
									 "Write off inventory",
									 "How many additional units do you want to write off?\nCurrently " + QString().setNum(shipment->write_off) + " " + medication->dispense_units + " are already written off.",
									 0,
									 0,
									 shipment->product_left,
									 1,
									 &ok);
	if (ok && wo_amount > 0) {
		shipment->addWriteOff(wo_amount);
	}

	initiateSearch(shipment->id);
	delete shipment;
}

void InventoryFrame::initiatePrintBarcode()
{
	unsigned int row;
	BarcodeLabel label;

	if (db_queried) {
		if (!ui->resultTable->selectionModel()->hasSelection()) {
			return;
		}
	} else {
		return;
	}

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	label.setBarcode(QString("SID") + QString().setNum(ids[row]));
	label.print();
}
