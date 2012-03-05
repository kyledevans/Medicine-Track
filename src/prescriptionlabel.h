/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QPrinter>

#include "prescriptionrecord.h"
#include "prescriberrecord.h"
#include "pharmacistrecord.h"
#include "patientrecord.h"
#include "drugrecord.h"
#include "shipmentrecord.h"

#ifndef PRESCRIPTIONLABEL_H
#define PRESCRIPTIONLABEL_H

#include <QFrame>

namespace Ui {
	class PrescriptionLabel;
}

class PrescriptionLabel : public QFrame
{
	Q_OBJECT

public:
	explicit PrescriptionLabel(PrescriptionRecord *, QWidget *parent = 0);
	~PrescriptionLabel();

	void print();

private slots:
	void printLabel(QPrinter *);

private:
	Ui::PrescriptionLabel *ui;

	PrescriptionRecord *prescription;
	PrescriberRecord *prescriber;
	PharmacistRecord *pharmacist;
	PatientRecord *patient;
	DrugRecord *medication;
	ShipmentRecord *shipment;
};

#endif // PRESCRIPTIONLABEL_H
