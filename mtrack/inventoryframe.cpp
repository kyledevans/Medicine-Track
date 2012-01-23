/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>

#include "inventoryframe.h"
#include "ui_inventoryframe.h"

#include "altershipmentwizard.h"
#include "alertinterface.h"

InventoryFrame::InventoryFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::InventoryFrame),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));

	ui->resultTable->addAction(ui->modifyAction);
}

InventoryFrame::~InventoryFrame()
{
    delete ui;
}


/*  SQL without C++.  Numbers in () indicate column and are not SQL:
SELECT shipments.id, drugs.name, drugs.form, drugs.strength, drugs.amount, shipments.expiration, shipments.lot,
shipments.product_count, shipments.product_left
FROM shipments
JOIN drugs ON shipments.drug_id = drugs.id
WHERE drugs.name LIKE SOME_VAR
AND shipments.lot LIKE SOME_VAR
AND shipments.product_left > 0
AND drugs.active = 1
AND shipments.expiration < CURDATE();
*/
void InventoryFrame::initiateSearch(int shipID)
{
	QString query;
	QSqlQueryModel *model;
	AlertInterface alert;

	model = new QSqlQueryModel(ui->resultTable);

	if (shipID == SQL::Undefined_ID) {
		query = QString("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, drugs.amount, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE drugs.name LIKE '%");
		query += SQL::cleanInput(ui->nameField->text()) + QString("%' AND shipments.lot LIKE '%");
		query += SQL::cleanInput(ui->lotField->text()) + QString("%'");
		if (ui->stockCheckbox->isChecked()) {	// Stocked checkbox
			query += QString(" AND shipments.product_left > 0");
		}
		if (ui->activeCheckbox->isChecked()) {	// Active checkbox
			query += QString(" AND drugs.active = 1");
		}
		if (ui->expiredCheckbox->isChecked()) {	// Expired checkbox
			query += QString(" AND shipments.expiration < CURDATE()");
		}
		query += QString(";");
	} else {
		query = QString("SELECT shipments.id, drugs.name, drugs.form, drugs.strength, drugs.amount, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE shipments.id = '");
		query += QString().setNum(shipID) + QString("';");
	}

	if (!alert.attemptQuery(model, &query)) {
		delete model;
		return;
	}

	ids.clear();
	// Retrieve the ID's before we remove them from the display
	for (int i = 0; i < model->rowCount(); i++) {
		ids.append(model->record(i).value(0).toInt());
		//drugIds.append(model->record(i).value(1).toInt());
	}

	model->removeColumn(0);

	model->setHeaderData(0, Qt::Horizontal, tr("Medication"));
	model->setHeaderData(1, Qt::Horizontal, tr("Form"));
	model->setHeaderData(2, Qt::Horizontal, tr("Strength"));
	model->setHeaderData(3, Qt::Horizontal, tr("Amount"));
	model->setHeaderData(4, Qt::Horizontal, tr("Expiration"));
	model->setHeaderData(5, Qt::Horizontal, tr("Lot #"));
	model->setHeaderData(6, Qt::Horizontal, tr("Units ordered"));
	model->setHeaderData(7, Qt::Horizontal, tr("Units stocked"));

	ui->resultTable->setModel(model);

	db_queried = true;
	if (model->rowCount() > 0) {
		ui->modifyButton->setEnabled(true);
	} else {
		ui->modifyButton->setEnabled(false);
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
