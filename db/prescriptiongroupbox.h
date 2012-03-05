/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptionrecord.h"

#ifndef PRESCRIPTIONGROUPBOX_H
#define PRESCRIPTIONGROUPBOX_H

#include <QGroupBox>

namespace Ui {
	class PrescriptionGroupBox;
}

class PrescriptionGroupBox : public QGroupBox
{
	Q_OBJECT
public:
	explicit PrescriptionGroupBox(QWidget *parent = 0);
	~PrescriptionGroupBox();

	void changeEvent(QEvent *);
	void setStrings();

	void setId(int);
	int getId();
	int getPatient_id();
	int getDrug_id();
	int getShipment_id();
	int getPrescriber_id();
	int getPharmacist_id();

private:
	Ui::PrescriptionGroupBox *ui;

	PrescriptionRecord prescription;
};

#endif // PRESCRIPTIONGROUPBOX_H
