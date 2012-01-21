/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALTERPHARMACISTWIZARD_H
#define ALTERPHARMACISTWIZARD_H

#include <QWizard>

#include "pharmacistrecord.h"

namespace Ui {
    class AlterPharmacistWizard;
}

class AlterPharmacistWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AlterPharmacistWizard(QWidget *parent = 0);
	explicit AlterPharmacistWizard(PharmacistRecord *, QWidget *parent = 0);
    ~AlterPharmacistWizard();

public slots:
	void returnResults();

signals:
	void wizardComplete(PharmacistRecord *);

private:
    Ui::AlterPharmacistWizard *ui;

	PharmacistRecord *pharm;
};

#endif // ALTERPHARMACISTWIZARD_H
