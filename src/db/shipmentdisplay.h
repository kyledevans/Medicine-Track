/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef SHIPMENTDISPLAY_H
#define SHIPMENTDISPLAY_H

#include <QDialog>

namespace Ui {
class ShipmentDisplay;
}

class ShipmentDisplay : public QDialog
{
	Q_OBJECT
	
public:
	explicit ShipmentDisplay(int, QWidget *parent = 0);
	~ShipmentDisplay();
	
private:
	Ui::ShipmentDisplay *ui;
};

#endif // SHIPMENTDISPLAY_H
