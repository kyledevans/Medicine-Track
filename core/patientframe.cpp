/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientframe.h"
#include "ui_patientframe.h"

#include <QVariant>

#include "../db/alertinterface.h"
#include "../db/patientdisplay.h"
#include "../wizards/patientwizard.h"
#include "../wizards/prescriptionwizard.h"
#include "prescriptionlabel.h"
#include "bl.h"

#include <QDebug>

PatientFrame::PatientFrame(QWidget *parent) :
	QFrame(parent),
    ui(new Ui::PatientFrame)
{
	ui->setupUi(this);

	ui->resultTable->postSetup();
	ui->dobField->setDate(DEFAULTS::Date);

	// Setup signals/slots
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(initiateSearch()));
	connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetPressed()));
	connect(ui->prescribeAction, SIGNAL(triggered()), this, SLOT(initiatePrescription()));
	connect(ui->modifyAction, SIGNAL(triggered()), this, SLOT(initiateModification()));
	connect(ui->newPatientAction, SIGNAL(triggered()), this, SLOT(initiateNewPatient()));
	connect(ui->resultTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));
	connect(ui->toggleAction, SIGNAL(triggered()), this, SLOT(toggleActive()));
	connect(ui->viewAction, SIGNAL(triggered()), this, SLOT(viewPatient()));

	// Add actions to resultTable right-click menu
	ui->resultTable->addAction(ui->prescribeAction);
	ui->resultTable->addAction(ui->modifyAction);
	ui->resultTable->addAction(ui->toggleAction);
	ui->resultTable->addAction(ui->viewAction);

	// Hide the column with internal id's from the user
	ui->resultTable->hideColumn(0);

	// Deactivate actions that require an item selected in resultTable
	selectionChanged();
}

PatientFrame::~PatientFrame()
{
	delete ui;
}

void PatientFrame::changeEvent(QEvent *e)
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

void PatientFrame::viewPatient()
{
	int row;
	PatientDisplay *display;

	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	display = new PatientDisplay(ui->resultTable->item(row, 0)->text().toInt(), this);
}

void PatientFrame::toggleActive()
{
	int row;
	PatientRecord patient;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();

	patient.retrieve(ui->resultTable->item(row, 0)->text().toInt());
	patient.toggleActive();
	initiateSearch(patient.getId());
}

void PatientFrame::initiateSearch(int patientID)
{
    PatientRecord *patient = nullptr;
    QList<PatientRecord*> *patients;

	// If the text fields are empty, don't do anything.
	if (ui->firstNameField->text().isEmpty() &&
		ui->lastNameField->text().isEmpty() &&
		ui->firstNameField->text().isEmpty() &&
		(ui->dobField->date() == DEFAULTS::Date) &&
		(patientID == SQL::Undefined_ID))
	{
		if (ui->resultTable->rowCount() > 0) {
			ui->resetButton->click();
		}
		return;
	}

    if (patientID != SQL::Undefined_ID) {
        patients = new QList<PatientRecord *>();
        patients->append(bl.getPatient(patientID));
    } else {
        patients = bl.searchPatients(ui->firstNameField->text(), ui->lastNameField->text(), ui->dobField->date(), ui->activeField->isChecked());
    }

    if (patients == nullptr)
        return;

    ui->resultTable->clearContents();
    ui->resultTable->setSortingEnabled(false);
    ui->resultTable->setRowCount(patients->length());

    for (int i = 0; i < patients->length(); i++) {
        patient = (*patients)[i];
        ui->resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(patient->getId())));
        ui->resultTable->setItem(i, 1, new QTableWidgetItem(QString::number(patient->getAllscripts_id())));
        ui->resultTable->setItem(i, 2, new QTableWidgetItem(patient->getLast()));
        ui->resultTable->setItem(i, 3, new QTableWidgetItem(patient->getFirst()));
        ui->resultTable->setItem(i, 4, new QTableWidgetItem(patient->getDob().toString(DEFAULTS::DateDisplayFormat)));
        ui->resultTable->setItemFlag(i, 5, new QTableWidgetItem(patient->getActive() ? QString("Active") : QString("Inactive")), true);
    }

    for (int i = patients->length() - 1; i >= 0; i--) {
        delete patients->takeAt(i);
    }
    delete patients;

    ui->resultTable->setSortingEnabled(true);
    ui->resultTable->sortByColumn(1, Qt::AscendingOrder);

    if (patientID != SQL::Undefined_ID) {
        ui->resultTable->selectRow(0);
        ui->resultTable->setFocus();
    }
}

