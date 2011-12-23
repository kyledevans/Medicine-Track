#ifndef MEDICATIONSFRAME_H
#define MEDICATIONSFRAME_H

#include <QFrame>

namespace Ui {
    class MedicationsFrame;
}

class MedicationsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MedicationsFrame(QWidget *parent = 0);
    ~MedicationsFrame();

private:
    Ui::MedicationsFrame *ui;
};

#endif // MEDICATIONSFRAME_H
