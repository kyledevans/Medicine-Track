/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef MEDICATIONSFRAME_H
#define MEDICATIONSFRAME_H

#include <QFrame>
#include <QList>

#include "medicationrecord.h"
#include "shipmentrecord.h"
#include "globals.h"

namespace Ui {
    class MedicationsFrame;
}

class MedicationsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MedicationsFrame(QWidget *parent = 0);
    ~MedicationsFrame();

public slots:
	void initiateSearch(int medID = SQL::Undefined_ID);

	void initiateModify();
	void submitModify(MedicationRecord *);

	void initiateNewMed();
	void submitNewMed(MedicationRecord *);
	void medCleanup(MedicationRecord *);

	void resetPressed();

	void initiateNewShipment();
	void submitNewShipment(ShipmentRecord *);
	void shipmentCleanup(ShipmentRecord *);

	void toggleActive();

	void selectionChanged();

private:
    Ui::MedicationsFrame *ui;
	QList<int> ids;					// Contains the drug id's of the current query
	bool db_queried;				// Indicates if the result table has a model
};

#endif // MEDICATIONSFRAME_H
