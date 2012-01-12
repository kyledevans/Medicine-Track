/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "medicationrecord.h"
#include "globals.h"

MedicationRecord::MedicationRecord():
	id(SQL::Undefined_ID),
	instruction_id(SQL::Undefined_ID),
	form(FORM::Undefined),
	active(false),
	exists(false)
{
}
