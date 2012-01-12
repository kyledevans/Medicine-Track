/*
Copyright (C) 2011-2012 Kyle Evans <kyledevans@gmail.com>

Released under the GPL version 2 only.
*/

#include "prescriberrecord.h"

#include "globals.h"

PrescriberRecord::PrescriberRecord():
	id(SQL::Undefined_ID),
	active(true),
	exists(false)
{
}
