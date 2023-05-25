// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.
#pragma once

#include "StarManager.h"
#include "StarInterface.h"
#include "Star.h"
#include "Ephemeris/EphemerisManager.h"
#include "Time.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

FUpdateStarsAsync* FUpdateStarsAsync::Runnable = NULL;

AStarManager::AStarManager()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	

	#if WITH_EDITOR
	//Default star actor to spawn if no valid star found in StarClassTable
	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/AstroLib/AstroLib/Blueprints/Stars/BP_Star.BP_Star'"));
	if (ItemBlueprint.Object) {
		DefaultStarClassToSpawn = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
	#endif

	SetENUVectorsInECEF();
	SetCelestialAxes();

}

void AStarManager::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!UEphemerisManager::GetActiveEphemeris() || !UEphemerisManager::GetActiveEphemeris()->IsValidLowLevelFast())
	{
		Ephem = UEphemerisManager::CreateNewEphemeris(DefaultEphem);
		UEphemerisManager::SetActiveEphemeris(Ephem);
		TArray<FString> EmptyArray;
		Ephem->InitEphemeris(EmptyArray);

		
		
		

	}
}

void AStarManager::SetENUVectorsInECEF()
{

	double lon_rad = Observer.SurfaceLocation.Longitude * DEG2RAD *-1;
	double lat_rad = Observer.SurfaceLocation.Latitude * DEG2RAD;
	double sin_lon = sin(lon_rad);
	double cos_lon = cos(lon_rad);
	double sin_lat = sin(lat_rad);
	double cos_lat = cos(lat_rad);

	ECEFEast[0] = -sin_lon;
	ECEFEast[1] = cos_lon;
	ECEFEast[2] = 0;

	ECEFNorth[0] = -cos_lon*sin_lat;
	ECEFNorth[1] = -sin_lon * sin_lat;
	ECEFNorth[2] = cos_lat;

	
	ECEFUp[0] = cos_lat * cos_lon;
	ECEFUp[1] = cos_lat * sin_lon;
	ECEFUp[2] = sin_lat;

}


void AStarManager::SetCelestialAxes()
{
	//Used for the 'Fast Update' mode, this gets the axis used to rotate this actor, which rotates all child actors.

	double jd_ut1 = UAstroLibBPLibrary::GetUT1JulianDatefromUTC(UTCTime);

	if (bAlwaysUpdateDeltaT)
	{
		delta_t = float(Time::getDeltaTByEspenakMeeus(UTCTime));
	}

	double OutCelVec[3];
	
	cel2ter(jd_ut1, 0.0, delta_t, 1, 0, 0, 0, 0, ECEFUp, OutCelVec);
	TerrestrialZAxisInCelestialSpace.SetDoubleVector(OutCelVec);

	cel2ter(jd_ut1, 0.0, delta_t, 1, 0, 0, 0, 0, ECEFNorth, OutCelVec);
	TerrestrialXAxisInCelestialSpace.SetDoubleVector(OutCelVec);

	cel2ter(jd_ut1, 0.0, delta_t, 1, 0, 0, 0, 0, ECEFEast, OutCelVec);
	TerrestrialYAxisInCelestialSpace.SetDoubleVector(OutCelVec);
	
}


void AStarManager::UpdateSurfaceLocationAndCelestialAxes()
{
	SetENUVectorsInECEF();
	SetCelestialAxes();
	//delta_t = float(Time::getDeltaTByEspenakMeeus(UTCTime));
	bAxesAndENUVectorsSetAtLeastOnce = true;
}


void AStarManager::SpawnStars(UWorld* World)
{

	if (!World)
	{
		World = GetWorld();
	}

	if (StarCatalogData)
	{

		for (auto it : StarCatalogData->GetRowMap())
		{

			FStarDataEntryRowBase * DataRow = (FStarDataEntryRowBase*)(it.Value);
			FStarCatalogEntry RowCatEntry = MakeFStarCatalogEntryFromDataRow(*DataRow, it.Key);

			SpawnStar(it.Key, RowCatEntry, World);


		}
	}
	else
	{
		UE_LOG(LogAstroLog, Warning, TEXT("No Star catalog data was selected."));
	}
}

