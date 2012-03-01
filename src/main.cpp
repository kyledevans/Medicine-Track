/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QtGui/QApplication>
#include <QTranslator>
#include "mainwindow.h"

#include <QSqlDatabase>

#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator trans;
	MainWindow w;

	if (trans.load("mtrack_en")) {
		qDebug() << "loaded translation";
	} else {
		qDebug() << "failed to load";
	}
	a.installTranslator(&trans);

	w.show();

	return a.exec();
}
