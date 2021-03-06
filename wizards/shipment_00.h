﻿/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "../db/shipmentrecord.h"
#include "../db/drugrecord.h"

#ifndef SHIPMENT_00_H
#define SHIPMENT_00_H

#include <QWizardPage>

namespace Ui {
	class Shipment_00;
}

class Shipment_00 : public QWizardPage
{
	Q_OBJECT
public:
	explicit Shipment_00(QWidget *parent = 0);
	~Shipment_00();

	void changeEvent(QEvent *);

	void getResults(ShipmentRecord *);
	void setMedication(DrugRecord *);

private:
	Ui::Shipment_00 *ui;
};

#endif // SHIPMENT_00_H
