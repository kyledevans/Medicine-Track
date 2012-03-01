/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "mtconfig.h"
#include "ui_mtconfig.h"

#include "mtsettings.h"

MTConfig::MTConfig(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MTConfig)
{
	MTSettings *settings;
	ui->setupUi(this);

	// Setup UI strings and tooltips
	ui->hostLabel->setText(MTSettings::db_host_Label);
	ui->hostLabel->setToolTip(MTSettings::db_host_Tooltip);
	ui->hostField->setToolTip(MTSettings::db_host_Tooltip);

	ui->databaseNameLabel->setText(MTSettings::db_name_Label);
	ui->databaseNameLabel->setToolTip(MTSettings::db_name_Tooltip);
	ui->databaseNameField->setToolTip(MTSettings::db_name_Tooltip);

	ui->usernameLabel->setText(MTSettings::db_user_Label);
	ui->usernameLabel->setToolTip(MTSettings::db_user_Tooltip);
	ui->usernameField->setToolTip(MTSettings::db_user_Tooltip);

	ui->passwordLabel->setText(MTSettings::db_password_Label);
	ui->passwordLabel->setToolTip(MTSettings::db_password_Tooltip);
	ui->passwordField->setToolTip(MTSettings::db_password_Tooltip);

	// Create an interface to the settings
	settings = new MTSettings;

	// Fill in the values read from the settings file
	ui->hostField->setText(settings->db_host);
	ui->databaseNameField->setText(settings->db_name);
	ui->usernameField->setText(settings->db_user);
	ui->passwordField->setText(settings->db_password);

	// Connect signals/slots
	connect(this, SIGNAL(accepted()), this, SLOT(commit()));

	delete settings;
}

MTConfig::~MTConfig()
{
	delete ui;
}

void MTConfig::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);

	switch(e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void MTConfig::commit()
{
	MTSettings *settings;
	settings = new MTSettings;

	settings->db_host = ui->hostField->text();
	settings->db_name = ui->databaseNameField->text();
	settings->db_user = ui->usernameField->text();
	settings->db_password = ui->passwordField->text();

	settings->writeSettings();

	delete settings;
}
