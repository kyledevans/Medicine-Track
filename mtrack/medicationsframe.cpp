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
#include "alertinterface.h"
#include "shipmentrecord.h"
#include "barcodelabel.h"

MedicationsFrame::MedicationsFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::MedicationsFrame),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->newMedicationAction, SIGNAL(triggered()), this, SLOT(initiateNewMed()));
	connect(ui->newStockAction, SIGNAL(triggered()), this, SLOT(initiateNewShipment()));

	ui->resultTable->addAction(ui->modifyAction);
}

MedicationsFrame::~MedicationsFrame()
{
    delete ui;
}

/* SQL command without C++
SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.str_units, drugs.dispense_units, drugs.unit_size, SUM( shipments.product_left )
FROM drugs
LEFT JOIN shipments ON drugs.id = shipments.drug_id
WHERE drugs.name LIKE 'SOME_VAR'
AND drugs.active = 'SOME_VAR'
GROUP BY drugs.id;
*/
void MedicationsFrame::initiateSearch(int medID)
{
	QString query;
	QSqlQueryModel *model;
	AlertInterface alert;
	BarcodeLabel barcode;
	ShipmentRecord shipment;

	if (medID == SQL::Undefined_ID) {
		query = QString("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.str_units, drugs.dispense_units, drugs.unit_size, SUM( shipments.product_left ) FROM drugs LEFT OUTER JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.name LIKE '%");
		query += SQL::cleanInput(ui->nameField->text()) + QString("%'");
		if (ui->activeCheckbox->isChecked()) {
			query += QString(" AND drugs.active = '1'");
		}
		query += QString(" GROUP BY drugs.id;");
		barcode.setBarcode(ui->nameField->text());
		if (barcode.toID() != SQL::Undefined_ID) {
			shipment.retrieve(barcode.toID());
			query = QString("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.str_units, drugs.dispense_units, drugs.unit_size, SUM( shipments.product_left ) FROM drugs LEFT OUTER JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.id = '");
			query += QString().setNum(shipment.drug_id) + QString("' GROUP BY drugs.id;");
			qDebug() << query;
		}
	} else {
		query = QString("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.str_units, drugs.dispense_units, drugs.unit_size, SUM( shipments.product_left ) FROM drugs LEFT JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.id = '");
		query += QString().setNum(medID) + QString("' GROUP BY drugs.id;");
	}

	model = new QSqlQueryModel(ui->resultTable);

	if (!alert.attemptQuery(model, &query)) {
		delete model;
		return;
	}

	ids.clear();
	// Retrieve the ID's before we remove them from the display
	for (int i = 0; i < model->rowCount(); i++) {
		ids.append(model->record(i).value(0).toInt());
	}

	model->removeColumn(0);

	model->setHeaderData(0, Qt::Horizontal, tr("Medication"));
	model->setHeaderData(1, Qt::Horizontal, tr("NDC"));
	model->setHeaderData(2, Qt::Horizontal, tr("Form"));
	model->setHeaderData(3, Qt::Horizontal, tr("Strength"));
	model->setHeaderData(4, Qt::Horizontal, tr("Units"));
	model->setHeaderData(5, Qt::Horizontal, tr("Dispensed units"));
	model->setHeaderData(6, Qt::Horizontal, tr("Unit size"));
	model->setHeaderData(7, Qt::Horizontal, tr("Stock"));

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

void MedicationsFrame::initiateNewMed()
{
	AlterMedicationWizard *wiz;
	MedicationRecord *med;

	med = new MedicationRecord();

	wiz = new AlterMedicationWizard(med);
	connect(wiz, SIGNAL(wizardComplete(MedicationRecord*)), this, SLOT(submitNewMed(MedicationRecord*)));
	connect(wiz, SIGNAL(wizardRejected(MedicationRecord*)), this, SLOT(medCleanup(MedicationRecord*)));
	wiz->exec();

	delete wiz;
}

void MedicationsFrame::initiateModify()
{
	unsigned int row;
	AlterMedicationWizard *wiz;
	MedicationRecord *med;

	if (db_queried) {
		if (!ui->resultTable->selectionModel()->hasSelection()) {
			return;
		}
	} else {
		return;
	}

	med = new MedicationRecord();

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!med->retrieve(ids[row])) {
		delete med;
		return;
	}

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
	initiateSearch(med->id);
	medCleanup(med);
}

void MedicationsFrame::medCleanup(MedicationRecord *med)
{
	delete med;
}

void MedicationsFrame::initiateNewShipment()
{
	unsigned int row;
	ShipmentRecord *ship;
	AlterShipmentWizard *wiz;

	if (!db_queried) {
		return;
	}
	if (!ui->resultTable->selectionModel()->hasSelection()) {
		return;
	}

	ship = new ShipmentRecord();
	wiz = new AlterShipmentWizard(ship);

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	ship->drug_id = ids[row];

	connect(wiz, SIGNAL(wizardComplete(ShipmentRecord*)), this, SLOT(submitNewShipment(ShipmentRecord*)));
	connect(wiz, SIGNAL(wizardRejected(ShipmentRecord*)), this, SLOT(shipmentCleanup(ShipmentRecord*)));

	wiz->exec();

	delete wiz;
}

void MedicationsFrame::submitNewShipment(ShipmentRecord *shipment)
{
	shipment->commitRecord();
	shipmentCleanup(shipment);
}

void MedicationsFrame::shipmentCleanup(ShipmentRecord *shipment)
{
	delete shipment;
}
