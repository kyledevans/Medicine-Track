/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include "apw_page01.h"
#include "ui_apw_page01.h"

#include "alertinterface.h"
#include "prescriberrecord.h"

APW_Page01::APW_Page01(QWidget *parent) :
    QWizardPage(parent),
	ui(new Ui::APW_Page01),
	patient(0),
	prescription(0),
	medication(0),
	shipment(0)
{
    ui->setupUi(this);
}

APW_Page01::~APW_Page01()
{
    delete ui;
	deleteLists();
}

void APW_Page01::deleteLists()
{
	while (!prescribers.isEmpty()) {
		delete prescribers.takeFirst();
	}

	while (!pharmacists.isEmpty()) {
		delete pharmacists.takeFirst();
	}
}

/* SQL without C++:
SELECT id, last, first, full_name, active
FROM prescribers
WHERE active = '1'
ORDER BY last, first;

SELECT id, last, first, initials, active
FROM pharmacists
WHERE active = '1'
ORDER BY last, first;
*/
bool APW_Page01::initCustom()
{
	int i;
	QString query;
	QSqlQueryModel *model;
	AlertInterface alert;
	PrescriberRecord *presTemp;
	PharmacistRecord *pharmTemp;

	model = new QSqlQueryModel();
	query = QString("SELECT id, last, first, full_name, active FROM prescribers WHERE active = '1' ORDER BY last, first;");

	if (!alert.attemptQuery(model, &query)) {
		delete model;
		return false;
	}
	for (i = 0; i < model->query().size(); i++) {
		presTemp = new PrescriberRecord();
		presTemp->id = model->record(i).value(0).toInt();
		presTemp->last = model->record(i).value(1).toString();
		presTemp->first = model->record(i).value(2).toString();
		presTemp->full_name = model->record(i).value(3).toString();
		presTemp->active = model->record(i).value(4).toBool();
		prescribers.append(presTemp);
	}

	query = QString("SELECT id, last, first, initials, active FROM pharmacists WHERE active = '1' ORDER BY last, first;");

	if (!alert.attemptQuery(model, &query)) {
		deleteLists();
		delete model;
		return false;
	}
	for (i = 0; i < model->query().size(); i++) {
		pharmTemp = new PharmacistRecord();
		pharmTemp->id = model->record(i).value(0).toInt();
		pharmTemp->last = model->record(i).value(1).toString();
		pharmTemp->first = model->record(i).value(2).toString();
		pharmTemp->initials = model->record(i).value(3).toString();
		pharmTemp->active = model->record(i).value(4).toBool();
		pharmacists.append(pharmTemp);
	}

	delete model;

	setupComboBoxes();

	return true;
}

void APW_Page01::setupComboBoxes()
{
	int i;

	for (i = 0; i < pharmacists.size(); i++) {
		ui->filledByField->addItem(pharmacists[i]->last + QString(", ") + pharmacists[i]->first, QVariant(pharmacists[i]->id));
	}

	for (i = 0; i < prescribers.size(); i++) {
		ui->writtenByField->addItem(prescribers[i]->full_name, QVariant(prescribers[i]->id));
	}

	ui->filledByField->setCurrentIndex(-1);
	ui->writtenByField->setCurrentIndex(-1);
}

void APW_Page01::setPatient(PatientRecord *new_patient)
{
	patient = new_patient;

	ui->lastLabel->setText(patient->last);
	ui->firstLabel->setText(patient->first);
}

void APW_Page01::setMedication(MedicationRecord *new_medication)
{
	medication = new_medication;
}

void APW_Page01::setPrescription(PrescriptionRecord *new_prescription)
{
	prescription = new_prescription;
}

void APW_Page01::setShipment(ShipmentRecord *new_shipment)
{
	shipment = new_shipment;
}
