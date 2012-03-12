/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QtGui/QApplication>
#include <QTranslator>
#include <QFileInfo>
#include "mainwindow.h"

#include <QSqlDatabase>

#include <QDebug>

#include <Windows.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTranslator trans;
	QFileInfo exe_location(argv[0]);
	MainWindow w;

	trans.load(exe_location.absolutePath() + "/mtrack.qm");
	a.installTranslator(&trans);
	qDebug() << "here";
	w.show();

	return a.exec();
}
