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

InventoryFrame::InventoryFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::InventoryFrame),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->writeOffAction, SIGNAL(triggered()), this, SLOT(initiateWriteOff()));
	connect(ui->printBarcodeAction, SIGNAL(triggered()), this, SLOT(initiatePrintBarcode()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

	ui->resultTable->addAction(ui->modifyAction);
	ui->resultTable->addAction(ui->writeOffAction);
	ui->resultTable->addAction(ui->printBarcodeAction);
}

InventoryFrame::~InventoryFrame()
{
    delete ui;
}


/*  SQL without C++:
SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot,
shipments.product_count, shipments.product_left
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
		query = QString("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE drugs.name LIKE '%");
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
			query = QString("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE shipments.id = '");
			barcode.setBarcode(ui->barcodeField->text());
			query += QString().setNum(barcode.toID()) + QString("'");
		}
		query += QString(";");
	} else {
		query = QString("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE shipments.id = '");
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
	}

	db_queried = true;
}

void InventoryFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->modifyButton->setEnabled(true);
		ui->printBarcodeAction->setEnabled(true);
		ui->modifyAction->setEnabled(true);
	} else {
		ui->modifyButton->setEnabled(false);
		ui->printBarcodeAction->setEnabled(false);
		ui->modifyAction->setEnabled(false);
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
