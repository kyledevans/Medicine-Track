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
	ui->nameLabel->setText(MedicationRecord::name_Label);
	ui->nameLabel->setToolTip(MedicationRecord::name_Tooltip);
	ui->nameField->setToolTip(MedicationRecord::name_Tooltip);

	ui->lotLabel->setText(ShipmentRecord::lot_Label);
	ui->lotLabel->setToolTip(ShipmentRecord::lot_Tooltip);
	ui->lotField->setToolTip(ShipmentRecord::lot_Tooltip);

	ui->barcodeLabel->setText(ShipmentRecord::barcode_Label);
	ui->barcodeLabel->setToolTip(ShipmentRecord::barcode_Tooltip);
	ui->barcodeField->setToolTip(ShipmentRecord::barcode_Tooltip);

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
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
}

/*  SQL without C++:
SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot,
shipments.product_count, shipments.product_left, shipments.write_off
FROM shipments
JOIN drugs ON shipments.drug_id = drugs.id
WHERE drugs.name LIKE '%SOME_VAR%'
AND shipments.lot LIKE '%SOME_VAR%'
AND shipments.product_left > 0
AND drugs.active = 'SOME_VAR'
AND shipments.active = 'SOME_VAR'
AND shipments.expiration < CURDATE();
*/
void InventoryFrame::initiateSearch(int shipID)
{
	QString query;
	QSqlQuery *model;
	AlertInterface alert;
	BarcodeLabel barcode;
	int i;					// Increment var

	if (shipID == SQL::Undefined_ID) {
		query = QString("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left, shipments.write_off FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE drugs.name LIKE '%");
		query += SQL::cleanInput(ui->nameField->text()) + QString("%' AND shipments.lot LIKE '%");
		query += SQL::cleanInput(ui->lotField->text()) + QString("%'");
		if (ui->stockCheckbox->isChecked()) {	// Stocked checkbox
			query += QString(" AND shipments.product_left > 0");
		}
		if (ui->activeCheckbox->isChecked()) {	// Active checkbox
			query += QString(" AND drugs.active = '1'");
			query += QString(" AND shipments.active = '1'");
		}
		if (ui->expiredCheckbox->isChecked()) {	// Expired checkbox
			query += QString(" AND shipments.expiration < CURDATE()");
		}
		if (!ui->barcodeField->text().isEmpty()) {	// If we're searching based on barcode, just ignore every other qualifier
			query = QString("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left, shipments.write_off FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE shipments.id = '");
			barcode.setBarcode(ui->barcodeField->text());
			query += QString().setNum(barcode.toID()) + QString("'");
		}
		query += QString(";");
	} else {
		query = QString("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left, shipments.write_off FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE shipments.id = '");
		query += QString().setNum(shipID) + QString("';");
	}

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
	bool ok;

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

	// Get from the user how many to write off
	wo_amount = QInputDialog::getInt(this, "Write off inventory", "How many additional units do you want to write off?", 0, 0, shipment->product_left, 1, &ok);
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
