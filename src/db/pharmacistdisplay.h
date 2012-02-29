/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistrecord.h"

#ifndef PHARMACISTDISPLAY_H
#define PHARMACISTDISPLAY_H

#include <QDialog>

namespace Ui {
class PharmacistDisplay;
}

class PharmacistDisplay : public QDialog
{
	Q_OBJECT
	
public:
	explicit PharmacistDisplay(int, QWidget *parent = 0);
	~PharmacistDisplay();
	
private:
	Ui::PharmacistDisplay *ui;

	PharmacistRecord pharmacist;
};

#endif // PHARMACISTDISPLAY_H
