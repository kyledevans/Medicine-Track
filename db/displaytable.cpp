/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "displaytable.h"

DisplayTable::DisplayTable(QWidget *parent) :
	QTableWidget(parent)
{

}

DisplayTable::~DisplayTable()
{

}

void DisplayTable::clearVerticalHeaders()
{
	int j, rows;

	rows = this->rowCount();

	for (j = 0; j < rows; ++j) {
		delete this->takeVerticalHeaderItem(j);
	}
}
