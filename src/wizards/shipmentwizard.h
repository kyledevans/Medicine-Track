/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "db/shipmentrecord.h"

#ifndef SHIPMENTWIZARD_H
#define SHIPMENTWIZARD_H

#include <QWizard>

namespace Ui {
	class ShipmentWizard;
}

class ShipmentWizard : public QWizard
{
	Q_OBJECT

public:
	explicit ShipmentWizard(ShipmentRecord *, QWidget *parent = 0);
	~ShipmentWizard();

	void changeEvent(QEvent *);

public slots:
	void returnResults();
	void rejectedWizard();

signals:
	void wizardComplete(ShipmentRecord *);
	void wizardRejected(ShipmentRecord *);

private:
	Ui::ShipmentWizard *ui;
	ShipmentRecord *shipment;
};

#endif // SHIPMENTWIZARD_H
