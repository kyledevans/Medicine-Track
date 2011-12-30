/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "prescriptionsframe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.1.99");
	db.setDatabaseName("epc_simple");
	db.setUserName("kyle");
	db.setPassword("kobalt");
	db.open();

	ui->setupUi(this);

	PrescriptionsFrame *prescriptionsFrame = new PrescriptionsFrame();
	ui->mainTabs->addTab(prescriptionsFrame, QString("Prescriptions"));
}

MainWindow::~MainWindow()
{
    delete ui;
	db.close();
}
