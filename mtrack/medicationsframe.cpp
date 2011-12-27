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

#include "altermedicationwizard.h"
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

	ui->resultTable->addAction(ui->modifyAction);
}

MedicationsFrame::~MedicationsFrame()
{
    delete ui;
}

/* SQL command without C++
SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.amount, SUM( shipments.product_left )
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
	} else {
		ui->modifyButton->setEnabled(false);
	}
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
	MedicationRecord *med = new MedicationRecord;
	AlterMedicationWizard *wiz;
	QSqlQueryModel *model = new QSqlQueryModel();
	QString query;

	if (db_queried) {
		if (ui->resultTable->selectionModel()->hasSelection()) {
			// This line finds the top row that was selected by the user
			row = ui->resultTable->selectionModel()->selectedRows()[0].row();
			med->id = drugIds[row];
			qDebug() << "drug.id = " << drugIds[row];
		}
	}

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

void MedicationsFrame::submitModify(MedicationRecord *med)
{
	qDebug() << "modify drugs.id = " << med->id;
}

void MedicationsFrame::submitNewMed(MedicationRecord *med)
{
	qDebug() << "new drugs.name = " << med->name;
}