AActor * AStarManager::SpawnStar(FName RowNameKey, FStarCatalogEntry CatEntryData, UWorld * World)
{
	AActor * StarActor = nullptr;
	IStarInterface* StarInterface;

	TSubclassOf<AActor> tmpActorStaticClass = GetStarClassToSpawn(CatEntryData);

	if (tmpActorStaticClass)
	{
		StarActor = GetActorToUse(tmpActorStaticClass, World);
		StarInterface = Cast<IStarInterface>(StarActor);

		if (!StarActor)
		{
			FName SpawnRowNameKey = RowNameKey;

			if (StarInterface)
			{
				if (StarInterface->UseInstancedStaticMesh())
				{
					SpawnRowNameKey = "Multiple";
				}
			}

			StarActor = World->SpawnActor(tmpActorStaticClass);
			SpawnedStarActorMap.Add(StarActor, SpawnRowNameKey);
			StarActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			StarInterface = Cast<IStarInterface>(StarActor);

		}

		

		if (StarInterface)
		{
			StarInterface->StarSpawn(RowNameKey, CatEntryData);
		}

		PostStarSpawn(StarActor, CatEntryData, RowNameKey);

		if (StarCalculationMethod == EStarUpdateOptions::FastUpdate)
		{
			//FastUpdate needs to initialize by calculating celestial coords
			UpdateStarPosition(StarActor, RowNameKey, EStarUpdateOptions::OnlyCelestial);
		}
		else if (StarCalculationMethod == EStarUpdateOptions::OnlyHorizon)
		{
			//Cant only update Horizon coordinated without first calculating full cellectial coordinates, so do full on spawn.
			UpdateStarPosition(StarActor, RowNameKey, EStarUpdateOptions::Full);
		}
		else
		{
			UpdateStarPosition(StarActor, RowNameKey, StarCalculationMethod);
		}

	}

	return StarActor;


}

		
AActor * AStarManager::GetActorToUse(TSubclassOf<AActor> StarClass, UWorld * World)
{
	
	for (TActorIterator<AActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsA(StarClass))
		{
			IStarInterface* tmpStarInterface = Cast<IStarInterface>(*ActorItr);
			if (tmpStarInterface)
			{
				if (tmpStarInterface->UseInstancedStaticMesh())
				{
					return *ActorItr;
				}
			}
		}

	}
	return nullptr;
}

TSubclassOf<AActor> AStarManager::GetStarClassToSpawn (const FStarCatalogEntry CatEntryData)
{
	float VisMag = CatEntryData.VisualMagnitude;
	
	if (StarClassTable.Num() > 0)
	{
		for (auto& StarClass : StarClassTable)
		{
			if (StarClass.MaxVisualMag >= VisMag && StarClass.MinVisualMag <= VisMag)
			{
				return StarClass.StarClassToSpawn;
			}
		}
		return nullptr;
	}
	
		return DefaultStarClassToSpawn;
}


bool AStarManager::PostStarSpawn(AActor * SpawnedStar, FStarCatalogEntry CatEntryData, FName RowNameKey)
{

	float VisMag = CatEntryData.VisualMagnitude;
	FVector TempMultiplier = FVector(1, 1, 1);

	if (StarSizeTable.Num() > 0)
	{
		for (auto& StarSize : StarSizeTable)
		{
			if (StarSize.MaxVisualMag >= VisMag && StarSize.MinVisualMag <= VisMag)
			{
				TempMultiplier = StarSize.StarSizeMultiplier;
			}
		}
		

		IStarInterface* StarInterface = Cast<IStarInterface>(SpawnedStar);
		if (StarInterface)
		{
			StarInterface->SetStarScale(TempMultiplier, RowNameKey);


		}
		else
		{
			SpawnedStar->SetActorScale3D(TempMultiplier);
		}
	}

	return true;


}

