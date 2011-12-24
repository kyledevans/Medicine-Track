/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQueryModel>
#include <QSqlRecord>

#include <QDebug>

#include "medicationsframe.h"
#include "ui_medicationsframe.h"

MedicationsFrame::MedicationsFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::MedicationsFrame),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));

	ui->resultTable->addAction(ui->modifyAction);
}

MedicationsFrame::~MedicationsFrame()
{
    delete ui;
}

/* SQL command without C++
SELECT drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.amount, SUM( shipments.product_left )
FROM drugs
JOIN shipments ON drugs.id = shipments.drug_id
WHERE drugs.name LIKE SOME_VAR
AND drugs.active = SOME_VAR
GROUP BY shipments.drug_id;
*/
void MedicationsFrame::initiateSearch()
{
	QString query;
	QSqlQueryModel *model = new QSqlQueryModel(ui->resultTable);
	drugIds.clear();

	query = QString("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.amount, SUM( shipments.product_left ) FROM drugs JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.name LIKE '%");
	query += ui->nameField->text();
	query += QString("%'");
	if (ui->activeCheckbox->isChecked()) {
		query += QString(" AND drugs.active = '1'");
	}
	query += QString(" GROUP BY shipments.drug_id;");

	model->setQuery(query);

	for (int i = 0; i < model->rowCount(); i++) {
		drugIds.append(model->record(i).value(0).toInt());
	}

	model->removeColumn(0);

	model->setHeaderData(0, Qt::Horizontal, tr("Medication"));
	model->setHeaderData(1, Qt::Horizontal, tr("NDC"));
	model->setHeaderData(2, Qt::Horizontal, tr("Form"));
	model->setHeaderData(3, Qt::Horizontal, tr("Strength"));
	model->setHeaderData(4, Qt::Horizontal, tr("Amount"));
	model->setHeaderData(5, Qt::Horizontal, tr("Stock"));

	ui->resultTable->setModel(model);

	db_queried = true;
	if (model->rowCount() > 0) {
		ui->modifyButton->setEnabled(true);
	} else {
		ui->modifyButton->setEnabled(false);
	}
}

void MedicationsFrame::initiateModify()
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
