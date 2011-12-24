/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

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
