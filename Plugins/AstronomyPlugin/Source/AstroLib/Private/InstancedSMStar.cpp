// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.
#pragma once

#include "InstancedSMStar.h"


// Sets default values
AInstancedSMStar::AInstancedSMStar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	 
	InstantStaticMeshComp = Cast<UInstancedStaticMeshComponent>(CreateDefaultSubobject(TEXT("StarInstancedSMComponent"), *InstantStaticMeshCompClass, *InstantStaticMeshCompClass, true, false));

	RootComponent = InstantStaticMeshComp;
	InstantStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}



bool AInstancedSMStar::SetStarCatalogData(FName RowNameKey, FStarCatalogEntry InStarCatEntry)
{
	FStarCatalogEntry & CatEntryRef = StarCatEntryMap.FindOrAdd(RowNameKey);
	CatEntryRef.SetupCatalogEntry(InStarCatEntry.StarNameFString, InStarCatEntry.CatalogFString, InStarCatEntry.StarNumberInt, InStarCatEntry.RightAscenstion, InStarCatEntry.Declination, InStarCatEntry.ProperMotionInRightAscension, InStarCatEntry.ProperMotionInDeclination, InStarCatEntry.Parallax, InStarCatEntry.RadialVelocity, InStarCatEntry.VisualMagnitude);
	CatEntryRef.NameKey = RowNameKey;

	return true;
}

bool AInstancedSMStar::GetStarCatalogData(FStarCatalogEntry & OutStarCatEntry, FName RowNameKey)
{
	if (StarCatEntryMap.Contains(RowNameKey))
	{
		OutStarCatEntry = *StarCatEntryMap.Find(RowNameKey);
		
		return true;
	}
	return false;
}

bool AInstancedSMStar::SetCelestialCoordinates(FSkyPosition InSkyPosition, FDateTime InTime, ECoordSystems InCoordSystem, FName RowNameKey)
{

	FSkyPosition & SkyPosRef = CelestialCoordinatesSkyPosMap.FindOrAdd(RowNameKey);
	bool & bCelestialCoordsSetAtLeastOnceRef = bCelestialCoordsSetAtLeastOnceMap.FindOrAdd(RowNameKey);

	SkyPosRef = InSkyPosition;
	CelestialCoordinatesDateTime = InTime;
	CelestialCoordSystem = InCoordSystem;
	bCelestialCoordsSetAtLeastOnceRef = true;
	
	return true;


}

bool AInstancedSMStar::GetCelestialCoordinates(FSkyPosition & OutSkyPosition, FDateTime & OutTime, ECoordSystems &OutCoordSystem, FName RowNameKey)
{

	if (bCelestialCoordsSetAtLeastOnceMap.Contains(RowNameKey))
	{
		bool bCelestialCoordsSetAtLeastOnceRef = *bCelestialCoordsSetAtLeastOnceMap.Find(RowNameKey);

		if (bCelestialCoordsSetAtLeastOnceRef == true)
		{
			if (CelestialCoordinatesSkyPosMap.Contains(RowNameKey))
			{
				FSkyPosition SkyPosRef = *CelestialCoordinatesSkyPosMap.Find(RowNameKey);
				SkyPosRef.Refresh();
				OutSkyPosition = SkyPosRef;
				OutTime = CelestialCoordinatesDateTime;
				OutCoordSystem = CelestialCoordSystem;
				return true;

			}
		}

	}

return false;

}

bool AInstancedSMStar::SetStarPositionType(EStarPlacementInWorld InType)
{
	PlacementType = InType;
	return true;
}

EStarPlacementInWorld AInstancedSMStar::GetStarPositionType()
{
	return PlacementType;
}

bool AInstancedSMStar::SetAzElCoordinates(FAstroDouble InAzimuth, FAstroDouble InElevation, FName RowNameKey)
{ 

	FAstroDouble & AzimuthMapRef = AzimuthMap.FindOrAdd(RowNameKey);
	FAstroDouble & ElevationMapRef = ElevationMap.FindOrAdd(RowNameKey);
	bool & bAzElCoordsSetAtLeastOnceMapRef = bAzElCoordsSetAtLeastOnceMap.FindOrAdd(RowNameKey);


	AzimuthMapRef = InAzimuth;
	ElevationMapRef = InElevation;
	bAzElCoordsSetAtLeastOnceMapRef = true;
	return true;
	

}

bool AInstancedSMStar::GetAzElCoordinates(FAstroDouble &OutAzimuth, FAstroDouble &OutElevation, FName RowNameKey)
{ 

	if (bAzElCoordsSetAtLeastOnceMap.Contains(RowNameKey))
	{
		bool  bAzElCoordsSetAtLeastOnceMapRef = *bAzElCoordsSetAtLeastOnceMap.Find(RowNameKey);
		if (bAzElCoordsSetAtLeastOnceMapRef == true)
		{
			if (AzimuthMap.Contains(RowNameKey) && ElevationMap.Contains(RowNameKey))
			{
				FAstroDouble AzimuthMapRef = *AzimuthMap.Find(RowNameKey);
				FAstroDouble ElevationMapRef = *ElevationMap.Find(RowNameKey);

				OutAzimuth = AzimuthMapRef;
				OutElevation = ElevationMapRef;
				return true;
			}
		}
	}

	
	return false;
	
}

