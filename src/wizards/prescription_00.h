/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QList>

#include "db/patientrecord.h"
#include "db/prescriptionrecord.h"

#ifndef PRESCRIPTION_00_H
#define PRESCRIPTION_00_H

#include <QWizardPage>

namespace Ui {
	class Prescription_00;
}

class Prescription_00 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Prescription_00(QWidget *parent = 0);
	~Prescription_00();

	void changeEvent(QEvent *);

	void setPatient(PatientRecord *);
	void setPrescription(PrescriptionRecord *);

	bool isComplete() const;

public slots:
	void initiateSearch();
	void resultSelected();

signals:
	void medicationChanged(int);

private:
	Ui::Prescription_00 *ui;

	PatientRecord *patient;
	PrescriptionRecord *prescription;

	bool db_queried;

	QList<int> ids;				// Contains the shipment id's of the current search
};

#endif // PRESCRIPTION_00_H
