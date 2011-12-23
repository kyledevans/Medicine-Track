#include "inventoryframe.h"
#include "ui_inventoryframe.h"

InventoryFrame::InventoryFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InventoryFrame)
{
    ui->setupUi(this);
}

InventoryFrame::~InventoryFrame()
{
    delete ui;
}
