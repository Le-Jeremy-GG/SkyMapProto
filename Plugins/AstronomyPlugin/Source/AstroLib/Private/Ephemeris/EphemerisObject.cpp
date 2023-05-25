// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.
#pragma once

#include "Ephemeris/EphemerisObject.h"
#include "Ephemeris/EphemerisManager.h"

int UEphemerisObject::GetIntForBody(EBodys InBody)
{
	
	
	for (auto& InBodyIntMap : BodyToIntArray)
	{
		if (InBodyIntMap.BodyName == InBody)
		{
			return InBodyIntMap.integer;
		}
	}
	return -1;
	
}
