/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PHARMACISTWIZARD_H
#define PHARMACISTWIZARD_H

#include <QWizard>

#include "pharmacistrecord.h"

namespace Ui {
	class PharmacistWizard;
}

class PharmacistWizard : public QWizard
{
	Q_OBJECT

public:
	explicit PharmacistWizard(QWidget *parent = 0);	// TODO: Remove this if it's not intended to be used
	explicit PharmacistWizard(PharmacistRecord *, QWidget *parent = 0);
	~PharmacistWizard();

public slots:
	void returnResults();
	void rejectedWizard();

signals:
	void wizardComplete(PharmacistRecord *);
	void wizardRejected(PharmacistRecord *);

private:
	Ui::PharmacistWizard *ui;

	PharmacistRecord *pharm;
};

#endif // PHARMACISTWIZARD_H
