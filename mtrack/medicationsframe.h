/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef MEDICATIONSFRAME_H
#define MEDICATIONSFRAME_H

#include <QFrame>
#include <QList>

namespace Ui {
    class MedicationsFrame;
}

class MedicationsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MedicationsFrame(QWidget *parent = 0);
    ~MedicationsFrame();

public slots:
	void initiateSearch();
	void initiateModify();

private:
    Ui::MedicationsFrame *ui;
	QList<int> drugIds;				// Contains the drug id's of the current query
	bool db_queried;				// Indicates if the result table has a model
};

#endif // MEDICATIONSFRAME_H
