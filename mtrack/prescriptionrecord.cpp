/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptionrecord.h"

#include "globals.h"

PrescriptionRecord::PrescriptionRecord():
	id(SQL::Undefined_ID),
	patient_id(SQL::Undefined_ID)
{
}
