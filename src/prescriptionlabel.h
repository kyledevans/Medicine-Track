/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QPrinter>

#include "db/prescriptionrecord.h"
#include "db/prescriberrecord.h"
#include "db/pharmacistrecord.h"
#include "db/patientrecord.h"
#include "db/drugrecord.h"
#include "db/shipmentrecord.h"

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
	explicit PrescriptionLabel(QWidget *parent = 0);
	explicit PrescriptionLabel(PrescriptionRecord *, QWidget *parent = 0);
	~PrescriptionLabel();

	void printLabel();	// Deletes the value in pres (TODO: this needs to change)

public slots:
	void print(QPrinter *);

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
