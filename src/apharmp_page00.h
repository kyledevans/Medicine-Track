/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef APHARMP_PAGE00_H
#define APHARMP_PAGE00_H

#include <QWizardPage>

#include "pharmacistrecord.h"

namespace Ui {
    class APharmP_Page00;
}

class APharmP_Page00 : public QWizardPage
{
    Q_OBJECT

public:
    explicit APharmP_Page00(QWidget *parent = 0);
    ~APharmP_Page00();

	void getResults(PharmacistRecord *);

private:
    Ui::APharmP_Page00 *ui;
};

#endif // APHARMP_PAGE00_H
