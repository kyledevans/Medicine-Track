/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "../db/prescriberrecord.h"
#include "../db/globals.h"

#ifndef PRESCRIBERFRAME_H
#define PRESCRIBERFRAME_H

#include <QFrame>

namespace Ui {
	class PrescriberFrame;
}

class PrescriberFrame : public QFrame
{
	Q_OBJECT

public:
	explicit PrescriberFrame(QWidget *parent = 0);
	~PrescriberFrame();

	void changeEvent(QEvent *);

public slots:
	void initiateSearch(int presID = SQL::Undefined_ID);

	void initiateNew();
	void initiateModify();

	void submitPrescriber(PrescriberRecord *);
	void prescriberCleanup(PrescriberRecord *);

	void toggleActive();

	void viewPrescriber();

	void resetPressed();
	void selectionChanged();

private:
	Ui::PrescriberFrame *ui;
};

#endif // PRESCRIBERFRAME_H
