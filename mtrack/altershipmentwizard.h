/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALTERSHIPMENTWIZARD_H
#define ALTERSHIPMENTWIZARD_H

#include <QWizard>

#include "shipmentrecord.h"

namespace Ui {
    class AlterShipmentWizard;
}

class AlterShipmentWizard : public QWizard
{
    Q_OBJECT

public:
	explicit AlterShipmentWizard(ShipmentRecord *, QWidget *parent = 0);
    ~AlterShipmentWizard();

public slots:
	void returnResults();
	void rejectedWizard();

signals:
	void wizardComplete(ShipmentRecord *);
	void wizardRejected(ShipmentRecord *);

private:
    Ui::AlterShipmentWizard *ui;
	ShipmentRecord *shipment;
};

#endif // ALTERSHIPMENTWIZARD_H
