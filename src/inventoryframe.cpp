﻿/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "inventoryframe.h"
#include "ui_inventoryframe.h"

#include <QInputDialog>

#include "alertinterface.h"
#include "drugrecord.h"
#include "shipmentdisplay.h"
#include "shipmentwizard.h"
#include "barcodelabel.h"

#include <QDebug>

InventoryFrame::InventoryFrame(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::InventoryFrame)
{
	ui->setupUi(this);

	ui->resultTable->postSetup();

	// Connecting various signals/slots...
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->writeOffAction, SIGNAL(triggered()), this, SLOT(initiateWriteOff()));
	connect(ui->printBarcodeAction, SIGNAL(triggered()), this, SLOT(initiatePrintBarcode()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->toggleAction, SIGNAL(triggered()), this, SLOT(toggleActive()));
	connect(ui->increaseAction, SIGNAL(triggered()), this, SLOT(initiateIncrease()));
	connect(ui->viewAction, SIGNAL(triggered()), this, SLOT(viewInventory()));

	// Add items to the resultTable right-click menu
	ui->resultTable->addAction(ui->writeOffAction);
	ui->resultTable->addAction(ui->printBarcodeAction);
	ui->resultTable->addAction(ui->toggleAction);
	ui->resultTable->addAction(ui->increaseAction);
	ui->resultTable->addAction(ui->viewAction);

	// Hide the column with internal id's from the user
	ui->resultTable->hideColumn(0);

	// Disable actions that require a selection in the resultTable
	selectionChanged();
}

InventoryFrame::~InventoryFrame()
{
	delete ui;
}

void InventoryFrame::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		resizeHeaders();
		break;
	default:
		break;
	}
}

void InventoryFrame::resizeHeaders()
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

void InventoryFrame::viewInventory()
{
	unsigned int row;
	ShipmentDisplay *display;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	display = new ShipmentDisplay(ui->resultTable->item(row, 0)->text().toInt());
}

void InventoryFrame::toggleActive()
{
	unsigned int row;
	ShipmentRecord shipment;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	shipment.retrieve(ui->resultTable->item(row, 0)->text().toInt());
	shipment.toggleActive();
	initiateSearch(shipment.getId());
}

void InventoryFrame::resetPressed()
{
	ui->activeCheckbox->setChecked(true);
	ui->expiredCheckbox->setChecked(false);
	ui->stockCheckbox->setChecked(true);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
}

