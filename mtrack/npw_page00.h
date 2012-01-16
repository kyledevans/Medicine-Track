/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef NPW_PAGE00_H
#define NPW_PAGE00_H

#include <QWizardPage>
#include "patientrecord.h"

namespace Ui {
    class NPW_Page00;
}

class NPW_Page00 : public QWizardPage
{
    Q_OBJECT

public:
    explicit NPW_Page00(QWidget *parent = 0);
    ~NPW_Page00();

	void getResults(PatientRecord *);

private:
    Ui::NPW_Page00 *ui;
};

#endif // NPW_PAGE00_H
