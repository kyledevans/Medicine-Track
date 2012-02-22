/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QIntValidator>

#include "apw_page01.h"
#include "ui_apw_page01.h"

#include "alertinterface.h"
#include "prescriberrecord.h"
#include "globals.h"

#include <QDebug>

APW_Page01::APW_Page01(QWidget *parent) :
    QWizardPage(parent),
	ui(new Ui::APW_Page01),
	patient(0),
	prescription(0),
	medication(0),
	shipment(0)
{
    ui->setupUi(this);

	// Setup UI strings and tooltips
	ui->amountLabel_2->setText(PrescriptionRecord::amount_Label);
	ui->amountLabel_2->setToolTip(PrescriptionRecord::amount_Tooltip);
	ui->amountField->setToolTip(PrescriptionRecord::amount_Tooltip);

	ui->writtenLabel->setText(PrescriptionRecord::written_Label);
	ui->writtenLabel->setToolTip(PrescriptionRecord::written_Tooltip);
	ui->writtenField->setToolTip(PrescriptionRecord::written_Tooltip);

	ui->filledLabel->setText(PrescriptionRecord::filled_Label);
	ui->filledLabel->setToolTip(PrescriptionRecord::filled_Tooltip);
	ui->filledField->setToolTip(PrescriptionRecord::filled_Tooltip);

	ui->writtenByLabel->setText(PrescriberRecord::full_name_Label_secondary);
	ui->writtenByLabel->setToolTip(PrescriberRecord::full_name_Tooltip_secondary);
	ui->writtenByField->setToolTip(PrescriberRecord::full_name_Tooltip_secondary);

	ui->filledByLabel->setText(PharmacistRecord::initials_Label_secondary);
	ui->filledByLabel->setToolTip(PharmacistRecord::initials_Tooltip_secondary);
	ui->filledByField->setToolTip(PharmacistRecord::initials_Tooltip_secondary);

	// Setup validators
	QValidator *numbers = new QIntValidator(this);
	ui->amountField->setValidator(numbers);

	// Set default values
	ui->filledField->setDate(QDate::currentDate());
	ui->writtenField->setDate(QDate::currentDate());
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
	QSqlQuery *model;				// Contains the query
	QString query;					// Contains the SQL query string
	AlertInterface alert;
	PrescriberRecord *presTemp;
	PharmacistRecord *pharmTemp;
	int i;							// Increment var

	// TODO: This isn't using model.prepare()
	model = new QSqlQuery;
	query = QString("SELECT id, last, first, full_name, active FROM prescribers WHERE active = '1' ORDER BY last, first;");

	if (!alert.attemptQuery(model, &query)) {
		delete model;
		return false;
	}
	for (i = 0; i < model->size(); i++) {
		model->next();
		presTemp = new PrescriberRecord();
		presTemp->id = model->value(0).toInt();
		presTemp->last = model->value(1).toString();
		presTemp->first = model->value(2).toString();
		presTemp->full_name = model->value(3).toString();
		presTemp->active = model->value(4).toBool();
		prescribers.append(presTemp);
	}
	delete model;

	query = QString("SELECT id, last, first, initials, active FROM pharmacists WHERE active = '1' ORDER BY last, first;");

	model = new QSqlQuery;
	if (!alert.attemptQuery(model, &query)) {
		deleteLists();
		delete model;
		return false;
	}
	for (i = 0; i < model->size(); i++) {
		model->next();
		pharmTemp = new PharmacistRecord();
		pharmTemp->id = model->value(0).toInt();
		pharmTemp->last = model->value(1).toString();
		pharmTemp->first = model->value(2).toString();
		pharmTemp->initials = model->value(3).toString();
		pharmTemp->active = model->value(4).toBool();
		pharmacists.append(pharmTemp);
	}

	delete model;

	setupComboBoxes();

	return true;
}

void APW_Page01::medUpdated()
{
	ui->medicationLabel->setText(medication->name);
	ui->formLabel->setText(FORM_STR::intToStr(medication->form));
	ui->strengthLabel->setText(medication->strength);
	ui->unitSizeLabel->setText(medication->unit_size);
	ui->dispenseUnitsLabel->setText(medication->dispense_units);
}

void APW_Page01::setupComboBoxes()
{
	int i;

	for (i = 0; i < prescribers.size(); i++) {
		ui->writtenByField->addItem(prescribers[i]->full_name, QVariant(prescribers[i]->id));
	}

	for (i = 0; i < pharmacists.size(); i++) {
		ui->filledByField->addItem(pharmacists[i]->last + QString(", ") + pharmacists[i]->first, QVariant(pharmacists[i]->id));
	}

	ui->writtenByField->setCurrentIndex(-1);
	ui->filledByField->setCurrentIndex(-1);
}

void APW_Page01::getResults()
{
	int temp;
	prescription->amount = ui->amountField->text().toInt();
	prescription->written = ui->writtenField->date();
	prescription->filled = ui->filledField->date();

	temp = ui->writtenByField->currentIndex();
	prescription->prescriber_id = ui->writtenByField->itemData(temp).toInt();
	temp = ui->filledByField->currentIndex();
	prescription->pharmacist_id = ui->filledByField->itemData(temp).toInt();
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
