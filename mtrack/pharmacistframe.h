/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PHARMACISTFRAME_H
#define PHARMACISTFRAME_H

#include <QFrame>
#include <QList>

#include "pharmacistrecord.h"
#include "globals.h"

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

private:
    Ui::PharmacistFrame *ui;

	QList<int> ids;		// Contains the pharmacist id's of the current query
	bool db_queried;	// True if there is data in the resultTable
};

#endif // PHARMACISTFRAME_H
