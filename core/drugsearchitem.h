#include <QString>

#ifndef DRUGSEARCHITEM_H
#define DRUGSEARCHITEM_H

class DrugSearchItem
{
public:
    DrugSearchItem();
    ~DrugSearchItem();

	int getDrugId() const;
	void setDrugId(int value);

	QString getDrugName() const;
	void setDrugName(const QString &value);

	QString getDrugNdc() const;
	void setDrugNdc(const QString &value);

	QString getDrugStrength() const;
	void setDrugStrength(const QString &value);

	QString getDrugUnitSize() const;
	void setDrugUnitSize(const QString &value);

	QString getInventoryRemainingLong() const;
	void setInventoryRemainingLong(const QString &value);

	bool getDrugActive() const;
	void setDrugActive(bool value);

	QString getDrugDispenseUnits() const;
	void setDrugDispenseUnits(const QString &value);

	int getInventoryRemaining() const;
	void setInventoryRemaining(int value);

	QString getDrugForm() const;
	void setDrugForm(const QString &value);

private:
	int drugId;
	QString drugName;
	QString drugNdc;
	QString drugForm;
	QString drugStrength;
	QString drugUnitSize;
	QString inventoryRemainingLong;
	bool drugActive;
	QString drugDispenseUnits;
	int inventoryRemaining;
};

#endif // DRUGSEARCHITEM_H