void AInstancedSMStar::StarSpawn(FName RowNameKey, FStarCatalogEntry InStarCatEntry)
{
	if (FindIndexByRowNameKey(RowNameKey) != -1)
	{
		RemoveStar(RowNameKey);
	}

	
	int NewInstance = InstantStaticMeshComp->AddInstance(GetActorTransform());
	StarInstanceMap.Add(RowNameKey, NewInstance);
	
	SetStarCatalogData(RowNameKey, InStarCatEntry);
	InstantStaticMeshComp->MarkRenderStateDirty();
}

bool AInstancedSMStar::RemoveStar(FName RowNameKey)
{

	int InstanceRow = FindIndexByRowNameKey(RowNameKey);
	
	if (InstanceRow != -1)
	{
		InstantStaticMeshComp->RemoveInstance(InstanceRow);
	}
	InstantStaticMeshComp->MarkRenderStateDirty();


	StarInstanceMap.Remove(RowNameKey);
	StarCatEntryMap.Remove(RowNameKey);
	CelestialCoordinatesSkyPosMap.Remove(RowNameKey);
	AzimuthMap.Remove(RowNameKey);
	ElevationMap.Remove(RowNameKey);
	WorldPositionMap.Remove(RowNameKey);

	return true;
}


int AInstancedSMStar::FindIndexByRowNameKey(FName RowNameKey)
{
	if (StarInstanceMap.Find(RowNameKey))
	{
		return *StarInstanceMap.Find(RowNameKey);
	}

	return -1;
}


bool AInstancedSMStar::UpdateStarPosition(FAstroDoubleVector NewWorldPosition, FName RowNameKey)
{

	int InstanceInt = FindIndexByRowNameKey(RowNameKey);

	if (InstanceInt != -1)
	{
		FTransform CurrentTransform;
		InstantStaticMeshComp->GetInstanceTransform(InstanceInt, CurrentTransform, true);
		CurrentTransform.SetLocation(NewWorldPosition.GetFVector());

		InstantStaticMeshComp->UpdateInstanceTransform(InstanceInt, CurrentTransform, true,true,true);
		
		FAstroDoubleVector & WorldPosRef = WorldPositionMap.FindOrAdd(RowNameKey);
		WorldPosRef = NewWorldPosition;
		return true;
	}

	return false;
	
}

bool AInstancedSMStar::SetStarScale(FVector NewScale, FName RowNameKey)
{
	int InstanceInt = FindIndexByRowNameKey(RowNameKey);

	if (InstanceInt != -1)
	{

		FTransform CurrentTransform;
		InstantStaticMeshComp->GetInstanceTransform(InstanceInt,CurrentTransform,true);
		CurrentTransform.SetScale3D(NewScale);

		InstantStaticMeshComp->UpdateInstanceTransform(InstanceInt, CurrentTransform, true, true, true);
		InstantStaticMeshComp->MarkRenderStateDirty();
		return true;

	}

	return false;

}

bool AInstancedSMStar::SetStarRotation(FRotator NewRotation, FName RowNameKey)
{
	int InstanceInt = FindIndexByRowNameKey(RowNameKey);

	if (InstanceInt != -1)
	{

		FTransform CurrentTransform;
		InstantStaticMeshComp->GetInstanceTransform(InstanceInt, CurrentTransform, true);
		CurrentTransform.SetRotation(NewRotation.Quaternion());

		InstantStaticMeshComp->UpdateInstanceTransform(InstanceInt, CurrentTransform, true, true, true);
		InstantStaticMeshComp->MarkRenderStateDirty();
		return true;

	}

	return false;

}


FAstroDoubleVector AInstancedSMStar::GetStarWorldPosition(FName RowNameKey)
{
	if (WorldPositionMap.Contains(RowNameKey))
	{
		FAstroDoubleVector & WorldPos = *WorldPositionMap.Find(RowNameKey);
		return WorldPos;
	}
	return FAstroDoubleVector(FVector(0, 0, 0));
}

bool AInstancedSMStar::FindStarByRowName(FName RowNameKey, int & Instance)
{

	int TestInstance = FindIndexByRowNameKey(RowNameKey);
	if (TestInstance != -1)
	{
		Instance = TestInstance;
		return true;
	}

	Instance = -1;
	return false;

}


bool AInstancedSMStar::FindStarByStarName(FName StarName, FName & StarRowNameKey, int & Instance)
{

	for (auto& CatEntryVal : StarCatEntryMap)
	{
		if (CatEntryVal.Value.StarNameFString == StarName.ToString())
		{
			StarRowNameKey = CatEntryVal.Key;
			Instance = FindIndexByRowNameKey(CatEntryVal.Key);
			return true;
		}
	}
	StarRowNameKey = "";
	Instance = -1;
	return false;

}


bool AInstancedSMStar::GetAllStarRowKeysInActor(TArray<FName> & StarRowKeyArray)
{

	StarCatEntryMap.GenerateKeyArray(StarRowKeyArray);	
	return true;

}