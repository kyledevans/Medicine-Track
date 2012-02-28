/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlQueryModel>
#include <QSqlQuery>

#ifndef ALERTINTERFACE_H
#define ALERTINTERFACE_H

#include <QWidget>

class AlertInterface : public QWidget
{
	Q_OBJECT
public:
	explicit AlertInterface(QWidget *parent = 0);

	bool attemptQuery(QSqlQuery *);		// Returns true if succesful
	void databaseInaccessible();		// Pops up an error to the user

signals:

public slots:

};

#endif // ALERTINTERFACE_H
