/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

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
	void initiateNew();

private:
    Ui::PharmacistFrame *ui;
};

#endif // PHARMACISTFRAME_H