void PatientFrame::selectionChanged()
{
	// Enable/disable buttons if there is a selection
	if (ui->resultTable->selectionModel()->hasSelection()) {
		ui->prescribeButton->setEnabled(true);
		ui->prescribeAction->setEnabled(true);
		ui->modifyButton->setEnabled(true);
		ui->modifyAction->setEnabled(true);
		ui->toggleAction->setEnabled(true);
		ui->viewAction->setEnabled(true);
	} else {
		ui->prescribeButton->setEnabled(false);
		ui->prescribeAction->setEnabled(false);
		ui->modifyButton->setEnabled(false);
		ui->modifyAction->setEnabled(false);
		ui->toggleAction->setEnabled(false);
		ui->viewAction->setEnabled(false);
	}
}

void PatientFrame::resetPressed()
{
	ui->dobField->setDate(DEFAULTS::Date);
	ui->resultTable->clearContents();
	ui->resultTable->setRowCount(0);
}

void PatientFrame::initiatePrescription()
{
	unsigned int row;
	PrescriptionRecord *prescription;

	PrescriptionWizard *wiz;

	wiz = new PrescriptionWizard;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!wiz->getPatient(ui->resultTable->item(row, 0)->text().toInt())) {
		delete wiz;
		return;
	}

	prescription = new PrescriptionRecord;
	prescription->setPatient_id(ui->resultTable->item(row, 0)->text().toInt());
	wiz->setPrescription(prescription);

	connect(wiz, SIGNAL(wizardComplete(PrescriptionRecord*)), this, SLOT(submitPrescription(PrescriptionRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PrescriptionRecord*)), this, SLOT(prescriptionCleanup(PrescriptionRecord*)));
	wiz->exec();
}

void PatientFrame::initiateModification()
{
	unsigned int row;
	PatientWizard *wiz;
	PatientRecord *patient;

	patient = new PatientRecord;

	// This line finds the top row that was selected by the user
	row = ui->resultTable->selectionModel()->selectedRows()[0].row();
	if (!patient->retrieve(ui->resultTable->item(row, 0)->text().toInt())) {
		delete patient;
		return;
	}

	wiz = new PatientWizard(patient);
	connect(wiz, SIGNAL(wizardComplete(PatientRecord*)), this, SLOT(submitPatient(PatientRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PatientRecord*)), this, SLOT(patientCleanup(PatientRecord*)));
	wiz->exec();

	delete wiz;
}

void PatientFrame::initiateNewPatient()
{
	PatientWizard *wiz;
	PatientRecord *patient = new PatientRecord;

	wiz = new PatientWizard(patient);
	connect(wiz, SIGNAL(wizardComplete(PatientRecord*)), this, SLOT(submitPatient(PatientRecord*)));
	connect(wiz, SIGNAL(wizardRejected(PatientRecord*)), this, SLOT(patientCleanup(PatientRecord*)));
	wiz->exec();

	delete wiz;
}

void PatientFrame::submitPrescription(PrescriptionRecord *prescription)
{
	PrescriptionLabel label(prescription);
	if (prescription->commitRecord()) {
		label.print();
	}
	prescriptionCleanup(prescription);
}

void PatientFrame::prescriptionCleanup(PrescriptionRecord *prescription)
{
	delete prescription;
}

void PatientFrame::submitPatient(PatientRecord *patient)
{
	patient->commitRecord();
	initiateSearch(patient->getId());
	delete patient;
}

void PatientFrame::patientCleanup(PatientRecord *patient)
{
	delete patient;
}
