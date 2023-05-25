// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "AstroStructs.h"
#include "UObject/Interface.h"
#include "StarInterface.generated.h"


UENUM(BlueprintType)
enum class EStarPlacementInWorld : uint8
{
	//Earth Ground perspective
	HorizonAzEl,
	//Celestial perspective
	Celestial,
	Unset,
	
};


// This class does not need to be modified.
UINTERFACE(Blueprintable, BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
class ASTROLIB_API UStarInterface : public UInterface
{
	GENERATED_BODY()


};

class ASTROLIB_API IStarInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	

	/* 
	Sets star catalog data for this star. Used to store data, does not affect actor placement.
	@param RowNameKey - The row name key to track the star by. For example, a datatable which uses the first column as the name key. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@param InStarCatEntry - Star catalog data. FStarCatalogEntry structure.
	@return - Always true.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool SetStarCatalogData(FName RowNameKey, FStarCatalogEntry InStarCatEntry) { return false; };

	/* 
	Retrieves star catalog data for a star contained in this actor.
	@param RowNameKey - The row name key for the star to get star catalog data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@param OutStarCatEntry - FStarCatalogEntry data for star
	@return - True if has data to return
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool GetStarCatalogData(FStarCatalogEntry & OutStarCatEntry, FName RowNameKey = "") { return false; };
	
	/* 
	Sets celestial sky position data for this star. 
	Celestial sky position is the star's position based on changes in time and depends on the ECoordSystems reference coordinate system.
	@param InSkyPosition - The celestial position.
	@param InTime - UTC Time InSkyPosition referes to.
	@param InCoordSystem Coord system that InSkyPosition is in (GCRS, TrueEquinox, True CIO, Astronomic).
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Always true
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool SetCelestialCoordinates(FSkyPosition InSkyPosition, FDateTime InTime, ECoordSystems InCoordSystem, FName RowNameKey = "") { return false; };
	
	/* 
	Retrieves celestial sky position data for this star.
	Celestial sky position is the star's position based on changes in time and depends on the ECoordSystems reference coordinate system.
	@param OutSkyPosition - The celestial position.
	@param OutTime - UTC Time InSkyPosition referes to.
	@param OutCoordSystem Coord system that InSkyPosition is in (GCRS, TrueEquinox, True CIO, Astronomic).
	@param RowNameKey - The row name key for the star to get data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return true if celestial coords have been set at least once. False if coords were never set.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool GetCelestialCoordinates(FSkyPosition & OutSkyPosition, FDateTime &OutTime, ECoordSystems & OutCoordSystem, FName RowNameKey = "") { return false; };

	/*
	Sets the position type that this actor is using to be placed in the world (ground or celestial).  Used to store data, does not affect actor placement.
	@param InType - The posion type.
	@return true if set successfully.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool SetStarPositionType(EStarPlacementInWorld InType) { return false; };
	
	/* 
	Retrieves the position type that this actor is using to be placed in the world (ground or celestial).
	@return Position type (Ground (horizon) or celestial)
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual EStarPlacementInWorld GetStarPositionType() { return EStarPlacementInWorld::Unset; };

	/* 
	Sets Azimuth and elevation. Used to store data, does not affect actor placement.
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@param InAzimuth - Azimuth in degrees.
	@param InElevation - Elevation in degrees.
	@return True set successfully.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool SetAzElCoordinates(FAstroDouble InAzimuth, FAstroDouble InElevation, FName RowNameKey = "") { return false; };

	/* 
	Retrieves Azimuth and elevation. 
	@param OutAzimuth - Azimuth in degrees.
	@param OutElevation - Elevation in degrees.
	@param RowNameKey - The row name key for the star to get data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return True azimuth and elevation have been set at least once.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool GetAzElCoordinates(FAstroDouble &OutAzimuth, FAstroDouble &OutElevation, FName RowNameKey = "") { return false; };

	/* 
	Handles a new star to spawn. Sets star catalog data, and for instnaced static meshes, creates the star instance.
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@param InStarCatEntry - Star catalog data.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual void StarSpawn(FName RowNameKey, FStarCatalogEntry InStarCatEntry) { return; };

	
	/*
	Identifies if actor uses instanced static mesh. (Or if this actor contains multiple stars, therefore, star RowNameKeys will be used.)
	@return True if uses instanced static mesh.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool UseInstancedStaticMesh() { return false; };

	/* 
	Returns the static mesh instance index of the star with RowNameKey. Only used if using instanced static mesh.
	@param RowNameKey - The row name key for the star to get an index for. 
	@return Static mesh instance index of the star with RowNameKey. Returns -1 if not found or not using instanced static mesh.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual int FindIndexByRowNameKey(FName RowNameKey) { return -1; };

	/* 
	Updates data when removing a star (but the actual Destroy() call should not be done from the interface)
	@param RowNameKey - The row name key for the star to remove. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Always true.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool RemoveStar(FName RowNameKey = "") { return false; };

	/* 
	Updates data when updating a star position. Changes instance locations when using instances static mesh.
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Returns false if this function is not used for an actor True if used and star is updated.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool UpdateStarPosition(FAstroDoubleVector NewWorldPosition, FName RowNameKey = "") { return false; };

	/* 
	Returns the stored star world position. (Set from UpdateStarPosition())
	@param RowNameKey - The row name key for the star to get data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return The star's world position. (Set with UpdateStarPosition).
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual FAstroDoubleVector GetStarWorldPosition(FName RowNameKey = "") { return FAstroDoubleVector(FVector(0,0,0)); };

	/* 
	Updates the star's scale size
	@param RowNameKey - The row name key for the star to set size for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Returns true if star found and scale updated.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool SetStarScale(FVector NewScale, FName RowNameKey = "") { return false; };

	/* 
	Updates the star's rotation
	@param RowNameKey - The row name key for the star to set rotation for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return Returns true if star found and successfully set rotation.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool SetStarRotation(FRotator NewRotation, FName RowNameKey = "") { return false; };

	/* 
	Determins if this star actor contains a representation of the star specified by RowNameKey.
	@param Instance If this is an instanced static mesh actor, this is the instance for the specified star.
	@param RowNameKey - The row name key for the star to get data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return True if this actor does contain this star.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool FindStarByRowName(FName RowNameKey, int & Instance) { return false; };

	/* 
	Determins if this star actor contains a representation of the star specified by a StarName.
	@param Instance - If this is an instanced static mesh actor, this is the instance for the specified star.
	@param RowNameKey - The row name key for the star to set data for. Used if actor has multiple 'stars' to track (i.e. instanced static meshes)
	@return True if this actor does contain this star.
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool FindStarByStarName(FName StarName, FName & StarRowNameKey, int & Instance) { return false; };

	/* 
	Retrieves an array of all StarRowKeys reppresented by this actor. If a single star is represented by a single actor,
	array will be size 1. If the actor contains many stars (i.e. instanced static mesh), then array will be greater than 1.
	@param StarRowLeyArray -  Array of StarRowKeys in this actor.
	@return Always true
	*/
	UFUNCTION(BlueprintCallable, Category = "Astronomy Star")
	virtual bool GetAllStarRowKeysInActor(TArray<FName> & StarRowKeyArray) { return false; };


};
