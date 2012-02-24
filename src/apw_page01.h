/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef APW_PAGE01_H
#define APW_PAGE01_H

#include <QList>

#include <QWizardPage>

#include "db/patientrecord.h"
#include "medicationrecord.h"
#include "db/prescriptionrecord.h"
#include "shipmentrecord.h"
#include "prescriberrecord.h"
#include "pharmacistrecord.h"

namespace Ui {
    class APW_Page01;
}

class APW_Page01 : public QWizardPage
{
    Q_OBJECT

public:
    explicit APW_Page01(QWidget *parent = 0);
    ~APW_Page01();

	void setPatient(PatientRecord *);
	void setPrescription(PrescriptionRecord *);
	void setMedication(MedicationRecord *);
	void setShipment(ShipmentRecord *);

	bool initCustom();
	void getResults();

	void medUpdated();

private:
	void deleteLists();
	void setupComboBoxes();

    Ui::APW_Page01 *ui;

	PatientRecord *patient;
	PrescriptionRecord *prescription;
	MedicationRecord *medication;
	ShipmentRecord *shipment;

	QList<PrescriberRecord *> prescribers;
	QList<PharmacistRecord *> pharmacists;
};

#endif // APW_PAGE01_H
