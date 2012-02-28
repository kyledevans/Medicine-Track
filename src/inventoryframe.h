/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "db/shipmentrecord.h"
#include "globals.h"

#ifndef INVENTORYFRAME_H
#define INVENTORYFRAME_H

#include <QFrame>

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

	void initiateWriteOff();
	void initiatePrintBarcode();
	void initiateIncrease();

	void toggleActive();

	void resetPressed();
	void selectionChanged();

private:
	Ui::InventoryFrame *ui;

	QList<int> ids;				// Contains the shipment id's of the current search
	bool db_queried;			// Indicates if the result table has a model
};

#endif // INVENTORYFRAME_H
