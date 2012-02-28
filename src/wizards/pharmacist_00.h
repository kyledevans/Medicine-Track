/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "db/pharmacistrecord.h"

#ifndef PHARMACIST_00_H
#define PHARMACIST_00_H

#include <QWizardPage>

namespace Ui {
	class Pharmacist_00;
}

class Pharmacist_00 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Pharmacist_00(QWidget *parent = 0);
	~Pharmacist_00();

	void getResults(PharmacistRecord *);

private:
	Ui::Pharmacist_00 *ui;
};

#endif // PHARMACIST_00_H