void AStarManager::UpdateStarPosition(AActor * Star, FName RowNameKey, EStarUpdateOptions UpdateOptions)
{

	FVector StarManagerLoc = GetActorLocation();
	FAstroDoubleVector StarManagerLocDbl;
	StarManagerLocDbl.SetDoubleVector(StarManagerLoc.X, StarManagerLoc.Y, StarManagerLoc.Z);

	FAstroDoubleVector NewPosition = CalculateStarPosition(Star, UpdateOptions, RowNameKey) + StarManagerLocDbl;
	if (AdjustForEclipticalPlane)
	{
		NewPosition = NewPosition.RotateAngleAxisDbl(23.5, FVector(0.f, 1.f, 0.f));
	}
	NewPosition = NewPosition.RotateAngleAxisDbl(NorthDirection, FVector(0.f, 0.f, 1.f));
	FRotator NewRot = FRotationMatrix::MakeFromX(GetActorLocation() - NewPosition.GetFVector()).Rotator();

	IStarInterface* StarInterface = Cast<IStarInterface>(Star);

	if (StarInterface)
	{
		StarInterface->UpdateStarPosition(NewPosition, RowNameKey);
		StarInterface->SetStarRotation(NewRot, RowNameKey);



		if (UpdateOptions == EStarUpdateOptions::Full)
		{
			StarInterface->SetStarPositionType(EStarPlacementInWorld::HorizonAzEl);
		}
		else if (UpdateOptions == EStarUpdateOptions::OnlyCelestial)
		{
			StarInterface->SetStarPositionType(EStarPlacementInWorld::Celestial);
		}
		else if (UpdateOptions == EStarUpdateOptions::OnlyHorizon)
		{
			StarInterface->SetStarPositionType(EStarPlacementInWorld::HorizonAzEl);
		}


	}
	else
	{
		Star->SetActorLocation(NewPosition.GetFVector());
		Star->SetActorRotation(NewRot);

	}




}


void AStarManager::UpdateAllStarPositions(EStarUpdateOptions UpdateOptions)
{

	if (UpdateOptions == EStarUpdateOptions::FastUpdate)
	{

		//If stars are already rotated based on earths rotation, then rotation based on StarManager will be all off. Need to reset the stars back to celestial coordinates without earth rotation's built in.
		if (StarsSetToHorizon)
		{
			SetActorRotation(FRotator(0.f,0.f,0.f));
			UpdateAllStarPositions(EStarUpdateOptions::OnlyCelestial);
		}
		if (!bAxesAndENUVectorsSetAtLeastOnce)
		{
			UpdateSurfaceLocationAndCelestialAxes();
		}
		if (bAlwaysUpdateEarthSurfaceLocation)
		{
			SetENUVectorsInECEF();
		}
		
		SetCelestialAxes();
		FMatrix RotationMatrix(TerrestrialXAxisInCelestialSpace.GetFVector().GetUnsafeNormal(), TerrestrialYAxisInCelestialSpace.GetFVector().GetUnsafeNormal()*-1, TerrestrialZAxisInCelestialSpace.GetFVector().GetUnsafeNormal(), FVector::ZeroVector);
		FRotator Rot = RotationMatrix.Rotator().GetInverse();
		
		if (AdjustForEclipticalPlane)
		{
			Rot.Add(23.5, NorthDirection, 0);
		}
		
		Rot.Add(0, NorthDirection, 0);

		

		SetActorRotation(Rot);

	}
	
	//All StarUpdate methods aside from FastUpdate
	else
	{
		for (auto& ActorPtr : SpawnedStarActorMap)
		{
			IStarInterface* StarInterface = Cast<IStarInterface>(ActorPtr.Key);

			if (StarInterface)
			{
				TArray<FName> StarKeyArray;
				StarInterface->GetAllStarRowKeysInActor(StarKeyArray);
				for (auto& StarArrayVal : StarKeyArray)
				{
					UpdateStarPosition(ActorPtr.Key, StarArrayVal, UpdateOptions);
				}

			}
			else
			{
				UpdateStarPosition(ActorPtr.Key, ActorPtr.Value, UpdateOptions);
			}
		}


		if (UpdateOptions == EStarUpdateOptions::OnlyCelestial)
		{
			StarsSetToHorizon = false;
		}
		else 
		{
			StarsSetToHorizon = true;
		}
	}
}




bool AStarManager::UpdateAllStarPositionsAsync(EStarUpdateOptions UpdateOptions)
{
		TMap<AActor*, FName> StarsToUpdate;
		StarsToUpdate = SpawnedStarActorMap;

		return UpdateStarPositionsAsync(StarsToUpdate, UpdateOptions);
}



