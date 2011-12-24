/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

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

private:
    Ui::PrescriptionsFrame *ui;
	QList<int> pres_ids;		// Contains prescription id's
	QList<int> patient_ids;		// Contains corresponding patient id's
	QList<int> drug_ids;		// Contains corresponding drug id's
	QList<int> shipment_ids;	// Contains corresponding shipment id's
	bool db_queried;			// Indicates if the result table has a model
};

#endif // PRESCRIPTIONSFRAME_H
