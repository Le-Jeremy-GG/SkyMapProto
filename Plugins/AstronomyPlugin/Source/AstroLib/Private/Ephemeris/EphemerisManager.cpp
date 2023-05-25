// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "Ephemeris/EphemerisManager.h"
#include "Ephemeris/JPLDE.h"
#include "Ephemeris/VSOP.h"
#include "Ephemeris/JPLLowPrecision.h"
#include "AstroLibBPLibrary.h"



UEphemerisObject * UEphemerisManager::ActiveEphem = nullptr;
UEphemerisObject * UEphemerisManager::DefaultGlobalEphem = nullptr;

EEphemeris UEphemerisManager::DefaultEphemType = EEphemeris::JPLLowPrecision;

UEphemerisManager::UEphemerisManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActiveEphem = nullptr;
	if (!DefaultGlobalEphem)
	{
		UEphemerisObject * Ephem = UEphemerisManager::CreateNewEphemeris(DefaultEphemType);
		TArray<FString> EmptyArray;
		Ephem->InitEphemeris(EmptyArray);
		UEphemerisManager::SetActiveEphemeris(Ephem);
		DefaultGlobalEphem = Ephem;
		Ephem->AddToRoot();
	}
}

UEphemerisManager::~UEphemerisManager()
{
	if (ActiveEphem )
	{
		if (ActiveEphem->IsValidLowLevel())
		{
			if (!ActiveEphem->IsPendingKillOrUnreachable())
			{
				ActiveEphem->CloseEphemeris();
				ActiveEphem = nullptr;
			}
		}

	}

	if (DefaultGlobalEphem)
	{
		if (DefaultGlobalEphem->IsValidLowLevel())
		{
			if (!DefaultGlobalEphem->IsPendingKillOrUnreachable())
			{
				DefaultGlobalEphem->RemoveFromRoot();
				DefaultGlobalEphem->CloseEphemeris();
				DefaultGlobalEphem = nullptr;
			}
		}

	}
}

UEphemerisObject * UEphemerisManager::CreateNewEphemeris(EEphemeris Ephem, UObject * Outer)
{
	//UEphemerisObject * NewObj = nullptr;
	if (!Outer)
	{
		Outer = (UObject*)GetTransientPackage();
	}

	switch (Ephem)
	{
	case EEphemeris::JPLDE:
		return NewObject<UJPLDE>(Outer);
		break;
	case EEphemeris::VSOP:
		return NewObject<UVSOP>(Outer);
		break;
	case EEphemeris::JPLLowPrecision:
		return NewObject<UJPLLowPrecision>(Outer);
		break;
	}


	return nullptr;
}


int UEphemerisManager::SetActiveEphemeris(UEphemerisObject * InEphObj)
{
	ActiveEphem = InEphObj;

	//Since were changing ephemeris, need to flush out any stored data from NOVAS
	FObserver TmpObserver;
	TmpObserver.LocationType = EWhereIsObserver::EarthSurface;
	TmpObserver.SurfaceLocation.Latitude = 45;
	TmpObserver.SurfaceLocation.Longitude = 45;

	FSkyPosition tmpSkyPos;
	UAstroLibBPLibrary::CalculatePlanetPosition(ESolarSystemBodys::Sun, TmpObserver, FDateTime(2000, 1, 1, 0, 0, 0, 0), ECoordSystems::TrueEquatorAndEquinoxOfDate, tmpSkyPos);

	return 0;
}


UEphemerisObject * UEphemerisManager::GetActiveEphemeris()
{
	return ActiveEphem;
}

int UEphemerisManager::GetState(EBodys ObjBody, EBodys ObjOrigin, FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, EPlane CoordinatePlane, const EEphemerisUnits OutUnit)
{
	int status;
	
	if (ActiveEphem && ActiveEphem->IsValidLowLevel())
	{
		if (ActiveEphem->IsInitialized())
		{
			status = ActiveEphem->GetObjectState(ObjBody, ObjOrigin, jd1, Optionaljd2, Location, Velocity, CoordinatePlane, OutUnit);
			return status;
		}
		else
		{

			//UE_LOG(LogTemp, Warning, TEXT("The active ephemeris used has not been initialized! Will attempt to default initialize."));
			TArray<FString> EmptyArray;
			ActiveEphem->InitEphemeris(EmptyArray);
			status = ActiveEphem->GetObjectState(ObjBody, ObjOrigin, jd1, Optionaljd2, Location, Velocity, CoordinatePlane, OutUnit);
			return status;
		}
	}
	else 
	{
		//UE_LOG(LogTemp, Warning, TEXT("The active ephemeris used has not been created or has been destroyed! Using default ephem."));
		if (DefaultGlobalEphem)
		{
			status = DefaultGlobalEphem->GetObjectState(ObjBody, ObjOrigin, jd1, Optionaljd2, Location, Velocity, CoordinatePlane, OutUnit);
			return status;
		}

		
	}
	
	
	

	return 0;

		
	
}

void UEphemerisManager::CloseEphemeris(UEphemerisObject * EphemObjToClose)
{
	if (EphemObjToClose)
	{
		EphemObjToClose->CloseEphemeris();
		if (ActiveEphem == EphemObjToClose )
		{
			ActiveEphem = nullptr;
		}
		//EphemObjToClose->BeginDestroy();
		EphemObjToClose = nullptr;
	}

}

EBodys UEphemerisManager::Conv_ESolarSystemBodyToEBodys(ESolarSystemBodys InSolarSysytemBody)
{

	switch (InSolarSysytemBody)
	{

	case ESolarSystemBodys::Mercury:
		return EBodys::Mercury;
		break;
	case ESolarSystemBodys::Venus:
		return EBodys::Venus;
		break;
	case ESolarSystemBodys::Earth:
		return EBodys::Earth;
		break;
	case ESolarSystemBodys::Mars:
		return EBodys::Mars;
		break;
	case ESolarSystemBodys::Jupiter:
		return EBodys::Jupiter;
		break;
	case ESolarSystemBodys::Saturn:
		return EBodys::Saturn;
		break;
	case ESolarSystemBodys::Uranus:
		return EBodys::Uranus;
		break;
	case ESolarSystemBodys::Neptune:
		return EBodys::Neptune;
		break;
	case ESolarSystemBodys::Pluto:
		return EBodys::Pluto;
		break;
	case ESolarSystemBodys::Sun:
		return EBodys::Sun;
		break;
	case ESolarSystemBodys::Moon:
		return EBodys::Moon;
		break;
	case ESolarSystemBodys::SolarSystemBarycenter:
		return EBodys::SolarSystemBarycenter;
		break;
	case ESolarSystemBodys::Unset:
		return EBodys::Unset;
		break;

	}
	
	return EBodys::Unset;
}








