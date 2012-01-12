/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef NMW_PAGE01_H
#define NMW_PAGE01_H

#include <QWizardPage>

namespace Ui {
    class NMW_Page01;
}

class NMW_Page01 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NMW_Page01(QWidget *parent = 0);
    ~NMW_Page01();

private:
    Ui::NMW_Page01 *ui;
};

#endif // NMW_PAGE01_H
