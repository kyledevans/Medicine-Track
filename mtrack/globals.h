/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

namespace FORM {
	const int Capsule = 0;
	const int Cream = 1;
	const int Elixir = 2;
	const int Ointment = 3;
	const int Suspension = 4;
	const int Syrup = 5;
	const int Tablet = 6;

	const int Undefined = -1;

	int sqlToForm(QString);
}

namespace SQL {
	const QString Capsule = QString("capsule");
	const QString Cream = QString("cream");
	const QString Elixir = QString("elixir");
	const QString Ointment = QString("ointment");
	const QString Suspension = QString("suspension");
	const QString Syrup = QString("syrup");
	const QString Tablet = QString("tablet");
	const QString Undefined_Form = QString("-1");

	const int Undefined_ID = -1;


	QString formToSql(int);	// Converts the FORM types to SQL strings
}

#endif // GLOBALS_H
