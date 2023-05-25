// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Star.h"
#include "GameFramework/Actor.h"
#include "Core/Private/HAL/PThreadRunnableThread.h"
#include "Core/Public/Async/Async.h"
#include "Ephemeris/EphemerisObject.h"
#include "AstroLibBPLibrary.h"
#include "StarManager.generated.h"


//Structure to describe which star actor to spawn based on visual magnitude (So you can spawn different blueprints based on the star brightness).
USTRUCT(BlueprintType)
struct FStarActorToSpawnTable 
{
	GENERATED_BODY()

public:

	//The actor class to spawn
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
	TSubclassOf<AActor> StarClassToSpawn;

	//Max visual magnitude that this actor will spawn for (lower number number is brighter)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager", meta = (MakeStructureDefaultValue = "1000"))
	float MaxVisualMag = 1000;
	
	//Min visual magnitude that this actor will spawn for (lower number number is brighter)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager", meta = (MakeStructureDefaultValue = "-1000"))
	float MinVisualMag = -1000;



};

//Structure to describe how to size the star based on the star brightness (so brighter stars will spawn larger).
USTRUCT(BlueprintType)
struct FStarSizeTable
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
		FVector StarSizeMultiplier = FVector(1,1,1);

	//Max visual magnitude that this actor will spawn for (lower number number is brighter)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager", meta = (MakeStructureDefaultValue = "1000"))
		float MaxVisualMag = 1000;

	//Min visual magnitude that this actor will spawn for (lower number number is brighter)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager", meta = (MakeStructureDefaultValue = "-1000"))
		float MinVisualMag = -1000;



};




//The structure for a datatable row to import a star catalog. Includes the same fields as FStarCatalogEntry (except for the namekey), but this needs to extend from FTableRowBase in order to import to a datatable.
USTRUCT(BlueprintType)
struct FStarDataEntryRowBase : public FTableRowBase
{
	GENERATED_BODY()

public:

	//Name of celestial object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	FString StarNameFString;

	//Catalog designator / Name(e.g., HIP)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	FString CatalogFString;
	
	//Integer identifier assigned to object
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	int StarNumberInt = 0;

	//ICRS right ascension(hours)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	FString RightAscenstion;

	//ICRS declination(degrees)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	FString Declination;

	//ICRS proper motion in right ascension(milliarcseconds / year)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	FString ProperMotionInRightAscension;

	//ICRS proper motion in declination (milliarcseconds / year)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	FString ProperMotionInDeclination;

	//promodec = parallax(milliarcseconds) 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	FString Parallax;

	//radial velocity(km / s)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	FString RadialVelocity;

	//Visual magnitude
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Celestial Coordinate")
	FString OptionalVisualMagnitude;


	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) 
	{

		for (auto it : InDataTable->GetRowMap())
		{
			FStarDataEntryRowBase* data = (FStarDataEntryRowBase*)(it.Value);
			// it.Key has the key from first column of the CSV file
			// it.Value has a pointer to a struct of data. You can safely cast it to your actual type, e.g FMyStruct* data = (FMyStruct*)(it.Value);
	
			if (!(data->RightAscenstion.IsNumeric() && data->Declination.IsNumeric() && data->ProperMotionInRightAscension.IsNumeric() && data->ProperMotionInDeclination.IsNumeric() && data->Parallax.IsNumeric() && data->RadialVelocity.IsNumeric()))
			{
				OutCollectedImportProblems.AddUnique("Warning: Non-numeric value in row name: " + FString(it.Key.ToString()));
			}
			
			

		}
	}


};


UCLASS()
class ASTROLIB_API AStarManager : public AActor
{
	GENERATED_BODY()
	
public:	

	
	//Star catalog data used to generate stars. DataTable must be importated with FStarDataEntryRowBase row base for valid header row.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	UDataTable * StarCatalogData;

