/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef APRESCRIBERPAGE_PAGE00_H
#define APRESCRIBERPAGE_PAGE00_H

#include <QWizardPage>

namespace Ui {
    class APrescriberPage_Page00;
}

class APrescriberPage_Page00 : public QWizardPage
{
    Q_OBJECT

public:
    explicit APrescriberPage_Page00(QWidget *parent = 0);
    ~APrescriberPage_Page00();

private:
    Ui::APrescriberPage_Page00 *ui;
};

#endif // APRESCRIBERPAGE_PAGE00_H
