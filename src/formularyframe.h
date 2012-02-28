/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "db/drugrecord.h"
#include "db/shipmentrecord.h"
#include "globals.h"

#ifndef FORMULARYFRAME_H
#define FORMULARYFRAME_H

#include <QFrame>

namespace Ui {
    class FormularyFrame;
}

class FormularyFrame : public QFrame
{
	Q_OBJECT

public:
	explicit FormularyFrame(QWidget *parent = 0);
	~FormularyFrame();

public slots:
	void initiateSearch(int medID = SQL::Undefined_ID);

	void initiateModify();
	void initiateNewMed();
	void submitNewMed(DrugRecord *);
	void medCleanup(DrugRecord *);

	void resetPressed();

	void initiateNewShipment();
	void submitNewShipment(ShipmentRecord *);
	void shipmentCleanup(ShipmentRecord *);

	void toggleActive();

	void selectionChanged();

private:
	Ui::FormularyFrame *ui;

	QList<int> ids;					// Contains the drug id's of the current query
	bool db_queried;				// Indicates if the result table has a model
};

#endif // FORMULARYFRAME_H
