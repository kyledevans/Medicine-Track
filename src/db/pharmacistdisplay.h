/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

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

	void changeEvent(QEvent *);
	
private:
	Ui::PharmacistDisplay *ui;
};

#endif // PHARMACISTDISPLAY_H
