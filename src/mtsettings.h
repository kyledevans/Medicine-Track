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
    static QString print_l0c0;
    static QString print_l0c1;
    static QString print_l1c0;
    static QString print_l1c1;
	static QString db_host;
	static QString db_name;
	static QString db_user;
	static QString db_password;

	/* Use these to keep UI strings consistent and sane. */
	static const QString db_host_Label;
	static const QString db_host_Tooltip;

	static const QString db_name_Label;
	static const QString db_name_Tooltip;

	static const QString db_user_Label;
	static const QString db_user_Tooltip;

	static const QString db_password_Label;
	static const QString db_password_Tooltip;

private:
	static QSettings *settings;
};

#endif // MTSETTINGS_H
