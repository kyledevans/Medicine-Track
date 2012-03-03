/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipmentrecord.h"

#ifndef SHIPMENTGROUPBOX_H
#define SHIPMENTGROUPBOX_H

#include <QGroupBox>

namespace Ui {
class ShipmentGroupBox;
}

class ShipmentGroupBox : public QGroupBox
{
	Q_OBJECT

public:
	explicit ShipmentGroupBox(QWidget *parent = 0);
	~ShipmentGroupBox();

	void changeEvent(QEvent *);

	void setId(int);
	int getId();
	int getDrug_id();

private:
	Ui::ShipmentGroupBox *ui;

	ShipmentRecord shipment;
};

#endif // SHIPMENTGROUPBOX_H
