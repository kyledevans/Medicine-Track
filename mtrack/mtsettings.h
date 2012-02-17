/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QSettings>

#ifndef MTSETTINGS_H
#define MTSETTINGS_H

class MTSettings
{
public:
	MTSettings();
	MTSettings(QString, QString);	// QCoreApplication::organizationName(), QCoreApplication::applicationName()

	bool importSettings();
	void writeSettings();

	static double configversion;
	static QString db_host;
	static QString db_name;
	static QString db_user;
	static QString db_password;

private:
	static QSettings *settings;
};

#endif // MTSETTINGS_H
