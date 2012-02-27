/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef ALERTINTERFACE_H
#define ALERTINTERFACE_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>

class AlertInterface : public QWidget
{
	Q_OBJECT
public:
	explicit AlertInterface(QWidget *parent = 0);

	bool attemptQuery(QSqlQueryModel *, QString *);	// Returns true if succesful
	bool attemptQuery(QSqlQuery *, QString *);      // Returns true if succesful
	bool attemptQuery(QSqlQuery *);					// Returns true if succesful
	void databaseInaccessible();

	void criticalError(QString);

signals:

public slots:

};

#endif // ALERTINTERFACE_H
