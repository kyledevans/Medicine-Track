﻿/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include <QString>
#include <QDate>
#include <QVariant>
#include <QColor>

#ifndef GLOBALS_H
#define GLOBALS_H

namespace FORM_INT {
	const int Capsule = 0;
	const int Cream = 1;
	const int Drops = 2;
	const int Elixir = 3;		// Come in fixed amounts
	const int Ointment = 4;
	const int Solution = 5;
	const int Suspension = 6;	// Come in fixed amounts
	const int Syrup = 7;
	const int Tablet = 8;

	const int Undefined = -1;

	int strToInt(QString);
}

namespace FORM_STR {
	const QString Capsule = QString("capsule");
	const QString Cream = QString("cream");
	const QString Drops = QString("drops");
	const QString Elixir = QString("elixir");
	const QString Ointment = QString("ointment");
	const QString Solution = QString("solution");
	const QString Suspension = QString("suspension");
	const QString Syrup = QString("syrup");
	const QString Tablet = QString("tablet");
	const QString Undefined = QString("-1");

	QString intToStr(int);	// Converts the FORM types to SQL strings
}

namespace DEFAULTS {
	const QDate Date(1970, 1, 31);
	const QString DateDisplayFormat("M/d/yyyy");
	const QString Organization("MTrack");
	const QString Domain("MTrack");
	const QString Application("MTrack");
	const QString DBHost("127.0.0.1");
	const QString DBName("mtrack");
	const QString DBUser("anon_mtrack");
	const QString DBPassword("anon_mtrack");
	const QString ImageDir("/images/");
	const double AppVersion = 1.2;			// The version of the application
	const double AppMajor = 1.0;			// The version of the application without any decimal places
	const double ConfigVersion = 1.0;		// Not the version of the software, simply the config version
	const double ConfigIncompatible = 2.0;	// The next version of the config file that is not compatible
	const double ConfigMajor = 1.0;			// The version of the config without any decimal places
	const bool ManagerEnabled = false;		// Default value for "manager" setting
}

namespace MTCOLORS {
	const QString Problem = QString("FF0000");
	const QColor Problem_BG(255, 192, 192);
}

namespace SQL {
	const int Undefined_ID = -1;

	QString cleanInput(QString);		// Used when doing pattern matching (IE: SELECT ... WHEN x LIKE ___)
	QString cleanNoMatching(QString);	// Doesn't clean '%' and '_' because it's safe

	QVariant prep(QString);				// Cleans and returns a value ready for inserting into SQL (uses cleanInput())
	QVariant prepWildcards(QString);	// Cleans and returns a value ready for inserting into SQL (uses cleanInput()) surrounds string with '%'
	QVariant prepNoMatching(QString);	// Cleans and returns a value ready for inserting into SQL (uses cleanNoMatching())
}

#endif // GLOBALS_H
