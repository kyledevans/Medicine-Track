/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef PRESCRIPTIONLABEL_H
#define PRESCRIPTIONLABEL_H

#include <QFrame>

namespace Ui {
    class PrescriptionLabel;
}

class PrescriptionLabel : public QFrame
{
    Q_OBJECT

public:
    explicit PrescriptionLabel(QWidget *parent = 0);
    ~PrescriptionLabel();

private:
    Ui::PrescriptionLabel *ui;
};

#endif // PRESCRIPTIONLABEL_H
