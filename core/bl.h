#include <QString>
#include <QDate>
#include <QSqlQuery>

#ifndef BL_H
#define BL_H

#include "../db/patientrecord.h"

class BL
{
public:
    BL();
    ~BL();

    // Patient queries
    QList<PatientRecord *> *searchPatients(QString _Firstname, QString _Lastname, QDate _DOB, bool _Active);

private:
    bool execQuery(QSqlQuery &_Query);
};

#endif // BL_H
