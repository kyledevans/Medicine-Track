/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescription_01.h"
#include "ui_prescription_01.h"

#include <QIntValidator>

#include "db/alertinterface.h"
#include "globals.h"

#include <QDebug>

Prescription_01::Prescription_01(QWidget *parent) :
	QWizardPage(parent),
    ui(new Ui::Prescription_01),
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

	registerField("amountField*", ui->amountField);
	registerField("writtenByField*", ui->writtenByField);
	registerField("filledByField*", ui->filledByField);
}

Prescription_01::~Prescription_01()
{
	delete ui;
	deleteLists();
}

void Prescription_01::deleteLists()
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
WHERE active = 1
ORDER BY last, first;

SELECT id, last, first, initials, active
FROM pharmacists
WHERE active = 1
ORDER BY last, first;
*/
bool Prescription_01::initCustom()
{
	QSqlQuery model;				// Contains the query
	AlertInterface alert;
	PrescriberRecord *presTemp;
	PharmacistRecord *pharmTemp;
	int i;							// Increment var

	model.prepare("SELECT id, last, first, full_name, active "
				  "FROM prescribers "
				  "WHERE active = 1 "
				  "ORDER BY last, first;");

	if (!alert.attemptQuery(&model)) {
		return false;
	}
	for (i = 0; i < model.size(); i++) {
		model.next();
		presTemp = new PrescriberRecord;
		presTemp->id = model.value(0).toInt();
		presTemp->last = model.value(1).toString();
		presTemp->first = model.value(2).toString();
		presTemp->full_name = model.value(3).toString();
		presTemp->active = model.value(4).toBool();
		prescribers.append(presTemp);
	}

	model.prepare("SELECT id, last, first, initials, active "
				  "FROM pharmacists "
				  "WHERE active = 1 "
				  "ORDER BY last, first;");

	if (!alert.attemptQuery(&model)) {
		deleteLists();
		return false;
	}
	for (i = 0; i < model.size(); i++) {
		model.next();
		pharmTemp = new PharmacistRecord;
		pharmTemp->id = model.value(0).toInt();
		pharmTemp->last = model.value(1).toString();
		pharmTemp->first = model.value(2).toString();
		pharmTemp->initials = model.value(3).toString();
		pharmTemp->active = model.value(4).toBool();
		pharmacists.append(pharmTemp);
	}

	setupComboBoxes();

	return true;
}

void Prescription_01::medUpdated()
{
	ui->medicationLabel->setText(medication->name);
	ui->formLabel->setText(FORM_STR::intToStr(medication->form));
	ui->strengthLabel->setText(medication->strength);
	ui->unitSizeLabel->setText(medication->unit_size);
	ui->dispenseUnitsLabel->setText(medication->dispense_units);
}

void Prescription_01::setupComboBoxes()
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

void Prescription_01::getResults()
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

void Prescription_01::setPatient(PatientRecord *new_patient)
{
	patient = new_patient;

	ui->lastLabel->setText(patient->last);
	ui->firstLabel->setText(patient->first);
}

void Prescription_01::setMedication(DrugRecord *new_medication)
{
	medication = new_medication;
}

void Prescription_01::setPrescription(PrescriptionRecord *new_prescription)
{
	prescription = new_prescription;
}

void Prescription_01::setShipment(ShipmentRecord *new_shipment)
{
	shipment = new_shipment;
}
