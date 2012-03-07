/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistrecord.h"
#include "globals.h"

#ifndef PHARMACISTFRAME_H
#define PHARMACISTFRAME_H

#include <QFrame>

namespace Ui {
	class PharmacistFrame;
}

class PharmacistFrame : public QFrame
{
	Q_OBJECT

public:
	explicit PharmacistFrame(QWidget *parent = 0);
	~PharmacistFrame();

	void changeEvent(QEvent *);

public slots:
	void initiateSearch(int pharmID = SQL::Undefined_ID);

	void initiateNew();
	void initiateModify();

	void submitPharmacist(PharmacistRecord *);
	void pharmacistCleanup(PharmacistRecord *);

	void viewPharmacist();

	void toggleActive();

	void resetPressed();
	void selectionChanged();

private:
	Ui::PharmacistFrame *ui;
};

#endif // PHARMACISTFRAME_H
