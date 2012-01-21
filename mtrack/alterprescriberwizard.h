/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALTERPRESCRIBERWIZARD_H
#define ALTERPRESCRIBERWIZARD_H

#include <QWizard>

#include "prescriberrecord.h"

namespace Ui {
    class AlterPrescriberWizard;
}

class AlterPrescriberWizard : public QWizard
{
    Q_OBJECT

public:
    explicit AlterPrescriberWizard(QWidget *parent = 0);
	explicit AlterPrescriberWizard(PrescriberRecord *, QWidget *parent = 0);
    ~AlterPrescriberWizard();

public slots:
	void returnResults();

signals:
	void wizardComplete(PrescriberRecord *);

private:
    Ui::AlterPrescriberWizard *ui;

	PrescriberRecord *pres;
};

#endif // ALTERPRESCRIBERWIZARD_H
