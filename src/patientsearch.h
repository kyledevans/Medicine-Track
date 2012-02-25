/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PATIENTSEARCH_H
#define PATIENTSEARCH_H

#include <QFrame>
#include <QList>

#include "db/patientrecord.h"
#include "db/prescriptionrecord.h"
#include "globals.h"

namespace Ui {
    class PatientSearch;
}

class PatientSearch : public QFrame
{
    Q_OBJECT

public:
    explicit PatientSearch(QWidget *parent = 0);
    ~PatientSearch();

public slots:
	void initiateSearch(int patientID = SQL::Undefined_ID);
	void resetPressed();
	void initiatePrescription();
	void initiateModification();
	void initiateNewPatient();
	void submitNewPatient(PatientRecord *);
	void submitModify(PatientRecord *);
	void newPatientCleanup(PatientRecord *);

	void submitNewPrescription(PrescriptionRecord *);
	void newPrescriptionCleanup(PrescriptionRecord *);

	void selectionChanged();

	void toggleActive();

private:
	void initiatePrint(PrescriptionRecord *);

    Ui::PatientSearch *ui;

	QList<int> ids;		// Contains the patient id's of the current query
	bool db_queried;	// Indicates if the result table has a model
};

#endif // PATIENTSEARCH_H
