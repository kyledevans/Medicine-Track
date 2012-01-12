/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriptionrecord.h"

#include "globals.h"

PrescriptionRecord::PrescriptionRecord():
	id(SQL::Undefined_ID),
	patient_id(SQL::Undefined_ID),
	drug_id(SQL::Undefined_ID),
	shipment_id(SQL::Undefined_ID),
	prescriber_id(SQL::Undefined_ID),
	pharmacist_id(SQL::Undefined_ID)
{
}
