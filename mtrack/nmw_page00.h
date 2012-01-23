/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef NMW_PAGE00_H
#define NMW_PAGE00_H

#include <QWizardPage>

#include "medicationrecord.h"

namespace Ui {
    class NMW_Page00;
}

class NMW_Page00 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NMW_Page00(QWidget *parent = 0);
    ~NMW_Page00();

	void getResults(MedicationRecord *);
	void setFormAmount(int, QString amount = QString(""));	// Set form and amount

private slots:
	void amountFieldCheck(int index = -1);

private:
    Ui::NMW_Page00 *ui;

	bool formHasFixedAmount();		// Returns true if the currently selected value in formField should have an amount associated with it
};

#endif // NMW_PAGE00_H
