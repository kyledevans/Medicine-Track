/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef MTCONFIG_H
#define MTCONFIG_H

#include <QDialog>

namespace Ui {
class MTConfig;
}

class MTConfig : public QDialog
{
    Q_OBJECT
    
public:
    explicit MTConfig(QWidget *parent = 0);
    ~MTConfig();

public slots:
	void commit();
    
private:
    Ui::MTConfig *ui;
};

#endif // MTCONFIG_H
