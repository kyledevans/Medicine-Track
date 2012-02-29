/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberrecord.h"

#ifndef PRESCRIBERDISPLAY_H
#define PRESCRIBERDISPLAY_H

#include <QDialog>

namespace Ui {
class PrescriberDisplay;
}

class PrescriberDisplay : public QDialog
{
	Q_OBJECT
	
public:
	explicit PrescriberDisplay(int, QWidget *parent = 0);
	~PrescriberDisplay();
	
private:
	Ui::PrescriberDisplay *ui;

	PrescriberRecord prescriber;
};

#endif // PRESCRIBERDISPLAY_H
