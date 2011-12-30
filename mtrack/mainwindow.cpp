/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patientsearch.h"
#include "prescriptionsframe.h"
#include "medicationsframe.h"
#include "inventoryframe.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("192.168.1.99");
	db.setDatabaseName("epc");
	db.setUserName("kyle");
	db.setPassword("kobalt");
	db.open();

	ui->setupUi(this);

	PatientSearch *patientFrame = new PatientSearch();
	PrescriptionsFrame *prescriptionsFrame = new PrescriptionsFrame();
	MedicationsFrame *medicationsFrame = new MedicationsFrame();
	InventoryFrame *inventoryFrame = new InventoryFrame();

	ui->mainTabs->addTab(patientFrame, QString("Patients"));
	ui->mainTabs->addTab(prescriptionsFrame, QString("Prescriptions"));
	ui->mainTabs->addTab(medicationsFrame, QString("Medications"));
	ui->mainTabs->addTab(inventoryFrame, QString("Inventory"));
}

MainWindow::~MainWindow()
{
    delete ui;
	db.close();
}
