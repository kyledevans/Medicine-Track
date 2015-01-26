/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberrecord.h"

#ifndef PRESCRIBERGROUPBOX_H
#define PRESCRIBERGROUPBOX_H

#include <QGroupBox>

namespace Ui {
	class PrescriberGroupBox;
}

class PrescriberGroupBox : public QGroupBox
{
	Q_OBJECT
public:
	explicit PrescriberGroupBox(QWidget *parent = 0);
	~PrescriberGroupBox();

	void changeEvent(QEvent *);

	void setId(int);

private:
	Ui::PrescriberGroupBox *ui;

	PrescriberRecord prescriber;
};

#endif // PRESCRIBERGROUPBOX_H
