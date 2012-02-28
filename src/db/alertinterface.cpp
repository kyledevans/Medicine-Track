/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "alertinterface.h"

#include <QSqlError>
#include <QMessageBox>

#include <QDebug>

AlertInterface::AlertInterface(QWidget *parent) :
	QWidget(parent)
{
}

bool AlertInterface::attemptQuery(QSqlQueryModel *model, QString *query)
{
	/*// TODO: Comment out the next 2 lines for production builds
	qDebug() << *query;
	return true;*/

	model->setQuery(*query);
	if (!model->lastError().isValid()) {
		return true;	// Everything went well
	}
	qDebug() << "DB ERROR:" << model->lastError().text();	// TODO: Remove this for production
	qDebug() << "query:" << *query;	// TODO: Remove this as well

	databaseInaccessible();
	return false;
}

bool AlertInterface::attemptQuery(QSqlQuery *model, QString *query)
{
	/*// TODO: Comment out the next 2 lines for production builds
	qDebug() << *query;
	return true;*/

	//model->setQuery(*query);
	model->exec(*query);
	if (!model->lastError().isValid()) {
		return true;	// Everything went well
	}
	qDebug() << "DB ERROR:" << model->lastError().text();	// TODO: Remove this for production
	qDebug() << "query:" << *query;	// TODO: Remove this as well

	databaseInaccessible();
	return false;
}

bool AlertInterface::attemptQuery(QSqlQuery *model)
{
	/*// TODO: Comment out the next 2 lines for production builds
	qDebug() << *query;
	return true;*/

	//model->setQuery(*query);
	model->exec();
	if (!model->lastError().isValid()) {
		return true;	// Everything went well
	}
	qDebug() << "DB ERROR:" << model->lastError().text();	// TODO: Remove this for production
	qDebug() << "query:" << model->lastQuery();				// TODO: Remove this as well

	databaseInaccessible();
	return false;
}

void AlertInterface::databaseInaccessible()
{
	QMessageBox::critical(this, QString("Database Error"), QString("Go to Application -> Reconnect.\n\nIf you continue to receieve this error make sure that the database is accesible from this computer."), QMessageBox::Ok, QMessageBox::Ok);
}
