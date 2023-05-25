// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "NovasCFunctions.h"
#include "Ephemeris/EphemerisManager.h"
#include "AstroLibBPLibrary.h"
//#include "IPluginManager.h"


short int solarsystem(double tjd, short int body, short int origin, double *position, double *velocity)
{
//	UE_LOG(LogTemp, Warning, TEXT("tjd: %f"), tjd);
//	UE_LOG(LogTemp, Warning, TEXT("body: %i"), body);
//	UE_LOG(LogTemp, Warning, TEXT("origin: %i"), origin);

	EBodys originBody = EBodys::SolarSystemBarycenter;
	if (origin == 1)
	{
		originBody = EBodys::Sun;
	}

	FAstroDoubleVector PosVector;
	FAstroDoubleVector VelVector;

	int error = UEphemerisManager::GetState(UAstroLibBPLibrary::GetBodyFromNovasInt(body), originBody, tjd, 0, PosVector, VelVector);

	position[0] = PosVector.x;
	position[1] = PosVector.y;
	position[2] = PosVector.z;

	velocity[0] = VelVector.x;
	velocity[1] = VelVector.y;
	velocity[2] = VelVector.z;
	
	//UE_LOG(LogTemp, Warning, TEXT("position: %f,%f,%f"), position[0], position[1], position[2]);

	return error;
}

 short int solarsystem_hp(double tjd[2], short int body, short int origin, double *position, double *velocity)
{

	return solarsystem(tjd[0] + tjd[1],body,origin,position,velocity);
}