bool AStarManager::UpdateStarPositionsAsync(TMap<AActor*, FName> InActorMap, EStarUpdateOptions UpdateOptions)
{

	if (StarCalculationMethod != EStarUpdateOptions::FastUpdate)
	{

		bool isthreaddone = FUpdateStarsAsync::IsThreadFinished();
		if (isthreaddone == true)
		{
			FUpdateStarsAsync::Shutdown();
			FUpdateStarsAsync::AsyncInit(this, InActorMap);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FastUpdate calculation method not valid for Async update."));
		return false;
	}

}



FStarCatalogEntry AStarManager::GetStarCatalogPosition(AActor * InActorPtr, FName RowNameKey)
{
	FStarCatalogEntry CatEntryData;
	if (InActorPtr)
	{
		IStarInterface* StarInterface = Cast<IStarInterface>(InActorPtr);
		if (StarInterface)
		{
			if (StarInterface->GetStarCatalogData(CatEntryData, RowNameKey))
			{
				return CatEntryData;
			}
		}


	FStarDataEntryRowBase * tempRow = StarCatalogData->FindRow<FStarDataEntryRowBase>(RowNameKey, "Cannot find star catalog data in DataTable, and StarInterface not implemented.");
	CatEntryData = MakeFStarCatalogEntryFromDataRow(*tempRow, RowNameKey);
	
	}
	
	return CatEntryData;

}


FAstroDoubleVector AStarManager::CalculateStarPosition(AActor * Star, EStarUpdateOptions UpdateOptions, FName RowNameKey, bool AttemptToUpdateStarInterface)
{
	

	FAstroDoubleVector NewLoc;
	
	IStarInterface* StarInterface = Cast<IStarInterface>(Star);

	bool UpdateInterface = StarInterface && AttemptToUpdateStarInterface;

	if (UpdateOptions == EStarUpdateOptions::Full)
	{
		FSkyPosition SkyPos;
		FAstroDouble Azimuth, Altitude;
		FSphereCoords SphereCoords;
		FStarCatalogEntry StarCatEntry;

		StarCatEntry = GetStarCatalogPosition(Star, RowNameKey);
		UAstroLibBPLibrary::CalculateStarPosition(StarCatEntry, Observer, UTCTime, RequestedCoordSystem, SkyPos);
		UAstroLibBPLibrary::TopocentricToHorizon(UTCTime, 0, 0, Observer.SurfaceLocation, SkyPos.SphericalCoords, RefractionOpts, Azimuth, Altitude, SphereCoords);
		NewLoc = UAstroLibBPLibrary::AzElToRectangular(Azimuth, Altitude, SpawnDistance);


		if (UpdateInterface)
		{
			StarInterface->SetCelestialCoordinates(SkyPos, UTCTime, RequestedCoordSystem, RowNameKey);
			StarInterface->SetAzElCoordinates(Azimuth,Altitude, RowNameKey);
		}


	}

	else if (UpdateOptions == EStarUpdateOptions::OnlyCelestial)
	{
		FSkyPosition SkyPos;
		FStarCatalogEntry StarCatEntry;

		StarCatEntry = GetStarCatalogPosition(Star, RowNameKey);
		UAstroLibBPLibrary::CalculateStarPosition(StarCatEntry, Observer, UTCTime, RequestedCoordSystem, SkyPos);
		NewLoc.SetDoubleVector(SkyPos.PositionVector.x * SpawnDistance, SkyPos.PositionVector.y * SpawnDistance *-1, SkyPos.PositionVector.z * SpawnDistance);

		if (UpdateInterface)
		{
			StarInterface->SetCelestialCoordinates(SkyPos, UTCTime, RequestedCoordSystem, RowNameKey);
		}


	}
	else if (UpdateOptions == EStarUpdateOptions::OnlyHorizon)
	{
		FSkyPosition SkyPos;
		ECoordSystems CoordSystem;
		FDateTime OutTime;
		bool GotCelestialCoords =false;

		FAstroDouble Azimuth, Altitude;
		FSphereCoords SphereCoords;

		if (StarInterface)
		{

			GotCelestialCoords = StarInterface->GetCelestialCoordinates(SkyPos, OutTime, CoordSystem, RowNameKey);

			if (!GotCelestialCoords)
			{
				UE_LOG(LogTemp, Warning, TEXT("Warning! Trying to update Horizon coordinates, but no Celestial coordinates have been set, or StarInterface not implemented."));
			}
		}

		else if (!StarInterface || !GotCelestialCoords)
		{

			FStarCatalogEntry StarCatEntry = GetStarCatalogPosition(Star, RowNameKey);
			UAstroLibBPLibrary::CalculateStarPosition(StarCatEntry, Observer, UTCTime, RequestedCoordSystem, SkyPos);

			if (UpdateInterface)
			{
				StarInterface->SetCelestialCoordinates(SkyPos, UTCTime, RequestedCoordSystem, RowNameKey);
			}
		}

		UAstroLibBPLibrary::TopocentricToHorizon(UTCTime, 0, 0, Observer.SurfaceLocation, SkyPos.SphericalCoords, RefractionOpts, Azimuth, Altitude, SphereCoords);
		NewLoc = UAstroLibBPLibrary::AzElToRectangular(Azimuth, Altitude, SpawnDistance);

		if (UpdateInterface)
		{
			StarInterface->SetAzElCoordinates(Azimuth, Altitude, RowNameKey);
		}


	}
	else if (UpdateOptions == EStarUpdateOptions::FastUpdate)
	{
		float Azimuth, Elevation;
		if (StarInterface)
		{
			FVector StarLoc = StarInterface->GetStarWorldPosition().GetFVector();
			UAstroLibBPLibrary::RectangularToAzEl(StarLoc, Azimuth, Elevation);
			StarInterface->SetAzElCoordinates(Azimuth, Elevation, RowNameKey);
			NewLoc = StarLoc;
		}
		else
		{
			NewLoc = Star->GetActorLocation();
		}
	}

	return NewLoc;

}


void AStarManager::RemoveAllStars()
{
	
	
	for (auto& ActorPtr : SpawnedStarActorMap)
	{

		if (ActorPtr.Key)
		{

			IStarInterface* StarInterface = Cast<IStarInterface>(ActorPtr.Key);
			if (StarInterface)
			{
				StarInterface->RemoveStar(ActorPtr.Value);
			}
			ActorPtr.Key->Destroy();
			ActorPtr.Key = nullptr;
		}
	}

	SpawnedStarActorMap.Empty();



}

void AStarManager::RemoveStar(AActor * ActorToRemove, FName RowNameKey)
{
	if (ActorToRemove)
	{
		IStarInterface* StarInterface = Cast<IStarInterface>(ActorToRemove);
		if (StarInterface)
		{
			StarInterface->RemoveStar(RowNameKey);
		}

		SpawnedStarActorMap.Remove(ActorToRemove);
		ActorToRemove->Destroy();
		ActorToRemove = nullptr;
	}

	//Remove star from SpawnedStarActorMap
	SpawnedStarActorMap.Remove(ActorToRemove);

}

FStarCatalogEntry AStarManager::MakeFStarCatalogEntryFromDataRow(FStarDataEntryRowBase DataRow, FName RowNameKey)
{
	FStarCatalogEntry NewEntry;
	NewEntry.SetupCatalogEntry(DataRow.StarNameFString, DataRow.CatalogFString, DataRow.StarNumberInt, FCString::Atof(*DataRow.RightAscenstion), FCString::Atof(*DataRow.Declination), FCString::Atof(*DataRow.ProperMotionInRightAscension), FCString::Atof(*DataRow.ProperMotionInDeclination), FCString::Atof(*DataRow.Parallax), FCString::Atof(*DataRow.RadialVelocity), FCString::Atof(*DataRow.OptionalVisualMagnitude));
	NewEntry.NameKey = RowNameKey;
	return NewEntry;
}

bool AStarManager::FindStarByRowNameKey(const FName RowNameKey, AActor* & StarActor, int & Instance)
{
	TArray<AActor*> StarInterfaceActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UStarInterface::StaticClass(), StarInterfaceActors);
	Instance = -1;

		for (auto a : StarInterfaceActors)
		{
			IStarInterface* StarInterface = Cast<IStarInterface>(a);
			if (StarInterface)
			{
				if (StarInterface->FindStarByRowName(RowNameKey, Instance))
				{
					StarActor = a;
					return true;
				}
			}
		}

		StarActor = nullptr;
	return false;
}

