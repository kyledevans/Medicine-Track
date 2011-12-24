/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQueryModel>

#include "inventoryframe.h"
#include "ui_inventoryframe.h"

InventoryFrame::InventoryFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InventoryFrame)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
}

InventoryFrame::~InventoryFrame()
{
    delete ui;
}


/*  SQL without C++
SELECT drugs.name, drugs.form, drugs.strength, drugs.amount, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left
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

	query = QString("SELECT drugs.name, drugs.form, drugs.strength, drugs.amount, shipments.expiration, shipments.lot, shipments.product_count, shipments.product_left FROM shipments JOIN drugs ON shipments.drug_id = drugs.id WHERE drugs.name LIKE '%");
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
		query += QString(" AND shipments.expiration > CURDATE()");
	}

	model->setQuery(query);
	model->setHeaderData(0, Qt::Horizontal, tr("Medication"));
	model->setHeaderData(1, Qt::Horizontal, tr("Form"));
	model->setHeaderData(2, Qt::Horizontal, tr("Strength"));
	model->setHeaderData(3, Qt::Horizontal, tr("Amount"));
	model->setHeaderData(4, Qt::Horizontal, tr("Expiration"));
	model->setHeaderData(5, Qt::Horizontal, tr("Lot #"));
	model->setHeaderData(6, Qt::Horizontal, tr("Units ordered"));
	model->setHeaderData(7, Qt::Horizontal, tr("Units stocked"));

	ui->resultTable->setModel(model);
}
