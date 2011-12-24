/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALTERMEDICATIONWIZARD_H
#define ALTERMEDICATIONWIZARD_H

#include <QWizard>

namespace Ui {
    class AlterMedicationWizard;
}

class AlterMedicationWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AlterMedicationWizard(QWidget *parent = 0);
    ~AlterMedicationWizard();

private:
    Ui::AlterMedicationWizard *ui;
};

#endif // ALTERMEDICATIONWIZARD_H
