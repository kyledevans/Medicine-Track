﻿/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QList>

#include "db/pharmacistrecord.h"
#include "globals.h"

#ifndef PHARMACISTFRAME_H
#define PHARMACISTFRAME_H

#include <QFrame>

namespace Ui {
	class PharmacistFrame;
}

class PharmacistFrame : public QFrame
{
	Q_OBJECT

public:
	explicit PharmacistFrame(QWidget *parent = 0);
	~PharmacistFrame();

public slots:
	void initiateSearch(int pharmID = SQL::Undefined_ID);
	void initiateNew();
	void submitNew(PharmacistRecord *);		// Commits to the db and cleans up
	void newCleanup(PharmacistRecord *);	// Cleans up after the new/modify wizard
	void initiateModify();
	void toggleActive();

	void resetPressed();

	void selectionChanged();

private:
	Ui::PharmacistFrame *ui;

	QList<int> ids;		// Contains the pharmacist id's of the current query
	bool db_queried;	// True if there is data in the resultTable
};

#endif // PHARMACISTFRAME_H
