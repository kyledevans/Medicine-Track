/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QVariant>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include <QDebug>

#include "medicationsframe.h"
#include "ui_medicationsframe.h"

#include "altermedicationwizard.h"
#include "altershipmentwizard.h"
#include "globals.h"

MedicationsFrame::MedicationsFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::MedicationsFrame),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	//connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->newMedicationAction, SIGNAL(triggered()), this, SLOT(initiateNewMed()));
	//connect(ui->newStockAction, SIGNAL(triggered()), this, SLOT(initiateNewShipment()));

	ui->resultTable->addAction(ui->modifyAction);
}

MedicationsFrame::~MedicationsFrame()
{
    delete ui;
}

/* SQL command without C++
SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.amount, SUM( shipments.product_left )
FROM drugs
LEFT JOIN shipments ON drugs.id = shipments.drug_id
WHERE drugs.name LIKE 'SOME_VAR'
AND drugs.active = 'SOME_VAR'
GROUP BY drugs.id;
*/
void MedicationsFrame::initiateSearch()
{
	QString query;
	QSqlQueryModel *model;

	query = QString("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.amount, SUM( shipments.product_left ) FROM drugs LEFT OUTER JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.name LIKE '%");
	query += ui->nameField->text();
	query += QString("%'");
	if (ui->activeCheckbox->isChecked()) {
		query += QString(" AND drugs.active = '1'");
	}
	query += QString(" GROUP BY drugs.id;");

	model = new QSqlQueryModel(ui->resultTable);
	model->setQuery(query);

	drugIds.clear();
	// Retrieve the ID's before we remove them from the display
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
		ui->newStockButton->setEnabled(true);
	} else {
		ui->modifyButton->setEnabled(false);
		ui->newStockButton->setEnabled(false);
	}
}

void MedicationsFrame::initiateNewShipment()
{

}

void MedicationsFrame::initiateModify()
{

}

void MedicationsFrame::initiateNewMed()
{
	AlterMedicationWizard *wiz;
	MedicationRecord *med = new MedicationRecord();

	wiz = new AlterMedicationWizard(med);
	connect(wiz, SIGNAL(wizardComplete(MedicationRecord*)), this, SLOT(submitNewMed(MedicationRecord*)));
	connect(wiz, SIGNAL(wizardRejected(MedicationRecord*)), this, SLOT(medCleanup(MedicationRecord*)));
	wiz->exec();

	delete wiz;
}

void MedicationsFrame::submitModify(MedicationRecord *med)
{

}

void MedicationsFrame::submitNewMed(MedicationRecord *med)
{
	med->commitRecord();
	medCleanup(med);
}

void MedicationsFrame::medCleanup(MedicationRecord *med)
{
	delete med;
}

void MedicationsFrame::submitNewShipment(ShipmentRecord *shipment)
{

}
