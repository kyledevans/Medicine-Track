/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "formularyframe.h"
#include "ui_formularyframe.h"

#include "alertinterface.h"
#include "drugdisplay.h"
#include "medicationwizard.h"
#include "shipmentwizard.h"
#include "barcodelabel.h"

#include <QDebug>

FormularyFrame::FormularyFrame(QWidget *parent) :
	QFrame(parent),
	ui(new Ui::FormularyFrame)
{
	ui->setupUi(this);

	ui->resultTable->postSetup();

	// Setup various signal/slot connections
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));
	connect(ui->newMedicationAction, SIGNAL(triggered()), this, SLOT(initiateNewMed()));
	connect(ui->newStockAction, SIGNAL(triggered()), this, SLOT(initiateNewShipment()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->toggleAction, SIGNAL(triggered()), this, SLOT(toggleActive()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->viewAction, SIGNAL(triggered()), this, SLOT(viewMedication()));

	// Add items to resultTable right-click menu
	ui->resultTable->addAction(ui->newStockAction);
	ui->resultTable->addAction(ui->modifyAction);
	ui->resultTable->addAction(ui->toggleAction);
	ui->resultTable->addAction(ui->viewAction);

	// Hide the column with internal id's from the user
	ui->resultTable->hideColumn(0);

	selectionChanged();
}

FormularyFrame::~FormularyFrame()
{
	delete ui;
}

void FormularyFrame::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void FormularyFrame::viewMedication()
{
	unsigned int row;
	DrugDisplay *display;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	display = new DrugDisplay(ui->resultTable->item(row, 0)->text().toInt());
}

void FormularyFrame::resetPressed()
{
	ui->activeCheckbox->setChecked(true);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
}

void FormularyFrame::toggleActive()
{
	unsigned int row;
    DrugRecord medication;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	medication.retrieve(ui->resultTable->item(row, 0)->text().toInt());
	medication.toggleActive();
	initiateSearch(medication.getId());
}

/* SQL command without C++
SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size,
CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units), drugs.active,
drugs.dispense_units, SUM( s_temp.product_left )
FROM drugs
LEFT JOIN (
	SELECT shipments.id, shipments.drug_id, shipments.product_left
	FROM shipments
	WHERE shipments.active = 1
	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id
WHERE drugs.name LIKE ?
AND drugs.active = ?
GROUP BY drugs.id;

SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size,
CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units), drugs.active,
drugs.dispense_units, SUM( s_temp.product_left )
FROM drugs
LEFT JOIN (
	SELECT shipments.id, shipments.drug_id, shipments.product_left
	FROM shipments
	WHERE shipments.active = 1
	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id
WHERE drugs.id = (
	SELECT shipments.drug_id
	FROM shipments
	WHERE shipments.id = ?)
GROUP BY drugs.id;
*/
void FormularyFrame::initiateSearch(int medID)
{
	QSqlQuery *model;
	AlertInterface alert;
	BarcodeLabel barcode;
	int i;      // Increment var

	barcode.setBarcode(ui->nameField->text());
	model = new QSqlQuery;

	if (medID != SQL::Undefined_ID) {	// Searching for a specific drug ID
		model->prepare("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size, "
					   "CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units), drugs.active, "
					   "drugs.dispense_units, SUM( s_temp.product_left ) "
					   "FROM drugs "
					   "LEFT JOIN ("
					   "	SELECT shipments.id, shipments.drug_id, shipments.product_left "
					   "	FROM shipments "
					   "	WHERE shipments.active = 1 "
					   "	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id "
					   "WHERE drugs.id = ? "
					   "GROUP BY drugs.id;");
		model->bindValue(0, medID);
	} else if (barcode.toID() != SQL::Undefined_ID) {	// There was a barcode, so search only for a matching barcode
		model->prepare("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size, "
					   "CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units), drugs.active, "
					   "drugs.dispense_units, SUM( s_temp.product_left ) "
					   "FROM drugs "
					   "LEFT JOIN ( "
					   "	SELECT shipments.id, shipments.drug_id, shipments.product_left "
					   "	FROM shipments "
					   "	WHERE shipments.active = 1 "
					   "	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id "
					   "WHERE drugs.id = ( "
					   "	SELECT shipments.drug_id "
					   "	FROM shipments "
					   "	WHERE shipments.id = ?) "
					   "GROUP BY drugs.id;");
		model->bindValue(0, QVariant(barcode.toID()));
	} else {		// Searching based on user input
		model->prepare("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size, "
					   "CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units), drugs.active, "
					   "drugs.dispense_units, SUM( s_temp.product_left ) "
					   "FROM drugs "
					   "LEFT JOIN ( "
					   "	SELECT shipments.id, shipments.drug_id, shipments.product_left "
					   "	FROM shipments "
					   "	WHERE shipments.active = 1 "
					   "	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id "
					   "WHERE drugs.name LIKE ? "
					   "AND drugs.active = ? "
					   "GROUP BY drugs.id;");
		model->bindValue(0, SQL::prepWildcards(ui->nameField->text()));
		model->bindValue(1, QVariant(ui->activeCheckbox->isChecked()));
	}

	if (!alert.attemptQuery(model)) {
		delete model;
		return;
	}

	ui->resultTable->clearContents();
	ui->resultTable->setSortingEnabled(false);
	ui->resultTable->setRowCount(model->size());
	for (i = 0; i < model->size(); i++) {
		model->next();
		ui->resultTable->setItem(i, 0, new QTableWidgetItem(model->value(0).toString()));
		ui->resultTable->setItem(i, 1, new QTableWidgetItem(model->value(1).toString()));
		ui->resultTable->setItem(i, 2, new QTableWidgetItem(model->value(2).toString()));
		ui->resultTable->setItem(i, 3, new QTableWidgetItem(model->value(3).toString()));
		ui->resultTable->setItem(i, 4, new QTableWidgetItem(model->value(4).toString()));
		ui->resultTable->setItem(i, 5, new QTableWidgetItem(model->value(5).toString()));
		if (model->value(9).toString() == QString("0")) {	// No inventory
			ui->resultTable->setItemFlag(i, 6, new QTableWidgetItem(QString("0 ") + model->value(8).toString()), false);
		} else {
			ui->resultTable->setItemFlag(i, 6, new QTableWidgetItem(model->value(6).toString()), true);
		}
		if (model->value(7).toBool()) {	// Active medication
			ui->resultTable->setItemFlag(i, 7, new QTableWidgetItem(QString("Active")), true);
		} else {
			ui->resultTable->setItemFlag(i, 7, new QTableWidgetItem(QString("Inactive")), false);
		}
	}
	ui->resultTable->setSortingEnabled(true);
	ui->resultTable->sortByColumn(1, Qt::AscendingOrder);

	delete model;
}

void FormularyFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->newStockButton->setEnabled(true);
		ui->newStockAction->setEnabled(true);
		ui->modifyButton->setEnabled(true);
		ui->modifyAction->setEnabled(true);
		ui->toggleAction->setEnabled(true);
		ui->viewAction->setEnabled(true);
	} else {
		ui->newStockButton->setEnabled(false);
		ui->newStockAction->setEnabled(false);
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
		ui->toggleAction->setEnabled(false);
		ui->viewAction->setEnabled(false);
	}
}

