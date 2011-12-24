/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQueryModel>
#include <QDebug>

#include "medicationsframe.h"
#include "ui_medicationsframe.h"

MedicationsFrame::MedicationsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MedicationsFrame)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
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
GROUP BY shipments.drug_id
*/
void MedicationsFrame::initiateSearch()
{
	QString query;
	QSqlQueryModel *model = new QSqlQueryModel(ui->resultTable);

	query = QString("SELECT drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.amount, SUM( shipments.product_left ) FROM drugs JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.name LIKE '%");
	query += ui->nameField->text();
	query += QString("%'");
	if (ui->activeCheckbox->isChecked()) {
		query += QString(" AND drugs.active = '1'");
	}
	query += QString(" GROUP BY shipments.drug_id;");

	qDebug() << query;

	model->setQuery(query);
	model->setHeaderData(0, Qt::Horizontal, tr("Medication"));
	model->setHeaderData(1, Qt::Horizontal, tr("NDC"));
	model->setHeaderData(2, Qt::Horizontal, tr("Form"));
	model->setHeaderData(3, Qt::Horizontal, tr("Strength"));
	model->setHeaderData(4, Qt::Horizontal, tr("Amount"));
	model->setHeaderData(5, Qt::Horizontal, tr("Stock"));

	ui->resultTable->setModel(model);
}
