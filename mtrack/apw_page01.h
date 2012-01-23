/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef APW_PAGE01_H
#define APW_PAGE01_H

#include <QWizardPage>

namespace Ui {
    class APW_Page01;
}

class APW_Page01 : public QWizardPage
{
    Q_OBJECT

public:
    explicit APW_Page01(QWidget *parent = 0);
    ~APW_Page01();

private:
    Ui::APW_Page01 *ui;
};

#endif // APW_PAGE01_H
