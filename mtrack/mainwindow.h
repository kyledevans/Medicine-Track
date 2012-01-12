/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSettings>

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

private:
    Ui::MainWindow *ui;
	QSqlDatabase db;

	bool importSettings();				// Returns true if it was able to import settings
	bool writeDefaults(QSettings *);	// Returns true if the write was succesful
};

#endif // MAINWINDOW_H
