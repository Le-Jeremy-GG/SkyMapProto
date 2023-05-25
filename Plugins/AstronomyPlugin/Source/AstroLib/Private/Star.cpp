// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "Star.h"


// Sets default values
AStar::AStar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


}



bool AStar::SetStarCatalogData(FName RowNameKey, FStarCatalogEntry InStarCatEntry)
{

	StarCatEntry.SetupCatalogEntry(InStarCatEntry.StarNameFString, InStarCatEntry.CatalogFString, InStarCatEntry.StarNumberInt, InStarCatEntry.RightAscenstion, InStarCatEntry.Declination, InStarCatEntry.ProperMotionInRightAscension, InStarCatEntry.ProperMotionInDeclination, InStarCatEntry.Parallax, InStarCatEntry.RadialVelocity, InStarCatEntry.VisualMagnitude);
	StarCatEntry.NameKey = RowNameKey;
	return true;
}

bool AStar::GetStarCatalogData(FStarCatalogEntry & OutStarCatEntry, FName RowNameKey)
{
	StarCatEntry.Refresh();
	OutStarCatEntry = StarCatEntry;
	return true;

}

bool AStar::SetCelestialCoordinates(FSkyPosition InSkyPosition, FDateTime InTime, ECoordSystems InCoordSystem, FName RowNameKey)
{

	CelestialCoordinatesSkyPos = InSkyPosition;
	CelestialCoordinatesDateTime = InTime;
	CelestialCoordSystem = InCoordSystem;
	bCelestialCoordsSetAtLeastOnce = true;

	return true;

}

bool AStar::GetCelestialCoordinates(FSkyPosition & OutSkyPosition, FDateTime & OutTime, ECoordSystems &OutCoordSystem, FName RowNameKey)
{
	if (bCelestialCoordsSetAtLeastOnce == true)
	{
		CelestialCoordinatesSkyPos.Refresh();
		OutSkyPosition = CelestialCoordinatesSkyPos;
		OutTime = CelestialCoordinatesDateTime;
		OutCoordSystem = CelestialCoordSystem;
		return true;
	}
	else
	{
		return false;
	}

}

bool AStar::SetStarPositionType(EStarPlacementInWorld InType)
{
	PlacementType = InType;
	return true;
}

EStarPlacementInWorld AStar::GetStarPositionType()
{
	return PlacementType;
}

bool AStar::SetAzElCoordinates(FAstroDouble InAzimuth, FAstroDouble InElevation, FName RowNameKey)
{ 
	Azimuth = InAzimuth;
	Elevation = InElevation;
	bAzElCoordsSetAtLeastOnce = true;
	return true;

}

bool AStar::GetAzElCoordinates(FAstroDouble &OutAzimuth, FAstroDouble &OutElevation, FName RowNameKey)
{ 
	if (bAzElCoordsSetAtLeastOnce == true)
	{
		OutAzimuth = Azimuth;
		OutElevation = Elevation;
		return true;
	}
	else
	{
		return false;
	}
}

void AStar::StarSpawn(FName RowNameKey, FStarCatalogEntry InStarCatEntry)
{
	SetStarCatalogData(RowNameKey, InStarCatEntry);
}

bool AStar::RemoveStar(FName RowNameKey)
{
	return true;
}


bool AStar::SetStarScale(FVector NewScale, FName RowNameKey)
{

	SetActorScale3D(NewScale);
	

	return true;
}

bool AStar::SetStarRotation(FRotator NewRotation, FName RowNameKey)
{
	SetActorRotation(NewRotation);
	return true;
}


bool AStar::UpdateStarPosition(FAstroDoubleVector NewWorldPosition, FName RowNameKey)
{
	WorldPosition = NewWorldPosition;
	SetActorLocation(NewWorldPosition.GetFVector());
	return true;
}


bool AStar::FindStarByRowName(FName RowNameKey, int & Instance)
{
	StarCatEntry.Refresh();
	if (StarCatEntry.NameKey == RowNameKey)
	{
		return true;
	}
	return false;

}


bool AStar::FindStarByStarName(FName StarName, FName & StarRowNameKey, int & Instance)
{
	StarCatEntry.Refresh();
	if (StarCatEntry.StarNameFString == StarName.ToString())
	{
		StarRowNameKey = StarCatEntry.NameKey;
		Instance = -1;
		return true;
	}
	return false;

}



bool AStar::GetAllStarRowKeysInActor(TArray<FName> & StarRowKeyArray)
{
	StarCatEntry.Refresh();
	StarRowKeyArray.Add(StarCatEntry.NameKey);
	return true;
}




