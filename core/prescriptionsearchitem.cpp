#include "prescriptionsearchitem.h"

PrescriptionSearchItem::PrescriptionSearchItem()
{

}

PrescriptionSearchItem::~PrescriptionSearchItem()
{

}
int PrescriptionSearchItem::getAllscriptsId() const
{
	return allscriptsId;
}

void PrescriptionSearchItem::setAllscriptsId(int value)
{
	allscriptsId = value;
}
QString PrescriptionSearchItem::getFirstname() const
{
	return firstname;
}

void PrescriptionSearchItem::setFirstname(const QString &value)
{
	firstname = value;
}
QString PrescriptionSearchItem::getLastname() const
{
	return lastname;
}

void PrescriptionSearchItem::setLastname(const QString &value)
{
	lastname = value;
}
QDate PrescriptionSearchItem::getDob() const
{
	return dob;
}

void PrescriptionSearchItem::setDob(const QDate &value)
{
	dob = value;
}
QString PrescriptionSearchItem::getDrugName() const
{
	return drugName;
}

void PrescriptionSearchItem::setDrugName(const QString &value)
{
	drugName = value;
}
QString PrescriptionSearchItem::getDrugForm() const
{
	return drugForm;
}

void PrescriptionSearchItem::setDrugForm(const QString &value)
{
	drugForm = value;
}
QString PrescriptionSearchItem::getDrugStrength() const
{
	return drugStrength;
}

void PrescriptionSearchItem::setDrugStrength(const QString &value)
{
	drugStrength = value;
}
int PrescriptionSearchItem::getPrescriptionID() const
{
	return prescriptionID;
}

void PrescriptionSearchItem::setPrescriptionID(int value)
{
	prescriptionID = value;
}
QString PrescriptionSearchItem::getPrescriptionAmount() const
{
	return prescriptionAmount;
}

void PrescriptionSearchItem::setPrescriptionAmount(const QString &value)
{
	prescriptionAmount = value;
}
QDate PrescriptionSearchItem::getPrescriptionWritten() const
{
	return prescriptionWritten;
}

void PrescriptionSearchItem::setPrescriptionWritten(const QDate &value)
{
	prescriptionWritten = value;
}
QDate PrescriptionSearchItem::getPrescriptionFilled() const
{
	return prescriptionFilled;
}

void PrescriptionSearchItem::setPrescriptionFilled(const QDate &value)
{
	prescriptionFilled = value;
}
bool PrescriptionSearchItem::getPrescriptionActive() const
{
	return prescriptionActive;
}

void PrescriptionSearchItem::setPrescriptionActive(bool value)
{
	prescriptionActive = value;
}
QString PrescriptionSearchItem::getShipmentLot() const
{
	return shipmentLot;
}

void PrescriptionSearchItem::setShipmentLot(const QString &value)
{
	shipmentLot = value;
}














