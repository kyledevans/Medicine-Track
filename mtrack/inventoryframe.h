/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef INVENTORYFRAME_H
#define INVENTORYFRAME_H

#include <QFrame>
#include <QList>

#include "shipmentrecord.h"
#include "globals.h"

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
	void initiateSearch(int shipID = SQL::Undefined_ID);
	void initiateModify();
	void submitModify(ShipmentRecord *);
	void shipmentCleanup(ShipmentRecord *);
	void initiateWriteOff();

private:
    Ui::InventoryFrame *ui;

	QList<int> ids;				// Contains the shipment id's of the current search
	bool db_queried;			// Indicates if the result table has a model
};

#endif // INVENTORYFRAME_H
