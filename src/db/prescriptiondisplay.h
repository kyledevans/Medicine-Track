/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptionrecord.h"

#ifndef PRESCRIPTIONDISPLAY_H
#define PRESCRIPTIONDISPLAY_H

#include <QDialog>

namespace Ui {
class PrescriptionDisplay;
}

class PrescriptionDisplay : public QDialog
{
	Q_OBJECT
	
public:
	explicit PrescriptionDisplay(int, QWidget *parent = 0);
	~PrescriptionDisplay();
	
private:
	Ui::PrescriptionDisplay *ui;

	PrescriptionRecord prescription;
};

#endif // PRESCRIPTIONDISPLAY_H
