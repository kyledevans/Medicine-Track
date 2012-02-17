/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

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

private:
    Ui::MainWindow *ui;
	QSqlDatabase db;

	bool importSettings();				// Returns true if it was able to import settings
};

#endif // MAINWINDOW_H
