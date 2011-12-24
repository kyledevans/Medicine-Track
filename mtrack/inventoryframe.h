/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

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

public slots:
	void initiateSearch();

private:
    Ui::InventoryFrame *ui;
};

#endif // INVENTORYFRAME_H
