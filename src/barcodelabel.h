/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QPrinter>
#include <QString>

#ifndef BARCODELABEL_H
#define BARCODELABEL_H

#include <QFrame>

namespace Ui {
	class BarcodeLabel;
}

class BarcodeLabel : public QFrame
{
	Q_OBJECT

public:
	explicit BarcodeLabel(QWidget *parent = 0);
	~BarcodeLabel();

	void setBarcode(QString);
	QString getBarcode();

	int toID();

	void print();

private slots:
	void printLabel(QPrinter*);

private:
	Ui::BarcodeLabel *ui;

	QString barcode;
	bool has_barcode;
};

#endif // BARCODELABEL_H