	//The UTC time to use for star positioning calculations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	FDateTime UTCTime = FDateTime::Now();

	//The observer location to use for star positioning calculations. The fields used will depend on the RequestedCoordSystem and StarUpdateMethod. For example, SurfaceLocation data is only required if generating star data relative to a position on earth (for example, topocentric or horizon coordinates).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	FObserver Observer;

	//The angle in degrees to adjust the north direction. Clockwise from the positive x vector.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	float NorthDirection = 0.f;

	//True if placing stars in a solar system space scene that is set in the ecliptical plane. Applies a rotation of 22.5 degrees along the Pitch axis. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	bool AdjustForEclipticalPlane = false;

	//Light refraction adjustment options to use when computing horison coordinates.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	ETer2HorRefractionOptions RefractionOpts = ETer2HorRefractionOptions::NoRefractionCorrection;

	//Coord system to use when calculating star positions. Use TrueEquatorAndCIOOfDate in most situations.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	ECoordSystems RequestedCoordSystem = ECoordSystems::TrueEquatorAndCIOOfDate;

	//Describes which class to use for a Star, based on visual magnitude. See function GetStarClassToSpawn.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
	TArray<FStarActorToSpawnTable> StarClassTable;

	//Describes how to size the star actor, based on visual magnitude. See function GetStarClassToSpawn.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
	TArray<FStarSizeTable> StarSizeTable;

	//Default class to use for a Star that will be used if StarClassTable is empty.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
	TSubclassOf<AActor> DefaultStarClassToSpawn;
	
	//Distance away from this AstroLibHelper to spawn the star.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
	float SpawnDistance = 1000000.f;
	
	//A map (actor pointer, FName) of the stars created from this helper. The FName is the Namekey imported from the FTableRowBase struct used for each row in the StarCatalogData DataTable.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
	TMap<AActor*, FName> SpawnedStarActorMap;

