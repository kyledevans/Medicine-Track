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
		presTemp->setId(model.value(0).toInt());
		presTemp->setLast(model.value(1).toString());
		presTemp->setFirst(model.value(2).toString());
		presTemp->setFull_name(model.value(3).toString());
		presTemp->setActive(model.value(4).toBool());
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
		pharmTemp->setId(model.value(0).toInt());
		pharmTemp->setLast(model.value(1).toString());
		pharmTemp->setFirst(model.value(2).toString());
		pharmTemp->setInitials(model.value(3).toString());
		pharmTemp->setActive(model.value(4).toBool());
		pharmacists.append(pharmTemp);
	}

	setupComboBoxes();

	return true;
}

void Prescription_01::medUpdated()
{
	ui->medicationLabel->setText(medication->getName());
	ui->formLabel->setText(FORM_STR::intToStr(medication->getForm()));
	ui->strengthLabel->setText(medication->getStrength());
	ui->unitSizeLabel->setText(medication->getUnit_size());
	ui->dispenseUnitsLabel->setText(medication->getDispense_units());
}

void Prescription_01::setupComboBoxes()
{
	int i;

	for (i = 0; i < prescribers.size(); i++) {
		ui->writtenByField->addItem(prescribers[i]->getFull_name(), QVariant(prescribers[i]->getId()));
	}

	for (i = 0; i < pharmacists.size(); i++) {
		ui->filledByField->addItem(pharmacists[i]->getLast() + QString(", ") + pharmacists[i]->getFirst(), QVariant(pharmacists[i]->getId()));
	}

	ui->writtenByField->setCurrentIndex(-1);
	ui->filledByField->setCurrentIndex(-1);
}

void Prescription_01::getResults()
{
	int temp;
	prescription->setAmount(ui->amountField->text().toInt());
	prescription->setWritten(ui->writtenField->date());
	prescription->setFilled(ui->filledField->date());

	temp = ui->writtenByField->currentIndex();
	prescription->setPrescriber_id(ui->writtenByField->itemData(temp).toInt());
	temp = ui->filledByField->currentIndex();
	prescription->setPharmacist_id(ui->filledByField->itemData(temp).toInt());
}

void Prescription_01::setPatient(PatientRecord *new_patient)
{
	patient = new_patient;

	ui->lastLabel->setText(patient->getLast());
	ui->firstLabel->setText(patient->getFirst());
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

void Prescription_01::changeEvent(QEvent *e)
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
