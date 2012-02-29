/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "drugrecord.h"

#ifndef DRUGDISPLAY_H
#define DRUGDISPLAY_H

#include <QDialog>

namespace Ui {
class DrugDisplay;
}

class DrugDisplay : public QDialog
{
	Q_OBJECT
	
public:
	explicit DrugDisplay(int, QWidget *parent = 0);
	~DrugDisplay();
	
private:
	Ui::DrugDisplay *ui;

	DrugRecord drug;
};

#endif // DRUGDISPLAY_H
