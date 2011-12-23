#ifndef INVENTORYFRAME_H
#define INVENTORYFRAME_H

#include <QFrame>

namespace Ui {
    class InventoryFrame;
}

class InventoryFrame : public QFrame
{
    Q_OBJECT

public:
    explicit InventoryFrame(QWidget *parent = 0);
    ~InventoryFrame();

private:
    Ui::InventoryFrame *ui;
};

#endif // INVENTORYFRAME_H
