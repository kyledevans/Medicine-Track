/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef APW_PAGE00_H
#define APW_PAGE00_H

#include <QWizardPage>

#include <QList>

#include "db/patientrecord.h"
#include "medicationrecord.h"
#include "db/prescriptionrecord.h"
#include "shipmentrecord.h"

namespace Ui {
    class APW_Page00;
}

class APW_Page00 : public QWizardPage
{
    Q_OBJECT

public:
    explicit APW_Page00(QWidget *parent = 0);
    ~APW_Page00();

	void setPatient(PatientRecord *);
	void setPrescription(PrescriptionRecord *);

	bool isComplete() const;

public slots:
	void initiateSearch();
	void resultSelected();

signals:
	void medicationChanged(int);

private:
    Ui::APW_Page00 *ui;

	PatientRecord *patient;
	PrescriptionRecord *prescription;

	bool db_queried;

	QList<int> ids;				// Contains the shipment id's of the current search
};

#endif // APW_PAGE00_H
