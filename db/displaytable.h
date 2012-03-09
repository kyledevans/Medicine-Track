/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef DISPLAYTABLE_H
#define DISPLAYTABLE_H

#include <QTableWidget>

class DisplayTable : public QTableWidget
{
	Q_OBJECT
public:
	explicit DisplayTable(QWidget *parent = 0);
	~DisplayTable();

	void clearVerticalHeaders();
};

#endif // DISPLAYTABLE_H
