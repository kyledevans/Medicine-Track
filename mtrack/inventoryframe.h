/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef INVENTORYFRAME_H
#define INVENTORYFRAME_H

#include <QFrame>
#include <QList>

#include "shipmentrecord.h"

namespace Ui {
    class InventoryFrame;
}

class InventoryFrame : public QFrame
{
    Q_OBJECT

public:
    explicit InventoryFrame(QWidget *parent = 0);
    ~InventoryFrame();

public slots:
	void initiateSearch();
	void initiateModify();
	void submitModify(ShipmentRecord *);

private:
    Ui::InventoryFrame *ui;
	QList<int> drugIds;			// Contains the drug id's of the current search
	QList<int> shipmentIds;		// Contains the shipment id's of the current search
	bool db_queried;			// Indicates if the result table has a model
};

#endif // INVENTORYFRAME_H
