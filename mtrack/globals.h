/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>
#include <QDate>

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
	const QString Undefined_Form = QString("-1");

	QString intToStr(int);	// Converts the FORM types to SQL strings
}

namespace DEFAULTS {
	const QDate Date(1970, 1, 31);
	const QString Organization("EPC");
	const QString Domain("evanspediatric.com");
	const QString Application("MTrack");
	const QString DBHost("192.168.1.99");
	const QString DBName("epc");
	const QString DBUser("kyle");
	const QString DBPassword("kobalt");
	const double ConfigVersion = 1.0;		// Not the version of the software, simply the config version
	const double ConfigIncompatible = 2.0;	// The next version of the config file that is not compatible
	const double ConfigMajor = 1.0;			// The version of the config without any decimal places
}

namespace SQL {
	const int Undefined_ID = -1;

	QString cleanInput(QString);		// Used when doing pattern matching (IE: SELECT ... WHEN x LIKE ___)
	QString cleanNoMatching(QString);	// Doesn't clean '%' and '_' because it's safe
}

#endif // GLOBALS_H
