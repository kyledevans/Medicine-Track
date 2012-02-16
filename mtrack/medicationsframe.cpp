/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QVariant>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableWidgetItem>

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
	ui->resultTable->addAction(ui->modifyAction);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->newMedicationAction, SIGNAL(triggered()), this, SLOT(initiateNewMed()));
	connect(ui->newStockAction, SIGNAL(triggered()), this, SLOT(initiateNewShipment()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

	selectionChanged();
}

MedicationsFrame::~MedicationsFrame()
{
    delete ui;
}

/* SQL command without C++
SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, CONCAT(drugs.strength, ' ', drugs.str_units), drugs.unit_size, CONCAT(SUM( shipments.product_left ), ' ', drugs.dispense_units)
FROM drugs
LEFT JOIN shipments ON drugs.id = shipments.drug_id
WHERE drugs.name LIKE 'SOME_VAR'
AND drugs.active = 'SOME_VAR'
GROUP BY drugs.id;
*/
void MedicationsFrame::initiateSearch(int medID)
{
	QString query;
    QSqlQuery *model;
	AlertInterface alert;
	BarcodeLabel barcode;
	ShipmentRecord shipment;
    int i;      // Increment var

	if (medID == SQL::Undefined_ID) {
        query = QString("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, CONCAT(drugs.strength, ' ', drugs.str_units), drugs.unit_size, CONCAT(SUM( shipments.product_left ), ' ', drugs.dispense_units) FROM drugs LEFT OUTER JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.name LIKE '%");
		query += SQL::cleanInput(ui->nameField->text()) + QString("%'");
		if (ui->activeCheckbox->isChecked()) {
			query += QString(" AND drugs.active = '1'");
		}
		query += QString(" GROUP BY drugs.id;");
		barcode.setBarcode(ui->nameField->text());
		if (barcode.toID() != SQL::Undefined_ID) {
			shipment.retrieve(barcode.toID());
            query = QString("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, CONCAT(drugs.strength, ' ', drugs.str_units), drugs.unit_size, CONCAT(SUM( shipments.product_left ), ' ', drugs.dispense_units) FROM drugs LEFT OUTER JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.id = '");
			query += QString().setNum(shipment.drug_id) + QString("' GROUP BY drugs.id;");
		}
	} else {
        query = QString("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, CONCAT(drugs.strength, ' ', drugs.str_units), drugs.unit_size, CONCAT(SUM( shipments.product_left ), ' ', drugs.dispense_units) FROM drugs LEFT JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.id = '");
		query += QString().setNum(medID) + QString("' GROUP BY drugs.id;");
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
		ids.append(model->value(0).toInt());	// Retrieve the ID's before they get deleted
        ui->resultTable->setItem(i, 0, new QTableWidgetItem(model->value(1).toString()));
        ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(2).toString()));
        ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(3).toString()));
        ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(4).toString()));
        ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(5).toString()));
        ui->resultTable->setItem(i, 5, new QTableWidgetItem(model->value(6).toString()));
    }

	db_queried = true;
    delete model;
}

void MedicationsFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->newStockButton->setEnabled(true);
		ui->newStockAction->setEnabled(true);
		ui->modifyButton->setEnabled(true);
		ui->modifyAction->setEnabled(true);
	} else {
		ui->newStockButton->setEnabled(false);
		ui->newStockAction->setEnabled(false);
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
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
