/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistrecord.h"

#ifndef PHARMACISTWIZARD_H
#define PHARMACISTWIZARD_H

#include <QWizard>

namespace Ui {
	class PharmacistWizard;
}

class PharmacistWizard : public QWizard
{
	Q_OBJECT
public:
	explicit PharmacistWizard(PharmacistRecord *, QWidget *parent = 0);
	~PharmacistWizard();

	void changeEvent(QEvent *);

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