void FormularyFrame::initiateNewMed()
{
	MedicationWizard *wiz;
    DrugRecord *med;

	med = new DrugRecord;

	wiz = new MedicationWizard(med);
	connect(wiz, SIGNAL(wizardComplete(DrugRecord*)), this, SLOT(submitDrug(DrugRecord*)));
    connect(wiz, SIGNAL(wizardRejected(DrugRecord*)), this, SLOT(medCleanup(DrugRecord*)));
	wiz->exec();

	delete wiz;
}

void FormularyFrame::initiateModify()
{
	unsigned int row;
	MedicationWizard *wiz;
    DrugRecord *med;

	med = new DrugRecord;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!med->retrieve(ui->resultTable->item(row, 0)->text().toInt())) {
		delete med;
		return;
	}

	wiz = new MedicationWizard(med);
	connect(wiz, SIGNAL(wizardComplete(DrugRecord*)), this, SLOT(submitDrug(DrugRecord*)));
    connect(wiz, SIGNAL(wizardRejected(DrugRecord*)), this, SLOT(medCleanup(DrugRecord*)));
	wiz->exec();

	delete wiz;
}

void FormularyFrame::submitDrug(DrugRecord *med)
{
	med->commitRecord();
	initiateSearch(med->getId());
	delete med;
}

void FormularyFrame::medCleanup(DrugRecord *med)
{
	delete med;
}

// Assumes: ui->resultTable has > 0 search results and one is selected
void FormularyFrame::initiateNewShipment()
{
	unsigned int row;
	ShipmentRecord *ship;
	ShipmentWizard *wiz;

	ship = new ShipmentRecord;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	ship->setDrug_id(ui->resultTable->item(row, 0)->text().toInt());

	wiz = new ShipmentWizard(ship);

	connect(wiz, SIGNAL(wizardComplete(ShipmentRecord*)), this, SLOT(submitShipment(ShipmentRecord*)));
	connect(wiz, SIGNAL(wizardRejected(ShipmentRecord*)), this, SLOT(shipmentCleanup(ShipmentRecord*)));

	wiz->exec();

	delete wiz;
}

void FormularyFrame::submitShipment(ShipmentRecord *shipment)
{
	BarcodeLabel barcode;
	shipment->commitRecord();
	initiateSearch(shipment->getDrug_id());
	barcode.setBarcode(QString("SID") + QString().setNum(shipment->getId()));
	barcode.print();
	delete shipment;
}

void FormularyFrame::shipmentCleanup(ShipmentRecord *shipment)
{
	delete shipment;
}