/*  SQL without C++:
SELECT shipments.id, drugs.name, drugs.form, drugs.strength, drugs.unit_size, shipments.expiration, shipments.lot,
shipments.product_count, shipments.product_left, shipments.write_off, active
FROM shipments
JOIN drugs ON shipments.drug_id = drugs.id
WHERE drugs.name LIKE ?
AND drugs.active = 1
AND shipments.lot LIKE ?
AND shipments.active = ?
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

	if (shipID != SQL::Undefined_ID) {
		model->prepare("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, drugs.unit_size, shipments.expiration, shipments.lot, "
					   "shipments.product_count, shipments.product_left, shipments.write_off, shipments.active "
					   "FROM shipments "
					   "JOIN drugs ON shipments.drug_id = drugs.id "
					   "WHERE shipments.id = ?;");
		model->bindValue(0, QVariant(shipID));
	} else if (barcode.toID() != SQL::Undefined_ID) {	// Doing a barcode search
		model->prepare("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, drugs.unit_size, shipments.expiration, shipments.lot, "
					   "shipments.product_count, shipments.product_left, shipments.write_off, shipments.active "
					   "FROM shipments "
					   "JOIN drugs ON shipments.drug_id = drugs.id "
					   "WHERE shipments.id = ?;");
		model->bindValue(0, QVariant(barcode.toID()));
	} else {	// Doing a normal search
		model->prepare("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, drugs.unit_size, shipments.expiration, shipments.lot, "
					   "shipments.product_count, shipments.product_left, shipments.write_off, shipments.active "
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
		model->bindValue(3, QVariant(ui->expiredCheckbox->isChecked()));
		model->bindValue(4, QVariant(!ui->stockCheckbox->isChecked()));
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
		ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(1).toString()));
		ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(2).toString()));
		ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(3).toString()));
		ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(4).toString()));
		if (model->value(5).toDate() > QDate::currentDate()) {	// Not expired yet
			ui->resultTable->setItemFlag(i, 5, new QTableWidgetItem(model->value(5).toDate().toString(DEFAULTS::DateDisplayFormat)), true);
		} else {	// Expired
			ui->resultTable->setItemFlag(i, 5, new QTableWidgetItem(model->value(5).toDate().toString(DEFAULTS::DateDisplayFormat)), false);
		}
		ui->resultTable->setItem(i, 6, new QTableWidgetItem(model->value(6).toString()));
		ui->resultTable->setItem(i, 7, new QTableWidgetItem(model->value(7).toString()));
		if (model->value(8).toInt() > 0) {	// Current inventory
			ui->resultTable->setItemFlag(i, 8, new QTableWidgetItem(model->value(8).toString()), true);
		} else {	// Out of stock
			ui->resultTable->setItemFlag(i, 8, new QTableWidgetItem(model->value(8).toString()), false);
		}
		ui->resultTable->setItem(i, 9, new QTableWidgetItem(model->value(9).toString()));
		if (model->value(10).toBool()) {	// Active
			ui->resultTable->setItemFlag(i, 10, new QTableWidgetItem(QString("Active")), true);
		} else {	// Inactive
			ui->resultTable->setItemFlag(i, 10, new QTableWidgetItem(QString("Inactive")), false);
		}
	}
	ui->resultTable->setSortingEnabled(true);
	ui->resultTable->sortByColumn(1, Qt::AscendingOrder);
	resizeHeaders();

	delete model;
}

void InventoryFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->printBarcodeAction->setEnabled(true);
		ui->writeOffAction->setEnabled(true);
		ui->toggleAction->setEnabled(true);
		ui->increaseAction->setEnabled(true);
		ui->viewAction->setEnabled(true);
	} else {
		ui->printBarcodeAction->setEnabled(false);
		ui->writeOffAction->setEnabled(false);
		ui->toggleAction->setEnabled(false);
		ui->increaseAction->setEnabled(false);
		ui->viewAction->setEnabled(false);
	}
}

void InventoryFrame::initiateWriteOff()
{
	unsigned int row, wo_amount;
	ShipmentRecord *shipment;
	DrugRecord *medication;
	bool ok;

	shipment = new ShipmentRecord;
	medication = new DrugRecord;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!shipment->retrieve(ui->resultTable->item(row, 0)->text().toInt())) {
		delete shipment;
		delete medication;
		return;
	}
	if (!medication->retrieve(shipment->getDrug_id())) {
		delete shipment;
		delete medication;
		return;
	}

	// Get from the user how many to write off
	wo_amount = QInputDialog::getInt(this,
									 "Write off inventory",
									 "How many additional units do you want to write off?\nCurrently " + QString().setNum(shipment->getWrite_off()) + " " + medication->getDispense_units() + " are already written off.",
									 0,
									 0,
									 shipment->getProduct_left(),
									 1,
									 &ok);
	if (ok && wo_amount > 0) {
		shipment->addWriteOff(wo_amount);
	}

	initiateSearch(shipment->getId());
	delete shipment;
	delete medication;
}

void InventoryFrame::initiateIncrease()
{
	unsigned int row, delta;
	ShipmentRecord *shipment;
	DrugRecord *medication;
	bool ok;

	shipment = new ShipmentRecord;
	medication = new DrugRecord;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!shipment->retrieve(ui->resultTable->item(row, 0)->text().toInt())) {
		delete shipment;
		delete medication;
		return;
	}
	if (!medication->retrieve(shipment->getDrug_id())) {
		delete shipment;
		delete medication;
		return;
	}

	// Get from the user how many to write off
	delta = QInputDialog::getInt(this,
									 "Increase Inventory",
									 "How many units do you want to add to the inventory?",
									 0,
									 0,
									 99999999,
									 1,
									 &ok);
	if (ok && delta > 0) {
		shipment->addInventory(delta);
	}

	initiateSearch(shipment->getId());
	delete shipment;
	delete medication;
}

void InventoryFrame::initiatePrintBarcode()
{
	unsigned int row;
	BarcodeLabel label;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	label.setBarcode(QString("SID") + QString().setNum(ui->resultTable->item(row, 0)->text().toInt()));
	label.print();
}
