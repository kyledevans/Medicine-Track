/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef APW_PAGE00_H
#define APW_PAGE00_H

#include <QWizardPage>

namespace Ui {
    class APW_Page00;
}

class APW_Page00 : public QWizardPage
{
    Q_OBJECT

public:
    explicit APW_Page00(QWidget *parent = 0);
    ~APW_Page00();

private:
    Ui::APW_Page00 *ui;
};

#endif // APW_PAGE00_H