bool AStarManager::FindStarLocationByRowNameKey(const FName RowNameKey, FAstroDoubleVector & WorldLoc)
{
	int StarInst;
	AActor * StarActor;
	FindStarByRowNameKey(RowNameKey, StarActor, StarInst);

	if (StarActor)
	{
		IStarInterface* StarInterface = Cast<IStarInterface>(StarActor);
		if (StarInterface)
		{
			WorldLoc = StarInterface->GetStarWorldPosition(RowNameKey);
			return true;
		}
	}

	return false;
}

bool AStarManager::FindStarByStarName(const FName StarName, AActor *& StarActor, FName & RowKeyName, int & Instance)
{
	TArray<AActor*> StarInterfaceActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UStarInterface::StaticClass(), StarInterfaceActors);
	Instance = -1;
	RowKeyName = "";

	for (auto a : StarInterfaceActors)
	{
		IStarInterface* StarInterface = Cast<IStarInterface>(a);
		if (StarInterface)
		{
			if (StarInterface->FindStarByStarName(StarName, RowKeyName, Instance))
			{
				StarActor = a;
				return true;
			}
		}
	}

	StarActor = nullptr;
	return false;
}

bool AStarManager::FindStarLocationByStarName(const FName StarName, FAstroDoubleVector & WorldLoc)
{
	int StarInst;
	AActor * StarActor;
	FName RowKeyName = "";
	FindStarByStarName(StarName, StarActor, RowKeyName, StarInst);

	if (StarActor)
	{
		IStarInterface* StarInterface = Cast<IStarInterface>(StarActor);
		if (StarInterface)
		{
			WorldLoc = StarInterface->GetStarWorldPosition(RowKeyName);
			return true;
		}
	}

	return false;
}


