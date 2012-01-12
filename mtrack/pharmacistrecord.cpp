/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "pharmacistrecord.h"

#include "globals.h"

PharmacistRecord::PharmacistRecord():
	id(SQL::Undefined_ID),
	active(true),
	exists(false)
{
}
