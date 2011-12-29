/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

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
SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.amount, SUM( shipments.product_left )
FROM drugs
LEFT JOIN shipments ON drugs.id = shipments.drug_id
WHERE drugs.name LIKE SOME_VAR
AND drugs.active = SOME_VAR
GROUP BY drugs.id;
*/
void MedicationsFrame::initiateSearch()
{
	QString query;
	QSqlQueryModel *model = new QSqlQueryModel(ui->resultTable);
	drugIds.clear();

	query = QString("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.amount, SUM( shipments.product_left ) FROM drugs LEFT OUTER JOIN shipments ON drugs.id = shipments.drug_id WHERE drugs.name LIKE '%");
	query += ui->nameField->text();
	query += QString("%'");
	if (ui->activeCheckbox->isChecked()) {
		query += QString(" AND drugs.active = '1'");
	}
	query += QString(" GROUP BY drugs.id;");

	model->setQuery(query);

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
	unsigned int row;
	ShipmentRecord *shipment;
	AlterShipmentWizard *wiz;

	if (db_queried) {
		if (ui->resultTable->selectionModel()->hasSelection()) {
			shipment = new ShipmentRecord;

			// This line finds the top row that was selected by the user
			row = ui->resultTable->selectionModel()->selectedRows()[0].row();
			shipment->drug_id = drugIds[row];
		} else {
			return;
		}
	} else {
		return;
	}

	wiz = new AlterShipmentWizard(shipment);
	connect(wiz, SIGNAL(wizardComplete(ShipmentRecord*)), this, SLOT(submitNewShipment(ShipmentRecord*)));
	wiz->exec();
	delete wiz;
}

/* Readable SQL.  Numbers in () indicate the column number and are not SQL:
SELECT drugs.id(0), drugs.instruction_id(1), drugs.name(2), drugs.generic(3), drugs.manufacturer(4), drugs.ndc(5), drugs.form(6),
drugs.strength(7), drugs.amount(8), drugs.active(9), instructions.text(10)
FROM drugs
JOIN instructions ON drugs.instruction_id = instructions.id
WHERE drugs.id = SOME_VAR
*/
void MedicationsFrame::initiateModify()
{
	unsigned int row;
	MedicationRecord *med;
	AlterMedicationWizard *wiz;
	QSqlQueryModel *model;
	QString query;

	if (db_queried) {
		if (ui->resultTable->selectionModel()->hasSelection()) {
			med = new MedicationRecord;

			// This line finds the top row that was selected by the user
			row = ui->resultTable->selectionModel()->selectedRows()[0].row();
			med->id = drugIds[row];
		} else {
			return;
		}
	} else {
		return;
	}

	// Allocate stuff yo.
	model = new QSqlQueryModel;

	/* Populate the med record from the database */
	query = QString("SELECT drugs.id, drugs.instruction_id, drugs.name, drugs.generic, drugs.manufacturer, drugs.ndc, drugs.form, drugs.strength, drugs.amount, drugs.active, instructions.text FROM drugs JOIN instructions ON drugs.instruction_id = instructions.id WHERE drugs.id = '");
	query += QString::number(med->id);
	query += QString("';");
	model->setQuery(query);

	med->instruction_id = model->record(0).value(1).toInt();
	med->name = model->record(0).value(2).toString();
	med->generic = model->record(0).value(3).toString();
	med->manufacturer = model->record(0).value(4).toString();
	med->ndc = model->record(0).value(5).toString();
	med->form = FORM::sqlToForm(model->record(0).value(6).toString());
	med->strength = model->record(0).value(7).toString();
	med->amount = model->record(0).value(8).toString();
	med->active = model->record(0).value(9).toBool();
	med->instructions = model->record(0).value(10).toString();
	med->exists = true;

	wiz = new AlterMedicationWizard(med);

	delete model;

	connect(wiz, SIGNAL(wizardComplete(MedicationRecord*)), this, SLOT(submitModify(MedicationRecord*)));

	wiz->exec();
	delete wiz;
}

void MedicationsFrame::initiateNewMed()
{
	MedicationRecord *med = new MedicationRecord;
	AlterMedicationWizard *wiz = new AlterMedicationWizard(med);

	connect(wiz, SIGNAL(wizardComplete(MedicationRecord *)), this, SLOT(submitNewMed(MedicationRecord *)));

	wiz->exec();
	delete wiz;
}

