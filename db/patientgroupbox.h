/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientrecord.h"

#ifndef PATIENTGROUPBOX_H
#define PATIENTGROUPBOX_H

#include <QGroupBox>

namespace Ui {
	class PatientGroupBox;
}

class PatientGroupBox : public QGroupBox
{
	Q_OBJECT
public:
	explicit PatientGroupBox(QWidget *parent = 0);
	~PatientGroupBox();

	void changeEvent(QEvent *);
	void setStrings();

	void setId(int);

private:
	Ui::PatientGroupBox *ui;

	PatientRecord patient;
};

#endif // PATIENTGROUPBOX_H