	//Ephemeris object to use for star placement calculations.
	UPROPERTY(BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	UEphemerisObject * Ephem;

	//The default ephemeris to use if no specific ephemeris is already initialized.
	UPROPERTY(BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	EEphemeris DefaultEphem = EEphemeris::VSOP;

	//The method to use when calculating star positions.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
	EStarUpdateOptions StarCalculationMethod = EStarUpdateOptions::Full;

	//True if earth axes and ENU vectors have been set at least once. Used when using Fast Update StarCalculationMethod.
	UPROPERTY(BlueprintReadOnly, Category = "Astronomy Astro StarManager")
	bool bAxesAndENUVectorsSetAtLeastOnce = false;
	
	//Delta_T to use for calculations.
	UPROPERTY(BlueprintReadOnly, Category = "Astronomy Astro StarManager")
	float delta_t = 0;

	//Update delta_t when calculating star positions with fast update. delta_t has a small affect in accuracy, when using fast update updating delta_t very often is not useful.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
	bool bAlwaysUpdateDeltaT = false;

	//Updates the observer's position each time calculating star positions with fast update. Leave off if the observer is not moving to improve performance, turn on if fast update needs to consider update to the observer's position.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Astronomy Astro StarManager")
	bool bAlwaysUpdateEarthSurfaceLocation = true;

	//East, North, Up Vectors. Set in SetENUVectorsInECEF()
	double ECEFNorth[3];
	double ECEFEast[3];
	double ECEFUp[3];

	//Tracks if stars have been rotated to be horizon (earth perspective). If they have been, FastUpdate will need to 'reset' them back to celestial coordinated before applying earth rotations.
	UPROPERTY(BlueprintReadOnly, Category = "Astronomy Astro StarManager")
	bool StarsSetToHorizon = true;

	//Terresterial X axis in celestial coords
	UPROPERTY(BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	FAstroDoubleVector TerrestrialXAxisInCelestialSpace;
	//Terresterial X axis in celestial coords
	UPROPERTY(BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	FAstroDoubleVector TerrestrialYAxisInCelestialSpace;
	//Terresterial X axis in celestial coords
	UPROPERTY(BlueprintReadWrite, Category = "Astronomy Astro StarManager")
	FAstroDoubleVector TerrestrialZAxisInCelestialSpace;


public:	

	AStarManager();

	//Used to make sure we have an ephemereis
	virtual void PostInitializeComponents();

	/*
	Updates the earth/celestial axes transformation and the ENU earth surface location parameters for the Fast Update star calculation method.
	Advanced use only, turn on bAlwaysUpdateEarthSurfaceLocation to update earth surface location for each Fast Update calculation.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update Surface Location And CelestialAxes", Keywords = "Star "), Category = "Astronomy Astro StarManager")
	void UpdateSurfaceLocationAndCelestialAxes();

	/*
	Updates the earth/celestial axes transformation for the Fast Update star calculation method. Advanced use only.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Celestial Axes", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	void SetCelestialAxes();

	/*
	Updates the the ENU (East North Up) vectors based on the earth surface location parameters for the Fast Update star calculation method. Advanced use only.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetENUVectorsInECEF", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	void SetENUVectorsInECEF();

	/*
	Spawns a star for every entry in the StarCatalogData datatable.
	@param World - The world to spawn the stars.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Spawn Stars", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	void SpawnStars(UWorld* World);

	/*
	Spawns a single star (see function GetStarClassToSpawn()) 
	@param CatEntryData - A struct (type FStarCatalogEntry), containing the celestial information of the star.
	@param RowNameKey - Optional, Key from datatable import. Used if actor contains multiple 'stars' (i.e instanced static meshes) or used if the actor does not implemement StarInterface - will use the FName to try to look up the star catalog data from the DataTable directly instead of using the StarInterface.
	@param World - The world to spawn the stars.
	@return Pointer to the new star actor
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Spawn Single Star", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	AActor * SpawnStar(FName RowNameKey, FStarCatalogEntry CatEntryData, UWorld * World);

	/*
	Checks the world to see if a new actor star is needed or not. Returns the existing actor to use, or null if need to spawn a new actor. 
	Needed because instanced static meshes do not need additional actors to spawn
	@return The actor in the world to add the star to. Null if no Class is found, and will need to spawn a new actor.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Check If Need to Spawn Star", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	AActor * GetActorToUse(TSubclassOf<AActor> StarClass, UWorld * World);


	/*
	Returns which star class should be spawned based on the star data in CatEntryData. 
	@param FStarCatalogEntry - The catalog data of the star to be spawned.
	@return The Actor subclass to spawn as a star.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetStarClassToSpawn", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	virtual TSubclassOf<AActor> GetStarClassToSpawn(FStarCatalogEntry CatEntryData);

	/*
	Called after a star is spawned, to change the new star's size for example.
	@param RowNameKey - The row key name to use for SpawnedStarActorMap. If from a datatable, this is usually the first column in the datatable.
	@return Always true.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PostStarSpawn", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	virtual bool PostStarSpawn(AActor * SpawnedStar, FStarCatalogEntry CatEntryDataFName, FName RowNameKey="");

	/*
	Updates the star's position, and updates the star's StarInterface data if implemented. If using FastUpdate, use UpdateAllStarPositions first to update stars, then call this to update star data. FastUpdate will update the star's interface with azimuth and elevation data, but will not move the star's position because the position is based on StarManager rotation. 
	@param Star - Pointer to star actor (actor that implements StarInterface to update.
	@param RowNameKey - Optional, Key from datatable import. Required if actor contains multiple 'stars' (i.e instanced static meshes) or used if the actor does not implemement StarInterface - will use the FName to try to look up the star catalog data from the DataTable directly instead of using the StarInterface.
	@param UpdateOptions - The StarCalculationMethod to use. FastUpdate will update the star's interface with azimuth and elevation data, but will not move the star's position. Use UpdateAllStarPositions to update stars with fast update.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update Star Position", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	void UpdateStarPosition(AActor * Star, FName RowNameKey, EStarUpdateOptions UpdateOptions = EStarUpdateOptions::Full);

	/*
	Updates all stars' positions, and updates the star's StarInterface if implemented. FastUpdate will NOT update the star's interface with azimuth and elevation data, but will not move the star's position. 
	@param UpdateOptions - The StarCalculationMethod to use. FastUpdate will update the star's interface with azimuth and elevation data, but will not move the star's position. Use UpdateStarPosition to update a stars interface data.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update All Star Positions", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	void UpdateAllStarPositions(EStarUpdateOptions UpdateOptions = EStarUpdateOptions::Full);

	/*
	Updates each star in InActorMap asynchrinously (seperate from game thread). Not valid for use with FastUpdate calculation method.
	@param InActorMap - Map (Actor pointer, FName Key) of the stars to update. The FName of the map may be blank, it is only used if the actor does not implemement StarInterface - it will use the FName to try to look up the star catalog data from the datatable directly instead of using the StarInterface.
	@param UpdateOptions - The StarCalculationMethod to use.  FastUpdate not valid.
	@return Returns true if a new process was kicked off, false if no process was kicked off. Only one update process will be created at a time, so if a previous running of an async star update is still running a new thread will not be created.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update Star Positions Async", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	bool UpdateStarPositionsAsync(TMap<AActor*, FName> InActorMap, EStarUpdateOptions UpdateOptions = EStarUpdateOptions::Full);

	/*
	Updates all stars asynchrinously (seperate from game thread). Not valid for use with FastUpdate calculation method.
	@param UpdateOptions - The StarCalculationMethod to use. FastUpdate not valid.
	@return Returns true if a new process was kicked off, false if no process was kicked off. Only one update process will be created at a time, so if a previous running of an async star update is still running a new thread will not be created.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update All Star Positions Async", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	bool UpdateAllStarPositionsAsync(EStarUpdateOptions UpdateOptions = EStarUpdateOptions::Full);

	/*
	Calculates and returns star's position based on the StarManager Time/Observer/CoordSystem properties. 
	@param Star - Star actor to preform the calculation for.
	@param UpdateOptions - The StarCalculationMethod to use. FastUpdate will not return a valid vector because FastUpdate does not update individual stars. Calling with Fast update option can be used to update the StarInterface Azimiuth and elevation.
	@param RowNameKey - Optional, Key from datatable import. Used if actor contains multiple 'stars' (i.e instanced static meshes) or used if the actor does not implemement StarInterface - will use the FName to try to look up the star catalog data from the DataTable directly instead of using the StarInterface.
	@param AttemptToUpdateStarInterface - If true, will update the star's StarInterface with updated data resulting from the calculation.
	@return FAstroDoubleVector of the star's position.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Calculate Star Position", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	FAstroDoubleVector CalculateStarPosition(AActor * Star, EStarUpdateOptions UpdateOptions = EStarUpdateOptions::Full, FName RowNameKey="", bool AttemptToUpdateStarInterface = true);


	/*
	*Attempts to get the star's star catalog position first by checking to see if the star actor implements 'StarInterface/GetStarData(). 
	If that fails, will try to find the stars catalog entry in the datatable by its row key.
	@param InActorPtr - Star actor to get the star catalog data for.
	@param RowNameKey - Optional, Key from datatable import. Used if actor contains multiple 'stars' (i.e instanced static meshes) or used if the actor does not implemement StarInterface - will use the FName to try to look up the star catalog data from the DataTable directly instead of using the StarInterface.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Star Catalog Position", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	FStarCatalogEntry GetStarCatalogPosition(AActor * InActorPtr, FName RowNameKey="");

	/*
	Removes and unloads all star actors associated with the StarManager actor  (using the actors SpawnedStarActorMap variable as a star reference).
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove All Stars", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	void RemoveAllStars();

	/*
	Removes a single star.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Remove Single Star", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	void RemoveStar(AActor * ActorToRemove, FName RowNameKey = "");

	/*
	Generates a FStarCatalogEntry structure from a FStarDataEntryRowBase row structure (which is used to import from a DataTable).
	This function is called to set a star's FStarCatalogEntry using the StarInterface when spawning a star for easier retrieval.
	@param DataRow - FStarDataEntryRowBase of the data to make a FStarCatalogEntry structure for. This is usually a row from a DataTable.
	@param RowNameKey - Optional, Key from datatable import. Used if actor contains multiple 'stars' (i.e instanced static meshes) or used if the actor does not implemement StarInterface - will use the FName to try to look up the star catalog data from the DataTable directly instead of using the StarInterface.
	@return FStarCatalogEntry structure.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MakeFStarCatalogEntryFromDataRow", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	virtual FStarCatalogEntry MakeFStarCatalogEntryFromDataRow(FStarDataEntryRowBase DataRow, FName RowNameKey);
	
	/*
	Finds a pointer to the star actor represented by RowNameKey by iterating through all actors that implement StarInterface and checking which stars those actors contain.
	@param RowNameKey - Star Key from datatable import to find
	@param StarActor - Pointer to actor containing the star.
	@param Instance - If an actor contains many stars (like an instanced static mesh), instance is the instance number of the star. Returns -1 if not an instanced star actor.
	@return True if star is found.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "FindStarByRowNameKey", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	virtual bool FindStarByRowNameKey(const FName RowNameKey, AActor * &StarActor, int & Instance);

	/*
	Finds the unreal world location of the star actor represented by RowNameKey by iterating through all actors that implement StarInterface.
	@param RowNameKey - Star Key from datatable import to find
	@param WorldLoc - World locaiton of star.
	@return True if star is found.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "FindStarLocationByRowNameKey", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	virtual bool FindStarLocationByRowNameKey(const FName RowNameKey, FAstroDoubleVector & WorldLoc);

	/*
	Finds a pointer to the star actor represented by RowNameKey by iterating through all actors that implement StarInterface and checking which stars those actors contain.
	@param StarName - The Star Name to search for. 
	@param StarActor - Pointer to actor containing the star.
	@param Instance - If an actor contains many stars (like an instanced static mesh), instance is the instance number of the star. Returns -1 if not an instanced star actor.
	@return True if star is found.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "FindStarByStarName", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	virtual bool FindStarByStarName(const FName StarName, AActor * &StarActor, FName & RowKeyName, int & Instance);

	/*
	Finds the unreal world location of the star actor represented by RowNameKey by iterating through all actors that implement StarInterface.
	@param StarName - The Star Name to search for. 
	@param WorldLoc - World locaiton of star.
	@return True if star is found.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "FindStarLocationByStarName", Keywords = "Astronomy Astro StarManager"), Category = "Astronomy Astro StarManager")
	virtual bool FindStarLocationByStarName(const FName StarName, FAstroDoubleVector & WorldLoc);


	
};





//~~~~~ Multi Threading ~~~
class FUpdateStarsAsync : public FRunnable
{
public: 

	static  FUpdateStarsAsync * Runnable;

	FRunnableThread* Thread;

	TMap<AActor*, FName> UpdateActorArray;

	FThreadSafeCounter StopTaskCounter;

	AStarManager * AstroHelperObj;

	bool bIsFinished = false;

	bool IsFinished() const
	{
		return bIsFinished;
	}

	FUpdateStarsAsync(AStarManager * InObj, TMap<AActor*, FName> ActorsToUpdate);
	virtual ~FUpdateStarsAsync();

	// FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	// FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	static FUpdateStarsAsync* AsyncInit(AStarManager * InObj, TMap<AActor*, FName> ActorsToUpdate);

	static void Shutdown();

	static bool IsThreadFinished();

};