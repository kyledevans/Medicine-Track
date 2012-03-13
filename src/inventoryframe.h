﻿/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipmentrecord.h"
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

	void changeEvent(QEvent *);

public slots:
	void initiateSearch(int shipID = SQL::Undefined_ID);
	void initiatePrintBarcode();

	void initiateWriteOff();
	void initiateIncrease();

	void toggleActive();

	void viewInventory();

	void resetPressed();
	void selectionChanged();

private:
	Ui::InventoryFrame *ui;

	void resizeHeaders();
};

#endif // INVENTORYFRAME_H
