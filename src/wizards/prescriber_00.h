﻿/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PRESCRIBER_00_H
#define PRESCRIBER_00_H

#include <QWizardPage>

#include "prescriberrecord.h"

namespace Ui {
	class Prescriber_00;
}

class Prescriber_00 : public QWizardPage
{
	Q_OBJECT

public:
	explicit Prescriber_00(QWidget *parent = 0);
	~Prescriber_00();

	void getResults(PrescriberRecord *);

private:
	Ui::Prescriber_00 *ui;
};

#endif // PRESCRIBER_00_H
