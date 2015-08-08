#include <QDate>

#ifndef PRESCRIPTIONSEARCHITEM_H
#define PRESCRIPTIONSEARCHITEM_H

class PrescriptionSearchItem
{
public:
    PrescriptionSearchItem();
    ~PrescriptionSearchItem();

	int getAllscriptsId() const;
	void setAllscriptsId(int value);

	QString getFirstname() const;
	void setFirstname(const QString &value);

	QString getLastname() const;
	void setLastname(const QString &value);

	QDate getDob() const;
	void setDob(const QDate &value);

	QString getDrugName() const;
	void setDrugName(const QString &value);

	QString getDrugForm() const;
	void setDrugForm(const QString &value);

	QString getDrugStrength() const;
	void setDrugStrength(const QString &value);

	int getPrescriptionID() const;
	void setPrescriptionID(int value);

	QString getPrescriptionAmount() const;
	void setPrescriptionAmount(const QString &value);

	QDate getPrescriptionWritten() const;
	void setPrescriptionWritten(const QDate &value);

	QDate getPrescriptionFilled() const;
	void setPrescriptionFilled(const QDate &value);

	bool getPrescriptionActive() const;
	void setPrescriptionActive(bool value);

	QString getShipmentLot() const;
	void setShipmentLot(const QString &value);

private:
	int allscriptsId;
	QString firstname;
	QString lastname;
	QDate dob;

	QString drugName;
	QString drugForm;
	QString drugStrength;

	int prescriptionID;
	QString prescriptionAmount;
	QDate prescriptionWritten;
	QDate prescriptionFilled;
	bool prescriptionActive;

	QString shipmentLot;
};

#endif // PRESCRIPTIONSEARCHITEM_H
