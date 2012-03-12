/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "displaytable.h"

#include <QHeaderView>
#include <QCoreApplication>

#include "globals.h"

#include <QDebug>

DisplayTable::DisplayTable(QWidget *parent) :
	QTableWidget(parent)
{
	// Set up the flag items
	flag_red_pix = new QPixmap(QCoreApplication::applicationDirPath() + DEFAULTS::ImageDir + "flag-red.png", "PNG");
}

void DisplayTable::postSetup()
{
	this->setColumnWidth(0, flag_red_pix->width());	// Set the flag column to a certain size
}

DisplayTable::~DisplayTable()
{

}

// TODO: Shouldn't need this anymore
void DisplayTable::clearFlags()
{
	int i;

	for (i = 0; i < this->rowCount(); i++) {
		delete QTableWidget::takeItem(i, 0);
	}
}

void DisplayTable::setItemFlag(int row, int column, QTableWidgetItem *item, bool normal)
{
	if (!normal) {
		item->setBackgroundColor(MTCOLORS::Problem_BG);
	}
	this->setItem(row, column, item);

	if (!normal) {
		setRowProblem(row);
	}
}

void DisplayTable::setRowProblem(int row)
{
	QTableWidgetItem *flagItem;

	if (QTableWidget::item(row, 0) == 0) {	// Row is not yet a problem
		flagItem = new QTableWidgetItem(QIcon(*flag_red_pix), " ");
		QTableWidget::setItem(row, 0, flagItem);
	}
}

void DisplayTable::setItem(int row, int column, QTableWidgetItem *item)
{
	QTableWidget::setItem(row, column + 1, item);	// The first column is saved for flags
}

QTableWidgetItem * DisplayTable::item(int row, int column) const
{
	return QTableWidget::item(row, column + 1);		// Correct for column offset
}

void DisplayTable::sortByColumn(int column, Qt::SortOrder order)
{
	QTableWidget::sortByColumn(column + 1, order);
}

void DisplayTable::hideColumn(int column)
{
	QTableWidget::hideColumn(column + 1);
}

void DisplayTable::clearContents()
{
	clearFlags();
	QTableWidget::clearContents();
}
