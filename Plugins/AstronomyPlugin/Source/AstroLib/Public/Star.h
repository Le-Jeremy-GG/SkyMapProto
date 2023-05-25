// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AstroStructs.h"
#include "StarInterface.h"
//#include "MyInterfaceTest.h"
#include "Star.generated.h"

//Class where this actor represents a single star.
UCLASS(Blueprintable, BlueprintType)
class AStar : public AActor, public IStarInterface
{
	GENERATED_BODY()
private:


public:	
	// Sets default values for this actor's properties
	AStar();




public:	

	UPROPERTY()
	FStarCatalogEntry StarCatEntry;

	UPROPERTY()
	FSkyPosition CelestialCoordinatesSkyPos;

	UPROPERTY(BlueprintReadOnly, Category = "AstroLib|Star")
	bool bCelestialCoordsSetAtLeastOnce = false;

	UPROPERTY()
	ECoordSystems CelestialCoordSystem;

	UPROPERTY(BlueprintReadOnly, Category = "AstroLib|Star")
	FDateTime CelestialCoordinatesDateTime;

	UPROPERTY()
	FAstroDouble Azimuth=0;
	UPROPERTY()
	FAstroDouble Elevation=0;
	
	UPROPERTY(BlueprintReadOnly, Category = "AstroLib|Star")
	bool bAzElCoordsSetAtLeastOnce = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "AstroLib|Star")
	EStarPlacementInWorld PlacementType;

	UPROPERTY()
	FAstroDoubleVector WorldPosition = FAstroDoubleVector(FVector(0,0,0));

	/* 
	Sets star catalog data for this star. Used to store data, does not affect actor placement.
	@param RowNameKey - The row name, for example, if being set from a datatable which uses the first column as the name key.
	@param InStarCatEntry - FStarCatalogEntry structure, Star catalog data.
	@return - Always true.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool SetStarCatalogData(FName RowNameKey, FStarCatalogEntry InStarCatEntry);

	/*
	Retrieves star catalog data for this star.
	@param RowNameKey - The row name key for the star to get star catalog data for. (Not required for actors with this baseclass that contain a single star)
	@param OutStarCatEntry - FStarCatalogEntry data for star
	@return - Always true.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool GetStarCatalogData(FStarCatalogEntry & OutStarCatEntry, FName RowNameKey = "");

	/*
	Sets celestial sky position data for this star.
	Celestial sky position is the star's position based on changes in time and depends on the ECoordSystems reference coordinate system.
	@param InSkyPosition - The celestial position.
	@param InTime - UTC Time InSkyPosition referes to.
	@param InCoordSystem Coord system that InSkyPosition is in (GCRS, TrueEquinox, True CIO, Astronomic).
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Always true
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool SetCelestialCoordinates(FSkyPosition InSkyPosition, FDateTime InTime, ECoordSystems InCoordSystem, FName RowNameKey ="" );

	/*
	Retrieves celestial sky position data for this star.
	Celestial sky position is the star's position based on changes in time and depends on the ECoordSystems reference coordinate system.
	@param OutSkyPosition - The celestial position.
	@param OutTime - UTC Time InSkyPosition referes to.
	@param OutCoordSystem Coord system that InSkyPosition is in (GCRS, TrueEquinox, True CIO, Astronomic).
	@param RowNameKey - The row name key for the star to get data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return true if celestial coords have been set at least once. False if coords were never set.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool GetCelestialCoordinates(FSkyPosition & OutSkyPosition, FDateTime &OutTime, ECoordSystems & OutCoordSystem, FName RowNameKey = "");

	/*
	Sets the position type that this actor is using to be placed in the world (ground or celestial).  Used to store data, does not affect actor placement.
	@param InType - The posion type.
	@return true if set successfully.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool SetStarPositionType(EStarPlacementInWorld InType) ;

	/*
	Retrieves the position type that this actor is using to be placed in the world (ground or celestial).
	@return Position type (Ground (horizon) or celestial)
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual EStarPlacementInWorld GetStarPositionType();
	
	/*
	Sets Azimuth and elevation. Used to store data, does not affect actor placement.
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@param InAzimuth - Azimuth in degrees.
	@param InElevation - Elevation in degrees.
	@return True set successfully.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool SetAzElCoordinates(FAstroDouble InAzimuth, FAstroDouble InElevation, FName RowNameKey = "");

	/*
	Retrieves Azimuth and elevation.
	@param OutAzimuth - Azimuth in degrees.
	@param OutElevation - Elevation in degrees.
	@param RowNameKey - The row name key for the star to get data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return True azimuth and elevation have been set at least once.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool GetAzElCoordinates(FAstroDouble &OutAzimuth, FAstroDouble &OutElevation, FName RowNameKey = "");
	
	/*
	Handles a new star to spawn. Sets star catalog data, and for instnaced static meshes, creates the star instance.
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@param InStarCatEntry - Star catalog data.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual void StarSpawn(FName RowNameKey, FStarCatalogEntry InStarCatEntry) ;


	/*
	Handles removing a star (but the actual Destroy() call should not be done from the interface)
	@param RowNameKey - The row name key for the star to remove. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Always true.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool RemoveStar(FName RowNameKey = "");

	/*
	Updates data when updating a star position. Changes instance locations when using instances static mesh, but will not call SetActorLocation.
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Returns false if this function is not used for an actor True if used and star is updated.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool UpdateStarPosition(FAstroDoubleVector NewWorldPosition, FName RowNameKey = "");

	/*
	Returns the stored star world position. (Set from UpdateStarPosition())
	@param RowNameKey - The row name key for the star to get data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return The star's world position. (Set with UpdateStarPosition).
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual FAstroDoubleVector GetStarWorldPosition(FName RowNameKey = "") { return WorldPosition; };

	/*
	Updates the star's scale size
	@param RowNameKey - The row name key for the star to set size for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Returns true if star found and scale updated.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool SetStarScale(FVector NewScale, FName RowNameKey = "");

	/*
	Updates the star's rotation
	@param RowNameKey - The row name key for the star to set rotation for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Returns true if star found and successfully set rotation.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool SetStarRotation(FRotator NewRotation, FName RowNameKey = "");

	/*
	Determins if this star actor contains a representation of the star specified by RowNameKey.
	@param Instance - If this is an instanced static mesh actor, this is the instance for the specified star.
	@param RowNameKey - The row name key for the star to get data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return True if this actor does contain this star.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool FindStarByRowName(FName RowNameKey, int & Instance);

	/*
	Determins if this star actor contains a representation of the star specified by a StarName.
	@param Instance - If this is an instanced static mesh actor, this is the instance for the specified star.
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return True if this actor does contain this star.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool FindStarByStarName(FName StarName, FName & StarRowNameKey, int & Instance);

	/*
	Retrieves an array of all StarRowKeys reppresented by this actor. If a single star is represented by a single actor,
	array will be size 1. If the actor contains many stars (i.e. instanced static mesh), then array will be greater than 1.
	@param StarRowLeyArray -  Array of StarRowKeys in this actor.
	@return Always true
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Star")
	virtual bool GetAllStarRowKeysInActor(TArray<FName> & StarRowKeyArray);

};

