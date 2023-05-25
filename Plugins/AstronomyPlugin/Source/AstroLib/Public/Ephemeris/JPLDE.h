// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EphemerisManager.h"
#include "SpiceUsr.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JPLDE.generated.h"


UCLASS()
class ASTROLIB_API UJPLDE : public UEphemerisObject
{
	GENERATED_BODY()

public: 

	void * EphLinkPointer;

	UJPLDE();
	~UJPLDE();


	public:
	FString MakeDataPaths(FString FilePath);

	double J2000ET;
	double SecondsPerDay;
	TArray<FString> LoadedKernalArray;


	virtual int InitEphemeris(TArray<FString> DataFiles) override;

	/*
	Initializes the ephemerisis. Must provide a path and filename to a binary JPL ephemeris files and leap second data. Use "$GameContentDir" to reference the game's content directory. For packaging, add this directory to the "Additional Non-Asset Directories to Copy" package setting.
	@param FilePath - The path and filename to a binary SPK file or other kernal type .Use "$GameContentDir" to reference the game's content directory. For packaging, add this directory to the "Additional Non-Asset Directories to Copy" package setting. A sample ephemeris file is included in in Plugins\AstroLib\AstronomyData\SPKData.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris|JPLDE", meta = (AutoCreateRefTerm = "DataFiles"))
	void LoadKernal(FString FilePath, FString & KernelInfo, bool LogKernelInfo = false);

	/*
	Returns an array listing all objects loaded into the JPLDE kernel, that match the tenplate pattern. 
	@param Template - Pattern to search for. Use '*' for string wildcard, and '%' for Character wildcard. Example: *earth* .
	*/
	UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris|JPLDE", meta = (AutoCreateRefTerm = "DataFiles"))
	TArray<FString> GetAllLoadedObjects(FString Template = "*");


	virtual int CloseEphemeris() override;


	virtual int GetObjectState(EBodys ObjBody, EBodys ObjOrigin, FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, const EPlane CoordinatePlane = EPlane::Equatorial, const EEphemerisUnits OutUnit = EEphemerisUnits::AU) override;

	//Checks if an error occurred in ephemeris processing library
	bool CheckError();

	/*
	Returns position and velocity data from the ephemeris.
	@param TargetID - ID code for target
	@param OriginID - ID code for origin body
	@return Error code. 0 is success.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris|JPLDE")
	int GetCustomObjectState(int TargetID, int OriginID, FAstroDouble TDBjd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, EPlane CoordinatePlane = EPlane::Equatorial, const EEphemerisUnits OutUnit = EEphemerisUnits::AU);
	
	/*
	Returns position and velocity data from the ephemeris.
	@param TargetID - ID code for target
	@param OriginID - ID code for origin body
	@return Error code. 0 is success.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris|JPLDE")
	int GetCustomObjectStateByName(FString TargetName, FString OriginID, FAstroDouble TDBjd1, FAstroDouble Optionaljd2, FAstroDoubleVector& Location, FAstroDoubleVector& Velocity, EPlane CoordinatePlane = EPlane::Equatorial, const EEphemerisUnits OutUnit = EEphemerisUnits::AU);


	/*
	Returns the axis and rotation for a planet or celestial body in J2000 reference frame.
	@param ObjBody - Object to get the rotation for
	@param TDBjd1 - TDB Time
	@return Error code. 0 is success.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris|JPLDE")
	int GetObjectOrientation(EBodys ObjBody, FAstroDouble TDBjd1, FVector& Axis, float& Rotation, const EPlane CoordinatePlane = EPlane::Equatorial);


	virtual bool IsInitialized() override;


	//virtual bool ContainsDataForDateFromJD(FAstroDouble JulianDate) override;


	//virtual bool ContainsDataForDateFromFDateTime(FDateTime InDate) override;


	/*
	Not implemented

	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

	Returns the starting julian date for the ephemeris
	@return Returns the starting julian date for the ephemeris
	*/
	//UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris|JPLDE")
	//FAstroDouble StartDate() override;

	/*
	Not implemented

	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

	Returns the ending julian date for the ephemeris
	@return Returns the ending julian date for the ephemeris
	*/
	//UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris|JPLDE")
	//FAstroDouble EndDate() override;

};


