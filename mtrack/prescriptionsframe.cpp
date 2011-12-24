/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QSqlQueryModel>
#include <QSqlRecord>

#include <QDebug>

#include "prescriptionsframe.h"
#include "ui_prescriptionsframe.h"

PrescriptionsFrame::PrescriptionsFrame(QWidget *parent) :
    QFrame(parent),
	ui(new Ui::PrescriptionsFrame),
	db_queried(false)
{
    ui->setupUi(this);

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModify()));

	ui->resultTable->addAction(ui->modifyAction);
}

PrescriptionsFrame::~PrescriptionsFrame()
{
    delete ui;
}

void PrescriptionsFrame::resetPressed()
{
	ui->filledField->setDate(QDate(1970, 1, 31));	// Default date is 1/31/1970
	ui->dobField->setDate(QDate(1970, 1, 31));
}

/* SQL query without C++:
SELECT patients.id, prescriptions.id, drugs.id, shipments.id, patients.last, patients.first, patients.dob, drugs.name,
drugs.form, drugs.strength, prescriptions.amount, prescriptions.written, prescriptions.filled, shipments.lot
FROM prescriptions
JOIN patients ON prescriptions.patient_id = patients.id
JOIN drugs ON prescriptions.drug_id = drugs.id
JOIN shipments ON prescriptions.shipment_id = shipments.id
WHERE drugs.name LIKE SOMEVAR
AND shipments.lot LIKE SOMEVAR
AND prescriptions.filled = SOMEVAR
AND patients.last LIKE SOMEVAR
AND patients.first LIKE SOMEVAR
AND patients.dob = SOMEVAR;
*/
void PrescriptionsFrame::initiateSearch()
{
	QString query;
	presIds.clear();
	patientIds.clear();
	drugIds.clear();
	shipmentIds.clear();

	// Test if any field has been altered
	if (ui->medicationNameField->text() != QString("")
			|| (ui->lotField->text() != QString(""))
			|| (ui->lastField->text() != QString(""))
			|| (ui->firstField->text() != QString(""))
			|| (ui->filledField->date().toString("yyyy-MM-dd") != QString("1970-01-31"))
			|| (ui->dobField->date().toString("yyyy-MM-dd") != QString("1970-01-31"))) {

		QSqlQueryModel *model = new QSqlQueryModel(ui->resultTable);
		query = QString("SELECT patients.id, prescriptions.id, drugs.id, shipments.id, patients.last, patients.first, patients.dob, drugs.name, drugs.form, drugs.strength, prescriptions.amount, prescriptions.written, prescriptions.filled, shipments.lot FROM prescriptions JOIN patients ON prescriptions.patient_id = patients.id JOIN drugs ON prescriptions.drug_id = drugs.id JOIN shipments ON prescriptions.shipment_id = shipments.id WHERE drugs.name LIKE '%")
				+= ui->medicationNameField->text()
				+= QString("%' AND shipments.lot LIKE '%")
				+= ui->lotField->text()
				+= QString("%'");
				if (ui->filledField->date().toString("yyyy-MM-dd") != QString("1970-01-31")) {
					query += QString(" AND prescriptions.filled = '")
						  += ui->filledField->date().toString("yyyy-MM-dd")
						  += QString("'");
				}
				query += QString(" AND patients.last LIKE '%")
				+= ui->lastField->text()
				+= QString("%' AND patients.first LIKE '%")
				+= ui->firstField->text()
				+= QString("%'");
				if (ui->dobField->date().toString("yyyy-MM-dd") != QString("1970-01-31")) {
					query += QString("AND patients.dob = '")
						  += ui->dobField->date().toString("yyyy-MM-dd")
						  += QString("'");
				}
				query += QString(";");

		model->setQuery(query);

		// Store prescription ID's
		for (int i = 0; i < model->rowCount(); i++) {
			patientIds.append(model->record(i).value(0).toInt());
			presIds.append(model->record(i).value(1).toInt());
			drugIds.append(model->record(i).value(2).toInt());
			shipmentIds.append(model->record(i).value(3).toInt());
		}

		model->removeColumns(0, 4);

		model->setHeaderData(0, Qt::Horizontal, tr("Last name"));
		model->setHeaderData(1, Qt::Horizontal, tr("First name"));
		model->setHeaderData(2, Qt::Horizontal, tr("D.O.B."));
		model->setHeaderData(3, Qt::Horizontal, tr("Medication"));
		model->setHeaderData(4, Qt::Horizontal, tr("Form"));
		model->setHeaderData(5, Qt::Horizontal, tr("Strength"));
		model->setHeaderData(6, Qt::Horizontal, tr("Amount"));
		model->setHeaderData(7, Qt::Horizontal, tr("Written"));
		model->setHeaderData(8, Qt::Horizontal, tr("Filled"));
		model->setHeaderData(9, Qt::Horizontal, tr("Lot #"));

		ui->resultTable->setModel(model);

		db_queried = true;

		if (model->rowCount() > 0) {
			ui->modifyButton->setEnabled(true);
		} else {
			ui->modifyButton->setEnabled(false);
		}
	}
}

void PrescriptionsFrame::initiateModify()
{
	unsigned int row;

	if (db_queried) {
		if (ui->resultTable->selectionModel()->hasSelection()) {
			// This line finds the top row that was selected by the user
			row = ui->resultTable->selectionModel()->selectedRows()[0].row();
			qDebug() << "prescriptions.id = " << presIds[row] << " | patients.id = " << patientIds[row] << " | drugs.id = " << drugIds[row] << " | shipments.id = " << shipmentIds[row];
		}
	}
}

