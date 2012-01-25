/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef APW_PAGE02_H
#define APW_PAGE02_H

#include <QWizardPage>

#include "prescriptionrecord.h"

namespace Ui {
    class APW_Page02;
}

class APW_Page02 : public QWizardPage
{
    Q_OBJECT

public:
    explicit APW_Page02(QWidget *parent = 0);
    ~APW_Page02();

	void setPrescription(PrescriptionRecord *);

	bool initCustom();

private:
    Ui::APW_Page02 *ui;

	PrescriptionRecord *prescription;
};

#endif // APW_PAGE02_H