/*
UPDATE instructions
SET text = 'SOME_VAR'
WHERE instructions.id = 'SOME_VAR';

UPDATE drugs
SET name = 'SOME_VAR', generic = 'SOME_VAR', manufacturer = 'SOME_VAR', ndc = 'SOME_VAR',
form = 'SOME_VAR', strength = 'SOME_VAR', amount = 'SOME_VAR', active = 'SOME_VAR'
WHERE drugs.id = 'SOME_VAR';
*/
void MedicationsFrame::submitModify(MedicationRecord *med)
{
	QSqlQueryModel *model = new QSqlQueryModel();
	QString query;

	// Update the instructions
	query = QString("UPDATE instructions SET text = '");
	query += med->instructions;
	query += QString("' WHERE instructions.id = '");
	query += QString().setNum(med->instruction_id);
	query += QString("';");

	model->setQuery(query);

	// Update the 'drugs' entry
	query = QString("UPDATE drugs SET name = '");
	query += med->name + QString("', generic = '");
	query += med->generic + QString("', manufacturer = '");
	query += med->ndc + QString("', form = '");
	query += SQL::formToSql(med->form) + QString("', strength = '");
	query += med->strength + QString("', amount = ");
	if ((med->form == FORM::Elixir) || (med->form == FORM::Suspension)) {
		query += QString("'") + med->amount + QString("', ");
	} else {
		query += QString("NULL, ");
	}
	query += QString("active = '");
	if (med->active == true) {
		query += QString("1' ");
	} else {
		query += QString("0' ");
	}
	query += QString(" WHERE drugs.id = '");
	query += QString().setNum(med->id) + QString("';");

	model->setQuery(query);
}

/* SQL without C++:
INSERT INTO instructions (text)
VALUES ('SOME_VAR');

INSERT INTO drugs (instruction_id, name, generic, manufacturer, ndc, form, strength, amount, active)
VALUES (...)

*/
void MedicationsFrame::submitNewMed(MedicationRecord *med)
{
	QSqlQueryModel *model = new QSqlQueryModel();
	QString query;

	query = QString("INSERT INTO instructions (text) VALUES (\"");
	query += med->instructions;
	query += QString("\");");

	model->setQuery(query);	// Create the instructions entry

	// Only insert an amount if applicable
	if ((med->form == FORM::Elixir) || (med->form == FORM::Suspension)) {
		query = QString("INSERT INTO drugs (instruction_id, name, generic, manufacturer, ndc, form, strength, amount, active) VALUES ('");
	} else {
		query = QString("INSERT INTO drugs (instruction_id, name, generic, manufacturer, ndc, form, strength, active) VALUES ('");
	}
	query += QString().setNum(model->query().lastInsertId().toInt());	// gets the new instructions.id from previous query
	query += QString("','");
	query += med->name + QString("','");
	query += med->generic + QString("','");
	query += med->manufacturer + QString("','");
	query += med->ndc + QString("','");
	query += SQL::formToSql(med->form) + QString("','");
	if ((med->form == FORM::Elixir) || (med->form == FORM::Suspension)) {
		query += med->amount + QString("','");
	}
	query += med->strength + QString("','");
	if (med->active == true) {
		query += QString("1');");
	} else {
		query += QString("0');");
	}

	model->setQuery(query);
}

/* SQL without C++:
INSERT INTO shipments (drug_id, expiration, lot, product_count, product_left)
VALUES (...)
*/
void MedicationsFrame::submitNewShipment(ShipmentRecord *shipment)
{
	QSqlQueryModel *model = new QSqlQueryModel();
	QString query;

	query = QString("INSERT INTO shipments (drug_id, expiration, lot, product_count, product_left) VALUES ('");
	query += QString().setNum(shipment->drug_id) + QString("','");
	query += shipment->expiration.toString("yyyy-MM-dd") + QString("','");
	query += shipment->lot + QString("','");
	query += QString().setNum(shipment->product_count) + QString("','");
	query += QString().setNum(shipment->product_left) + QString("');");

	model->setQuery(query);
}
