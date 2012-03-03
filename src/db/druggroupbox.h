/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "drugrecord.h"

#ifndef DRUGGROUPBOX_H
#define DRUGGROUPBOX_H

#include <QGroupBox>

namespace Ui {
class DrugGroupBox;
}

class DrugGroupBox : public QGroupBox
{
	Q_OBJECT
	
public:
	explicit DrugGroupBox(QWidget *parent = 0);
	~DrugGroupBox();

	void changeEvent(QEvent *);

	void setId(int);
	
private:
	Ui::DrugGroupBox *ui;

	DrugRecord drug;
};

#endif // DRUGGROUPBOX_H
