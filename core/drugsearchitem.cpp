#include "drugsearchitem.h"

DrugSearchItem::DrugSearchItem()
{

}

DrugSearchItem::~DrugSearchItem()
{

}
int DrugSearchItem::getDrugId() const
{
	return drugId;
}

void DrugSearchItem::setDrugId(int value)
{
	drugId = value;
}
QString DrugSearchItem::getDrugName() const
{
	return drugName;
}

void DrugSearchItem::setDrugName(const QString &value)
{
	drugName = value;
}
QString DrugSearchItem::getDrugNdc() const
{
	return drugNdc;
}

void DrugSearchItem::setDrugNdc(const QString &value)
{
	drugNdc = value;
}
QString DrugSearchItem::getDrugStrength() const
{
	return drugStrength;
}

void DrugSearchItem::setDrugStrength(const QString &value)
{
	drugStrength = value;
}
QString DrugSearchItem::getDrugUnitSize() const
{
	return drugUnitSize;
}

void DrugSearchItem::setDrugUnitSize(const QString &value)
{
	drugUnitSize = value;
}
QString DrugSearchItem::getInventoryRemainingLong() const
{
	return inventoryRemainingLong;
}

void DrugSearchItem::setInventoryRemainingLong(const QString &value)
{
	inventoryRemainingLong = value;
}
bool DrugSearchItem::getDrugActive() const
{
	return drugActive;
}

void DrugSearchItem::setDrugActive(bool value)
{
	drugActive = value;
}
QString DrugSearchItem::getDrugDispenseUnits() const
{
	return drugDispenseUnits;
}

void DrugSearchItem::setDrugDispenseUnits(const QString &value)
{
	drugDispenseUnits = value;
}
int DrugSearchItem::getInventoryRemaining() const
{
	return inventoryRemaining;
}

void DrugSearchItem::setInventoryRemaining(int value)
{
	inventoryRemaining = value;
}
QString DrugSearchItem::getDrugForm() const
{
	return drugForm;
}

void DrugSearchItem::setDrugForm(const QString &value)
{
	drugForm = value;
}











