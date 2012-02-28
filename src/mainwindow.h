/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSqlDatabase>
#include <QProcess>

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void connectDB();
	void initiateOptions();
	void initiateHelp();

private:
	Ui::MainWindow *ui;

	QProcess *help_process;
	QSqlDatabase db;

	bool importSettings();				// Returns true if it was able to import settings
};

#endif // MAINWINDOW_H
