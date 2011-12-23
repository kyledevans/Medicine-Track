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

private:
    Ui::PrescriptionsFrame *ui;
};

#endif // PRESCRIPTIONSFRAME_H
