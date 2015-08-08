/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "bl.h"

#ifndef PRESCRIPTIONFRAME_H
#define PRESCRIPTIONFRAME_H

#include <QFrame>

namespace Ui {
	class PrescriptionFrame;
}

class PrescriptionFrame : public QFrame
{
	Q_OBJECT
public:
	explicit PrescriptionFrame(QWidget *parent = 0);
	~PrescriptionFrame();

	void changeEvent(QEvent *);

public slots:
	void initiateSearch();
	void initiatePrint();

	void invalidatePrescription();

	void viewPrescription();

	void resetPressed();
	void selectionChanged();

private:
	Ui::PrescriptionFrame *ui;
    BL bl;
};

#endif // PRESCRIPTIONFRAME_H
