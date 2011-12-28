/*
Copyright (C) 2011 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "shipmentrecord.h"

#include "globals.h"

ShipmentRecord::ShipmentRecord():
	id(SQL::Undefined_ID),
	drug_id(SQL::Undefined_ID),
	product_count(-1),
	product_left(-1),
	exists(false)
{
}
