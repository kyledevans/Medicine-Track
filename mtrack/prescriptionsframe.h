/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PRESCRIPTIONSFRAME_H
#define PRESCRIPTIONSFRAME_H

#include <QFrame>

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
};

#endif // PRESCRIPTIONSFRAME_H
