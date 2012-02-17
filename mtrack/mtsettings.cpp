/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "mtsettings.h"

#include <QSettings>

#include "globals.h"

#include <QDebug>

double MTSettings::configversion;
QString MTSettings::db_host;
QString MTSettings::db_name;
QString MTSettings::db_user;
QString MTSettings::db_password;
QSettings * MTSettings::settings = 0;

MTSettings::MTSettings()
{

}

MTSettings::MTSettings(QString org, QString app)
{
	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, org, app);
	settings->setFallbacksEnabled(false);
}

void MTSettings::writeSettings()
{
	if (settings == 0) {
		return;
	}
	settings->setValue("db host", db_host);
	settings->setValue("db name", db_name);
	settings->setValue("db user", db_user);
	settings->setValue("db password", db_password);
}

bool MTSettings::importSettings()
{
	QVariant val;
	bool ok = false;

	val = settings->value("configversion", -1.0);
	val.toDouble(&ok);
	if (ok && (val.toDouble() < 0.0)) {	// Need to fill in default settings
		settings->setValue("config version", QString::number(DEFAULTS::ConfigVersion, 'f', 3));
		configversion = DEFAULTS::ConfigVersion;
	} else {
		configversion = val.toDouble();
		if (val.toDouble() >= DEFAULTS::ConfigIncompatible) {
			return false;	// Config file is not compatible
		}
		if (val.toDouble() < DEFAULTS::ConfigMajor) {
			return false;	// Config file is an earlier version and not compatible
		}
	}

	val = settings->value("db host", -1.0);
	val.toDouble(&ok);
	if (ok && (val.toDouble() < 0.0)) {
		settings->setValue("db host", DEFAULTS::DBHost);
		db_host = DEFAULTS::DBHost;
	} else {
		db_host = val.toString();
	}

	val = settings->value("db name", -1.0);
	val.toDouble(&ok);
	if (ok && (val.toDouble() < 0.0)) {
		settings->setValue("db name", DEFAULTS::DBName);
		db_name = DEFAULTS::DBName;
	} else {
		db_name = val.toString();
	}

	val = settings->value("db user", -1.0);
	val.toDouble(&ok);
	if (ok && (val.toDouble() < 0.0)) {
		settings->setValue("db user", DEFAULTS::DBUser);
		db_user = DEFAULTS::DBUser;
	} else {
		db_user = val.toString();
	}

	val = settings->value("db password", -1.0);
	val.toDouble(&ok);
	if (ok && (val.toDouble() < 0.0)) {
		settings->setValue("db password", DEFAULTS::DBPassword);
		db_password = DEFAULTS::DBPassword;
	} else {
		db_password = val.toString();
	}

	return true;
}
