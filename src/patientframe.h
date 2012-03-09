/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QBrush>
#include <QPixmap>

#include "patientrecord.h"
#include "prescriptionrecord.h"
#include "globals.h"

#ifndef PATIENTFRAME_H
#define PATIENTFRAME_H

#include <QFrame>

namespace Ui {
    class PatientFrame;
}

class PatientFrame : public QFrame
{
	Q_OBJECT
public:
    explicit PatientFrame(QWidget *parent = 0);
    ~PatientFrame();

	void changeEvent(QEvent *);

public slots:
	void initiateSearch(int patientID = SQL::Undefined_ID);

	void initiatePrescription();
	void initiateModification();
	void initiateNewPatient();

	void submitPatient(PatientRecord *);
	void patientCleanup(PatientRecord *);

	void submitPrescription(PrescriptionRecord *);
	void prescriptionCleanup(PrescriptionRecord *);

	void viewPatient();

	void resetPressed();
	void selectionChanged();

	void toggleActive();

private:
	void initiatePrint(PrescriptionRecord *);

	Ui::PatientFrame *ui;
};

#endif // PATIENTFRAME_H
