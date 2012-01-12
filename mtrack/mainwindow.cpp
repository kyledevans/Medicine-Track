/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

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
	ui->setupUi(this);

	db = QSqlDatabase::addDatabase("QMYSQL");

	if (importSettings()) {
		connectDB();
	}

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

bool MainWindow::importSettings()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), QCoreApplication::applicationName());
	QVariant val;

	settings.setFallbacksEnabled(false);

	val = settings.value("configversion", -1.0);

	if (val.toDouble() < 0.0) {	// Need to fill in default settings
		if (!writeDefaults(&settings)) {
			return false;
		}
	}

	val = settings.value("db host", "-1");
	db.setHostName(val.toString());

	val = settings.value("db name");
	db.setDatabaseName(val.toString());

	val = settings.value("db user");
	db.setUserName(val.toString());

	val = settings.value("db password");
	db.setPassword(val.toString());

	return true;
}

bool MainWindow::writeDefaults(QSettings *settings)
{
	QVariant val;

	val = settings->value("configversion", -1);
	if (val.toString().contains("-1")) {	// Need to fill in default settings
		settings->setValue("config version", DEFAULTS::ConfigVersion);
	} else if (val.toDouble() >= DEFAULTS::ConfigIncompatible) {
		return false;	// Config file is not compatible
	} else if (val.toDouble() < DEFAULTS::ConfigMajor) {
		return false;	// Config file is an earlier version and not compatible
	}
	val = settings->value("db host", "-1");
	if (val.toString().contains("-1")) {
		settings->setValue("db host", DEFAULTS::DBHost);
	}
	val = settings->value("db name", "-1");
	if (val.toString().contains("-1")) {
		settings->setValue("db name", DEFAULTS::DBName);
	}
	val = settings->value("db user", "-1");
	if (val.toString().contains("-1")) {
		settings->setValue("db user", DEFAULTS::DBUser);
	}
	val = settings->value("db password", "-1");
	if (val.toString().contains("-1")) {
		settings->setValue("db password", DEFAULTS::DBPassword);
	}
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
