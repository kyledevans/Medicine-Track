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

	void postSetup();
	void setItemFlag(int, int, QTableWidgetItem *, bool);	// Red flag on false

	// Reimplemented methods
	void setItem(int, int, QTableWidgetItem *);
	QTableWidgetItem * item(int row, int column) const;
	void sortByColumn(int column, Qt::SortOrder order);
	void hideColumn(int column);

public slots:
	void clearContents();

private:
	void setRowProblem(int);	// (row)
	void clearFlags();

private:
	QPixmap *flag_red_pix;
};

#endif // DISPLAYTABLE_H
