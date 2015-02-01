#include "bl.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "../db/globals.h"

BL::BL()
{

}

BL::~BL()
{

}

/* SQL without C++:
SELECT id, allscripts_id, last, first, dob, active
FROM patients
WHERE last LIKE ?
AND first LIKE ?
AND (<true if NOT searching by DOB> OR (dob = ?))
AND active = ?
*/
QList<PatientRecord*> * BL::searchPatients(QString _Firstname, QString _Lastname, QDate _DOB, bool _Active)
{
    QSqlQuery query;
    bool dont_search_dob = true;
    QList<PatientRecord*> *patients;
    PatientRecord *patient;

    query.prepare(
                "SELECT id, allscripts_id, last, first, dob, active "
                "FROM patients "
                "WHERE last LIKE ? "
                "AND first LIKE ? "
                "AND (? OR (dob = ?)) "
                "AND active = ?");
    query.bindValue(0, SQL::prepWildcards(_Lastname));
    query.bindValue(1, SQL::prepWildcards(_Firstname));
    if (_DOB != DEFAULTS::Date) {
        dont_search_dob = false;
    }
    query.bindValue(2, QVariant(dont_search_dob));
    query.bindValue(3, QVariant(_DOB));
    query.bindValue(4, QVariant(_Active));

    if (!execQuery(query))
        return nullptr;

    patients = new QList<PatientRecord*>();
    patients->reserve(query.size());

    for (int i = 0; i < query.size(); i++) {
        query.next();
        patient = new PatientRecord();
        patient->setId(query.value(0).toInt());
        patient->setAllscripts_id(query.value(1).toInt());
        patient->setLast(query.value(2).toString());
        patient->setFirst(query.value(3).toString());
        patient->setDob(query.value(4).toDate());
        patient->setActive(query.value(5).toBool());

        patients->append(patient);
    }

    return patients;
}

PatientRecord *BL::getPatient(int _PatientID)
{
    QSqlQuery query;
    PatientRecord *patient;

    query.prepare(
                "SELECT id, allscripts_id, last, first, dob, active "
                "FROM patients "
                "WHERE id = ?;");

    query.bindValue(0, QVariant(_PatientID));

    if (!execQuery(query))
        return nullptr;

    query.next();
    patient = new PatientRecord();
    patient->setId(query.value(0).toInt());
    patient->setAllscripts_id(query.value(1).toInt());
    patient->setLast(query.value(2).toString());
    patient->setFirst(query.value(3).toString());
    patient->setDob(query.value(4).toDate());
    patient->setActive(query.value(5).toBool());

    return patient;
}

bool BL::execQuery(QSqlQuery &_Query)
{
    _Query.exec();

    if (!_Query.lastError().isValid()) {
        return true;	// Everything went well
    }

    qDebug() << "DB ERROR:" << _Query.lastError().text();	// TODO: Remove this for production
    qDebug() << "query:" << _Query.lastQuery();				// TODO: Remove this as well

    return false;
}

