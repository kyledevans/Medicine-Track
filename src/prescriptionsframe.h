/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PRESCRIPTIONSFRAME_H
#define PRESCRIPTIONSFRAME_H

#include <QFrame>
#include <QList>

namespace Ui {
	class PrescriptionsFrame;
}

class PrescriptionsFrame : public QFrame
{
	Q_OBJECT

public:
	explicit PrescriptionsFrame(QWidget *parent = 0);
	~PrescriptionsFrame();

public slots:
	void initiateSearch();
	void resetPressed();
	void initiateModify();
	void initiatePrint();

	void selectionChanged();

	void invalidatePrescription();

private:
	Ui::PrescriptionsFrame *ui;

	QList<int> ids;				// Contains prescription id's
	bool db_queried;			// Indicates if the result table has a model
};

#endif // PRESCRIPTIONSFRAME_H
