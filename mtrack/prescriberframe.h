/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PRESCRIBERFRAME_H
#define PRESCRIBERFRAME_H

#include <QFrame>

#include "prescriberrecord.h"
#include "globals.h"

namespace Ui {
    class PrescriberFrame;
}

class PrescriberFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PrescriberFrame(QWidget *parent = 0);
    ~PrescriberFrame();

public slots:
	void initiateSearch(int presID = SQL::Undefined_ID);
	void initiateNew();
	void submitNew(PrescriberRecord *);
	void newCleanup(PrescriberRecord *);
	void initiateModify();

	void selectionChanged();

private:
    Ui::PrescriberFrame *ui;

	QList<int> ids;		// Contains the prescriber id's of the current query
	bool db_queried;	// True if there is data in the resultTable
};

#endif // PRESCRIBERFRAME_H
