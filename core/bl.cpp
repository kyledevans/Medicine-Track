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

/* SQL query without C++:
SELECT prescriptions.id, patients.allscripts_id, patients.last,
patients.first, patients.dob, drugs.name, drugs.form, drugs.strength,
CONCAT(prescriptions.amount, ' ', drugs.dispense_units),
prescriptions.written, prescriptions.filled, shipments.lot, prescriptions.active
FROM prescriptions
JOIN patients ON prescriptions.patient_id = patients.id
JOIN drugs ON prescriptions.drug_id = drugs.id
JOIN shipments ON prescriptions.shipment_id = shipments.id
WHERE drugs.name LIKE ?
AND shipments.lot LIKE ?
AND (<true if NOT searching by filled date> OR (prescriptions.filled = ?))
AND patients.last LIKE ?
AND patients.first LIKE ?
AND (<true if NOT searching by dob> OR (patients.dob = ?))
AND prescriptions.active = ?;
*/
QList<PrescriptionSearchItem *> *  BL::searchPrescriptions(QString _Medication, QString _Lot, QDate _FilledDate, QString _Firstname, QString _Lastname, QDate _DOB, bool _Active, bool _Invalid)
{
    QSqlQuery query;
    bool dont_search_filled = true;
    bool dont_search_dob = true;
    QList<PrescriptionSearchItem *> *searchItems;
    PrescriptionSearchItem *item;

    query.prepare(
                "SELECT prescriptions.id, patients.allscripts_id, patients.last, "
                "patients.first, patients.dob, drugs.name, drugs.form, drugs.strength, "
                "CONCAT(prescriptions.amount, ' ', drugs.dispense_units), "
                "prescriptions.written, prescriptions.filled, shipments.lot, prescriptions.active "
                "FROM prescriptions "
                "JOIN patients ON prescriptions.patient_id = patients.id \n"
                "JOIN drugs ON prescriptions.drug_id = drugs.id "
                "JOIN shipments ON prescriptions.shipment_id = shipments.id "
                "WHERE drugs.name LIKE ? "
                "AND shipments.lot LIKE ? "
                "AND (? OR (prescriptions.filled = ?)) "
                "AND patients.last LIKE ? "
                "AND patients.first LIKE ? "
                "AND (? OR (patients.dob = ?)) "
                "AND patients.active = ? "
                "AND prescriptions.active = ?;");

    query.bindValue(0, SQL::prepWildcards(_Medication));
    query.bindValue(1, SQL::prepWildcards(_Lot));
    if (_FilledDate != DEFAULTS::Date) {    // Enables searching by filled date if the user made a change
        dont_search_filled = false;
    }
    query.bindValue(2, QVariant(dont_search_filled));
    query.bindValue(3, QVariant(_FilledDate));
    query.bindValue(4, SQL::prepWildcards(_Lastname));
    query.bindValue(5, SQL::prepWildcards(_Firstname));
    if (_DOB != DEFAULTS::Date) {       // Enables searching by dob if the user made a change
        dont_search_dob = false;
    }
    query.bindValue(6, QVariant(dont_search_dob));
    query.bindValue(7, QVariant(_DOB));
    query.bindValue(8, QVariant(_Active));
    query.bindValue(9, QVariant(!_Invalid));

    if (!execQuery(query))
        return nullptr;

    searchItems = new QList<PrescriptionSearchItem *>();
    searchItems->reserve(query.size());

    for (int i = 0; i < query.size(); i++) {
        query.next();
        item = new PrescriptionSearchItem();
        item->setPrescriptionID(query.value(0).toInt());
        item->setAllscriptsId(query.value(1).toInt());
        item->setLastname(query.value(2).toString());
        item->setFirstname(query.value(3).toString());
        item->setDob(query.value(4).toDate());
        item->setDrugName(query.value(5).toString());
        item->setDrugForm(query.value(6).toString());
        item->setDrugStrength(query.value(7).toString());
        item->setPrescriptionAmount(query.value(8).toString());
        item->setPrescriptionWritten(query.value(9).toDate());
        item->setPrescriptionFilled(query.value(10).toDate());
        item->setShipmentLot(query.value(11).toString());
        item->setPrescriptionActive(query.value(12).toBool());

        searchItems->append(item);
    }

    return searchItems;
}

DrugSearchItem *BL::getDrugSearchItem(int _DrugId)
{
    QSqlQuery query;
    DrugSearchItem *item;

    query.prepare("SELECT drugs.id, drugs.name, drugs.ndc, drugs.form, drugs.strength, drugs.unit_size, "
                   "CONCAT(SUM( s_temp.product_left ), ' ', drugs.dispense_units), drugs.active, "
                   "drugs.dispense_units, SUM( s_temp.product_left ) "
                   "FROM drugs "
                   "LEFT JOIN ("
                   "	SELECT shipments.id, shipments.drug_id, shipments.product_left "
                   "	FROM shipments "
                   "	WHERE shipments.active = 1 "
                   "	AND shipments.expiration > CURDATE()) AS s_temp ON drugs.id = s_temp.drug_id "
                   "WHERE drugs.id = ? "
                   "GROUP BY drugs.id;");
    query.bindValue(0, _DrugId);

    if (!execQuery(query))
        return nullptr;

    query.next();
    item = new DrugSearchItem();

    item->setDrugId(query.value(0).toInt());
    item->setDrugName(query.value(1).toString());
    item->setDrugNdc(query.value(2).toString());
    item->setDrugForm(FORM_STR::intToStr(query.value(3).toInt()));
    item->setDrugStrength(query.value(4).toString());
    item->setDrugUnitSize(query.value(5).toString());
    item->setInventoryRemainingLong(query.value(6).toString());
    item->setDrugActive(query.value(7).toBool());
    item->setDrugDispenseUnits(query.value(8).toString());
    item->setInventoryRemaining(query.value(9).toInt());

    return item;
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

