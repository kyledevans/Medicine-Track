/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PATIENTDISPLAY_H
#define PATIENTDISPLAY_H

#include <QDialog>

namespace Ui {
	class PatientDisplay;
}

class PatientDisplay : public QDialog
{
	Q_OBJECT
public:
	explicit PatientDisplay(int, QWidget *parent = 0);
	~PatientDisplay();

	void changeEvent(QEvent *);

private:
	Ui::PatientDisplay *ui;
};

#endif // PATIENTDISPLAY_H
