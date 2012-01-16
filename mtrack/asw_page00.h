/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ASW_PAGE00_H
#define ASW_PAGE00_H

#include <QWizardPage>

#include "shipmentrecord.h"

namespace Ui {
    class ASW_Page00;
}

class ASW_Page00 : public QWizardPage
{
    Q_OBJECT

public:
    explicit ASW_Page00(QWidget *parent = 0);
    ~ASW_Page00();

	void getResults(ShipmentRecord *);

private:
    Ui::ASW_Page00 *ui;
};

#endif // ASW_PAGE00_H
