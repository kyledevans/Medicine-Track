/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

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

public slots:
	void initiateNew();

private:
    Ui::PrescriberFrame *ui;
};

#endif // PRESCRIBERFRAME_H