//Async Update----------------------------------------------------------------------------------
FUpdateStarsAsync::FUpdateStarsAsync(AStarManager * InObj, TMap<AActor*, FName> ActorsToUpdate)
	: StopTaskCounter(0)
{
	AstroHelperObj = InObj;
	UpdateActorArray = ActorsToUpdate;

	Thread = FRunnableThread::Create(this, TEXT("FUpdateStarsAsync"), 0, TPri_BelowNormal); 
}

FUpdateStarsAsync::~FUpdateStarsAsync()
{
	delete Thread;
	Thread = NULL;
}

bool FUpdateStarsAsync::Init()
{
	return true;
}

uint32 FUpdateStarsAsync::Run()
{
	FPlatformProcess::Sleep(0.02);

	auto It = UpdateActorArray.CreateConstIterator();
	while (StopTaskCounter.GetValue() == 0 && It)
	{
		
		AActor* StarPointer = It.Key();
		
		FVector NewPos = AstroHelperObj->CalculateStarPosition(It.Key(), AstroHelperObj->StarCalculationMethod, It.Value(),false).GetFVector() + AstroHelperObj->GetActorLocation();
		
		if (AstroHelperObj->AdjustForEclipticalPlane)
		{
			NewPos = NewPos.RotateAngleAxis(23.5, FVector(0.f, 1.f, 0.f));
		}

		NewPos = NewPos.RotateAngleAxis(AstroHelperObj->NorthDirection, FVector(0.f, 0.f, 1.f));

		AsyncTask(ENamedThreads::GameThread, [=]() {
			StarPointer->SetActorLocation(NewPos);
		});
		
		++It;
	}

	bIsFinished = true;
	
	return 0;
}

void FUpdateStarsAsync::Stop()
{
	StopTaskCounter.Increment();
}

FUpdateStarsAsync* FUpdateStarsAsync::AsyncInit(AStarManager * InObj, TMap<AActor*, FName> ActorsToUpdate)
{

	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FUpdateStarsAsync(InObj, ActorsToUpdate);
	}
	return Runnable;
}

void FUpdateStarsAsync::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FUpdateStarsAsync::Shutdown()
{

	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}

bool FUpdateStarsAsync::IsThreadFinished()
{
	if (Runnable)
	{
		return Runnable->IsFinished();
	}
	return true;
}
