#include <QString>
#include <QDate>
#include <QSqlQuery>

#ifndef BL_H
#define BL_H

#include "../db/patientrecord.h"
#include "../db/prescriptionrecord.h"
#include "prescriptionsearchitem.h"
#include "drugsearchitem.h"

class BL
{
public:
    BL();
    ~BL();

    // Patient queries
    QList<PatientRecord *> *searchPatients(QString _Firstname, QString _Lastname, QDate _DOB, bool _Active);
    PatientRecord * getPatient(int _PatientID);

    // Prescription queries
    QList<PrescriptionSearchItem *> * searchPrescriptions(QString _Medication, QString _Lot, QDate _FilledDate, QString _Firstname, QString _Lastname, QDate _DOB, bool _Active, bool _Invalid);

    // Drug queries
    DrugSearchItem * getDrugSearchItem(int _DrugId);
private:
    bool execQuery(QSqlQuery &_Query);
};

#endif // BL_H
