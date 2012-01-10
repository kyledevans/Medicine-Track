/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "globals.h"

int FORM_INT::strToInt(QString val)
{
	QString temp = val.toLower();

	if (temp.contains(FORM_STR::Capsule, Qt::CaseInsensitive)) {
		return FORM_INT::Capsule;
	} else if (temp.contains(FORM_STR::Cream, Qt::CaseInsensitive)) {
		return FORM_INT::Cream;
	} else if (temp.contains(FORM_STR::Drops, Qt::CaseInsensitive)) {
		return FORM_INT::Drops;
	} else if (temp.contains(FORM_STR::Elixir, Qt::CaseInsensitive)) {
		return FORM_INT::Elixir;
	} else if (temp.contains(FORM_STR::Ointment, Qt::CaseInsensitive)) {
		return FORM_INT::Ointment;
	} else if (temp.contains(FORM_STR::Solution, Qt::CaseInsensitive)) {
		return FORM_INT::Solution;
	} else if (temp.contains(FORM_STR::Suspension, Qt::CaseInsensitive)) {
		return FORM_INT::Suspension;
	} else if (temp.contains(FORM_STR::Syrup, Qt::CaseInsensitive)) {
		return FORM_INT::Syrup;
	} else if (temp.contains(FORM_STR::Tablet, Qt::CaseInsensitive)) {
		return FORM_INT::Tablet;
	} else {
		return FORM_INT::Undefined;
	}
}

QString FORM_STR::intToStr(int val)
{
	switch (val) {
	case FORM_INT::Capsule:
		return FORM_STR::Capsule;
	case FORM_INT::Cream:
		return FORM_STR::Cream;
	case FORM_INT::Drops:
		return FORM_STR::Drops;
	case FORM_INT::Elixir:
		return FORM_STR::Elixir;
	case FORM_INT::Ointment:
		return FORM_STR::Ointment;
	case FORM_INT::Solution:
		return FORM_STR::Solution;
	case FORM_INT::Suspension:
		return FORM_STR::Suspension;
	case FORM_INT::Syrup:
		return FORM_STR::Syrup;
	case FORM_INT::Tablet:
		return FORM_STR::Tablet;
	default:
		return FORM_STR::Undefined_Form;
	}
}

QString SQL::cleanInput(QString str)
{
	QRegExp esc0("\\\\");
	QRegExp esc1("\'");
	QRegExp esc2("\"");
	QRegExp esc3("%");
	QRegExp esc4("_");
	str.replace(esc0, "\\\\");
	str.replace(esc1, "\\'");
	str.replace(esc2, "\\\"");
	str.replace(esc3, "\\%");
	str.replace(esc4, "\\_");

	return str;
}

QString SQL::cleanNoMatching(QString str)
{
	QRegExp esc0("\\\\");
	QRegExp esc1("\'");
	QRegExp esc2("\"");
	str.replace(esc0, "\\\\");
	str.replace(esc1, "\\'");
	str.replace(esc2, "\\\"");

	return str;
}
