/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "mtsettings.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patientsearch.h"
#include "prescriptionsframe.h"
#include "medicationsframe.h"
#include "inventoryframe.h"
#include "globals.h"
#include "alertinterface.h"
#include "prescriberframe.h"
#include "pharmacistframe.h"
#include "mtconfig.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	db = QSqlDatabase::addDatabase("QMYSQL");

	QCoreApplication::setOrganizationName(DEFAULTS::Organization);
	QCoreApplication::setOrganizationDomain(DEFAULTS::Domain);
	QCoreApplication::setApplicationName(DEFAULTS::Application);
	QCoreApplication::setApplicationVersion(QString().setNum(DEFAULTS::AppVersion));

	if (importSettings()) {
		connectDB();
	}

	PatientSearch *patientFrame = new PatientSearch();
	PrescriptionsFrame *prescriptionsFrame = new PrescriptionsFrame();
	MedicationsFrame *medicationsFrame = new MedicationsFrame();
	InventoryFrame *inventoryFrame = new InventoryFrame();
	PrescriberFrame *prescriberFrame = new PrescriberFrame();
	PharmacistFrame *pharmacistFrame = new PharmacistFrame();

	ui->mainTabs->addTab(patientFrame, QString("Patients"));
	ui->mainTabs->addTab(prescriptionsFrame, QString("Prescription log"));
	ui->mainTabs->addTab(medicationsFrame, QString("Medications"));
	ui->mainTabs->addTab(prescriberFrame, QString("Prescribers"));
	ui->mainTabs->addTab(pharmacistFrame, QString("Pharmacists"));
	ui->mainTabs->addTab(inventoryFrame, QString("Inventory"));

	connect(ui->optionsAction, SIGNAL(triggered()), this, SLOT(initiateOptions()));
}

void MainWindow::initiateOptions()
{
	MTConfig config;
	config.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
	db.close();
}

bool MainWindow::importSettings()
{
	MTSettings *settings;

	settings = new MTSettings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

	if (!settings->importSettings()) {
		qDebug() << "Failed to read settings";
	}

	db.setHostName(settings->db_host);
	db.setDatabaseName(settings->db_name);
	db.setUserName(settings->db_user);
	db.setPassword(settings->db_password);

	connectDB();

	delete settings;
	return true;
}

void MainWindow::connectDB()
{
	if (!db.isOpen()) {
		db.close();
	}
	db.open();
	if (!db.isOpen()) {
		AlertInterface alert(this);
		alert.databaseInaccessible();
	}
}
