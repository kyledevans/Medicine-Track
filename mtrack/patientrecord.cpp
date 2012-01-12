/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "patientrecord.h"

#include "globals.h"

PatientRecord::PatientRecord():
	id(SQL::Undefined_ID),
	allscripts_id(SQL::Undefined_ID),
	exists(false)
{
}
