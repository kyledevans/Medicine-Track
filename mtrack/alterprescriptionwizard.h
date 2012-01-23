/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALTERPRESCRIPTIONWIZARD_H
#define ALTERPRESCRIPTIONWIZARD_H

#include <QWizard>

namespace Ui {
    class AlterPrescriptionWizard;
}

class AlterPrescriptionWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AlterPrescriptionWizard(QWidget *parent = 0);
    ~AlterPrescriptionWizard();

private:
    Ui::AlterPrescriptionWizard *ui;
};

#endif // ALTERPRESCRIPTIONWIZARD_H
