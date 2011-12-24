/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQueryModel>
#include <QSqlRecord>

#include <QDebug>

#include "inventoryframe.h"
#include "ui_inventoryframe.h"

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


/*  SQL without C++
SELECT drugs.id, drugs.name, drugs.form, drugs.strength, drugs.amount, shipments.expiration, shipments.lot,
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

	query = QString("SELECT drugs.id, drugs.name, drugs.form, drugs.strength, drugs.amount, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE drugs.name LIKE '%");
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
		drugIds.append(model->record(i).value(0).toInt());
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

	if (db_queried) {
		if (ui->resultTable->selectionModel()->hasSelection()) {
			// This line finds the top row that was selected by the user
			row = ui->resultTable->selectionModel()->selectedRows()[0].row();
			qDebug() << "drug.id = " << drugIds[row];
		}
	}
}
