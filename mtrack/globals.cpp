/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "globals.h"

int FORM::sqlToForm(QString val)
{
	QString temp = val.toLower();

	if (temp.contains(SQL::Capsule)) {
		return FORM::Capsule;
	} else if (temp.contains(SQL::Cream)) {
		return FORM::Cream;
	} else if (temp.contains(SQL::Elixir)) {
		return FORM::Elixir;
	} else if (temp.contains(SQL::Ointment)) {
		return FORM::Ointment;
	} else if (temp.contains(SQL::Suspension)) {
		return FORM::Suspension;
	} else if (temp.contains(SQL::Syrup)) {
		return FORM::Syrup;
	} else if (temp.contains(SQL::Tablet)) {
		return FORM::Tablet;
	} else {
		return FORM::Undefined;
	}
}

QString SQL::formToSql(int val)
{
	switch (val) {
	case FORM::Capsule:
		return SQL::Capsule;
	case FORM::Cream:
		return SQL::Cream;
	case FORM::Elixir:
		return SQL::Elixir;
	case FORM::Ointment:
		return SQL::Ointment;
	case FORM::Suspension:
		return SQL::Suspension;
	case FORM::Syrup:
		return SQL::Syrup;
	case FORM::Tablet:
		return SQL::Tablet;
	default:
		return SQL::Undefined_Form;
	}
}
