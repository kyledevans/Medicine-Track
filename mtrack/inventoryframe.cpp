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
SELECT shipments.id, drugs.id, drugs.name, drugs.form, drugs.strength, drugs.amount, shipments.expiration, shipments.lot,
shipments.product_count, shipments.product_left
FROM shipments
JOIN drugs ON shipments.drug_id = drugs.id
WHERE drugs.name LIKE SOME_VAR
AND shipments.lot LIKE SOME_VAR
AND shipments.product_left > 0
AND drugs.active = 1
AND shipments.expiration > CURDATE();
*/
void InventoryFrame::initiateSearch()
{
	QString query;
	QSqlQueryModel *model = new QSqlQueryModel(ui->resultTable);
	drugIds.clear();

	query = QString("SELECT shipments.id, drugs.id, drugs.name, drugs.form, drugs.strength, drugs.amount, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE drugs.name LIKE '%");
	query += ui->nameField->text();
	query += QString("%' AND shipments.lot LIKE '%");
	query += ui->lotField->text();
	query += QString("%'");
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

	model->setQuery(query);

	// Retrieve the ID's before we remove them from the display
	for (int i = 0; i < model->rowCount(); i++) {
		shipmentIds.append(model->record(i).value(0).toInt());
		drugIds.append(model->record(i).value(1).toInt());
	}

	model->removeColumn(0);
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

/* SQL without C++.  Numbers in () indicate column and are not SQL:
SELECT shipments.drug_id(0), shipments.expiration(1), shipments.lot(2), shipments.product_count(3), shipments.product_left(4)
FROM shipments
WHERE id = 'SOME_VAR';
*/
void InventoryFrame::initiateModify()
{
/*	unsigned int row;
	QSqlQueryModel *model;
	QString query;
	ShipmentRecord *shipment;
	AlterShipmentWizard *wiz;

	if (db_queried) {
		if (ui->resultTable->selectionModel()->hasSelection()) {
			shipment = new ShipmentRecord;

			// This line finds the top row that was selected by the user
			row = ui->resultTable->selectionModel()->selectedRows()[0].row();
			shipment->id = shipmentIds[row];
		} else {
			return;
		}
	} else {
		return;
	}

	model = new QSqlQueryModel;

	query = QString("SELECT shipments.drug_id, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left FROM shipments WHERE id = '");
	query += QString().setNum(shipment->id) + QString("';");

	model->setQuery(query);

	shipment->drug_id = model->record(0).value(0).toInt();
	shipment->expiration = QDate().fromString(model->record(0).value(1).toString(), "yyyy-MM-dd");
	shipment->lot = model->record(0).value(2).toString();
	shipment->product_count = model->record(0).value(3).toInt();
	shipment->product_left = model->record(0).value(4).toInt();
	shipment->exists = true;

	wiz = new AlterShipmentWizard(shipment);

	delete model;

	connect(wiz, SIGNAL(wizardComplete(ShipmentRecord*)), this, SLOT(submitModify(ShipmentRecord*)));
	wiz->exec();
	delete wiz;*/
}

/* SQL without C++
UPDATE shipments
SET expiration = 'SOME_VAR', lot = 'SOME_VAR', product_count = 'SOME_VAR', product_left = 'SOME_VAR'
WHERE id = 'SOME_VAR';
*/
void InventoryFrame::submitModify(ShipmentRecord *shipment)
{
/*	QSqlQueryModel *model = new QSqlQueryModel;
	QString query;

	query = QString("UPDATE shipments SET expiration = '");
	query += shipment->expiration.toString("yyyy-MM-dd") + QString("', lot = '");
	query += shipment->lot + QString("', product_count = '");
	query += QString().setNum(shipment->product_count) + QString("', product_left = '");
	query += QString().setNum(shipment->product_left) + QString("' WHERE id = '");
	query += QString().setNum(shipment->id) + QString("';");

	qDebug() << query;
	model->setQuery(query);
	qDebug() << model->lastError().databaseText();*/
}
