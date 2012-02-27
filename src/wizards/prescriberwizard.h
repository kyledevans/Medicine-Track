/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PRESCRIBERWIZARD_H
#define PRESCRIBERWIZARD_H

#include <QWizard>

#include "prescriberrecord.h"

namespace Ui {
	class PrescriberWizard;
}

class PrescriberWizard : public QWizard
{
	Q_OBJECT

public:
	explicit PrescriberWizard(QWidget *parent = 0);	// TODO: Remove this if it's never intended to be used
	explicit PrescriberWizard(PrescriberRecord *, QWidget *parent = 0);
	~PrescriberWizard();

public slots:
	void returnResults();
	void rejectedWizard();

signals:
	void wizardComplete(PrescriberRecord *);
	void wizardRejected(PrescriberRecord *);

private:
	Ui::PrescriberWizard *ui;

	PrescriberRecord *pres;
};

#endif // PRESCRIBERWIZARD_H
