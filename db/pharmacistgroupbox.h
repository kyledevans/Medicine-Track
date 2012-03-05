/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistrecord.h"

#ifndef PHARMACISTGROUPBOX_H
#define PHARMACISTGROUPBOX_H

#include <QGroupBox>

namespace Ui {
	class PharmacistGroupBox;
}

class PharmacistGroupBox : public QGroupBox
{
	Q_OBJECT
public:
	explicit PharmacistGroupBox(QWidget *parent = 0);
	~PharmacistGroupBox();

	void changeEvent(QEvent *);
	void setStrings();

	void setId(int);

private:
	Ui::PharmacistGroupBox *ui;

	PharmacistRecord pharmacist;
};

#endif // PHARMACISTGROUPBOX_H
