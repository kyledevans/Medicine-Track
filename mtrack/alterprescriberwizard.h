/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALTERPRESCRIBERWIZARD_H
#define ALTERPRESCRIBERWIZARD_H

#include <QWizard>

namespace Ui {
    class AlterPrescriberWizard;
}

class AlterPrescriberWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AlterPrescriberWizard(QWidget *parent = 0);
    ~AlterPrescriberWizard();

private:
    Ui::AlterPrescriberWizard *ui;
};

#endif // ALTERPRESCRIBERWIZARD_H
