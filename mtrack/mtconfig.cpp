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

	settings = new MTSettings;

	ui->hostField->setText(settings->db_host);
	ui->databaseNameField->setText(settings->db_name);
	ui->usernameField->setText(settings->db_user);
	ui->passwordField->setText(settings->db_password);

	connect(this, SIGNAL(accepted()), this, SLOT(commit()));

	delete settings;
}

MTConfig::~MTConfig()
{
    delete ui;
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
