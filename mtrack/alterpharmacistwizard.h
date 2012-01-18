/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALTERPHARMACISTWIZARD_H
#define ALTERPHARMACISTWIZARD_H

#include <QWizard>

namespace Ui {
    class AlterPharmacistWizard;
}

class AlterPharmacistWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AlterPharmacistWizard(QWidget *parent = 0);
    ~AlterPharmacistWizard();

private:
    Ui::AlterPharmacistWizard *ui;
};

#endif // ALTERPHARMACISTWIZARD_H
