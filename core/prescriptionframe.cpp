/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QMessageBox>

#include "prescriptionframe.h"
#include "ui_prescriptionframe.h"

#include "../db/alertinterface.h"
#include "../db/prescriptionrecord.h"
#include "../db/prescriptiondisplay.h"
#include "../db/drugrecord.h"
#include "../db/shipmentrecord.h"
#include "prescriptionlabel.h"
#include "../db/globals.h"

#include <QDebug>

PrescriptionFrame::PrescriptionFrame(QWidget *parent) :
	QFrame(parent),
    ui(new Ui::PrescriptionFrame)
{
	ui->setupUi(this);

	ui->resultTable->postSetup();

	// Setup signals/slots
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->printAction, SIGNAL(triggered()), this, SLOT(initiatePrint()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->invalidateAction, SIGNAL(triggered()), this, SLOT(invalidatePrescription()));
	connect(ui->viewAction, SIGNAL(triggered()), this, SLOT(viewPrescription()));

	// Add items to resultTable right-click menu
	ui->resultTable->addAction(ui->printAction);
	ui->resultTable->addAction(ui->invalidateAction);
	ui->resultTable->addAction(ui->viewAction);

	// Hide the column with internal id's from the user
	ui->resultTable->hideColumn(0);

	// Disable actions that require an item selected in the resultTable
	selectionChanged();
}

PrescriptionFrame::~PrescriptionFrame()
{
	delete ui;
}

void PrescriptionFrame::changeEvent(QEvent *e)
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

void PrescriptionFrame::viewPrescription()
{
	unsigned int row;
	PrescriptionDisplay *display;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	display = new PrescriptionDisplay(ui->resultTable->item(row, 0)->text().toInt());
}

void PrescriptionFrame::resetPressed()
{
	ui->filledField->setDate(DEFAULTS::Date);
	ui->dobField->setDate(DEFAULTS::Date);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
	ui->invalidField->setChecked(false);
}

void PrescriptionFrame::selectionChanged()
{
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->printAction->setEnabled(true);
		ui->invalidateAction->setEnabled(true);
		ui->viewAction->setEnabled(true);
	} else {
		ui->printAction->setEnabled(false);
		ui->invalidateAction->setEnabled(false);
		ui->viewAction->setEnabled(false);
	}
}

void PrescriptionFrame::initiateSearch()
{
	// Test if any field has been altered
	if (ui->medicationNameField->text().isEmpty() &&
		ui->lotField->text().isEmpty() &&
		ui->lastField->text().isEmpty() &&
		ui->firstField->text().isEmpty() &&
		(ui->filledField->date() == DEFAULTS::Date) &&
		(ui->dobField->date() == DEFAULTS::Date)) {

		if (ui->resultTable->rowCount() > 0) {
			ui->resetButton->click();
		}
		return;
	}

    QList<PrescriptionSearchItem *> *searchItems = bl.searchPrescriptions(ui->medicationNameField->text(), ui->lotField->text(), ui->filledField->date(), ui->firstField->text(), ui->lastField->text(), ui->dobField->date(), ui->activeField->isChecked(), ui->invalidField->isChecked());

    if (searchItems == nullptr)
        return;

    ui->resultTable->clearContents();
    ui->resultTable->setSortingEnabled(false);
    ui->resultTable->setRowCount(searchItems->size());

    PrescriptionSearchItem *item;
    for (int i = 0; i < searchItems->length(); i++) {
        item = searchItems->at(i);

        ui->resultTable->setItem(i, 0, new QTableWidgetItem(item->getPrescriptionID()));
        ui->resultTable->setItem(i, 1, new QTableWidgetItem(QString::number(item->getAllscriptsId())));
        ui->resultTable->setItem(i, 2, new QTableWidgetItem(item->getLastname()));
        ui->resultTable->setItem(i, 3, new QTableWidgetItem(item->getFirstname()));
        ui->resultTable->setItem(i, 4, new QTableWidgetItem(item->getDob().toString(DEFAULTS::DateDisplayFormat)));
        ui->resultTable->setItem(i, 5, new QTableWidgetItem(item->getDrugName()));
        ui->resultTable->setItem(i, 6, new QTableWidgetItem(item->getDrugForm()));
        ui->resultTable->setItem(i, 7, new QTableWidgetItem(item->getDrugStrength()));
        ui->resultTable->setItem(i, 8, new QTableWidgetItem(item->getPrescriptionAmount()));
        ui->resultTable->setItem(i, 9, new QTableWidgetItem(item->getPrescriptionWritten().toString(DEFAULTS::DateDisplayFormat)));
        ui->resultTable->setItem(i, 10, new QTableWidgetItem(item->getPrescriptionFilled().toString(DEFAULTS::DateDisplayFormat)));
        ui->resultTable->setItem(i, 11, new QTableWidgetItem(item->getShipmentLot()));
        ui->resultTable->setItemFlag(i, 12, new QTableWidgetItem(item->getPrescriptionActive() ? QString("Valid") : QString("Invalid")), item->getPrescriptionActive());
    }

    ui->resultTable->setSortingEnabled(true);
    ui->resultTable->sortByColumn(2, Qt::AscendingOrder);

    for (int i = searchItems->length() - 1; i >= 0; i--) {
        delete searchItems->takeAt(i);
    }
    delete searchItems;
}

void PrescriptionFrame::initiatePrint()
{
	PrescriptionLabel *label;
	unsigned int row;
	PrescriptionRecord *pres;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	pres = new PrescriptionRecord;
	pres->retrieve(ui->resultTable->item(row, 0)->text().toInt());
	label = new PrescriptionLabel(pres);
	label->print();

	delete label;
}

void PrescriptionFrame::invalidatePrescription()
{
	QMessageBox msg;
	PrescriptionRecord prescription;
	int val;
	unsigned int row;

	msg.setText("Verify prescription removal");
	msg.setInformativeText("Are you sure you want to remove this prescription permanently?");
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msg.setDefaultButton(QMessageBox::No);
	val = msg.exec();

	if (val == QMessageBox::Yes) {	// Yes pressed
		// This line finds the top row that was selected by the user
		row = ui->resultTable->selectionModel()->selectedRows()[0].row();

		// Retrieve and invalidate prescription
		prescription.retrieve(ui->resultTable->item(row, 0)->text().toInt());
		prescription.toggleActive();
		initiateSearch();
	}
}
