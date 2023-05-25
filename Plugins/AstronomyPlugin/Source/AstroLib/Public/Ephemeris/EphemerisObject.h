// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AstroStructs.h"
#include "EphemerisObject.generated.h"


/**
 * 
 */


USTRUCT(BlueprintType)
struct FBodyIntMap
{
	GENERATED_BODY()

public:

	UPROPERTY()
	EBodys BodyName = EBodys::Unset;

	UPROPERTY()
	int integer = 0;
	
	FBodyIntMap()
	{

	}
	
	FBodyIntMap(EBodys InName, int inInt)
	{
		BodyName = InName;
		integer = inInt;
	}
	

};


UCLASS(abstract, Blueprintable, BlueprintType)
class ASTROLIB_API UEphemerisObject : public UObject
{
	GENERATED_BODY()

public:

	//Name of the ephemeris
	UPROPERTY()
	FString Name;

	UPROPERTY()
	EEphemeris EphemerisType;

	UPROPERTY()
	TArray<FBodyIntMap> BodyToIntArray;

	UFUNCTION(BlueprintCallable, Category="AstroLib")
	virtual int GetIntForBody(EBodys InBody);


protected:
	bool isInitialized;

public:

	/*
	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

	Initializes the ephemerisis. Optionally pass a filename if needed.
	@param DataFiles - (only used for JPLDE Ephemeris type) The path and filename to a binary JPL ephemeris file. Use "$GameContentDir" to reference the game's content directory. For packaging, add this directory to the "Additional Non-Asset Directories to Copy" package setting. A sample ephemeris file is included in in Plugins\AstroLib\AstronomyData\SPKData.
	@return Error code. 0 is success.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris" , meta = (AutoCreateRefTerm = "DataFiles"))
	virtual int InitEphemeris(TArray<FString> DataFiles ) { check(0 && "You must override this"); return 0;};
	
	/*
	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

	Closes and unloades ephemeris data from memory.
	@return Error code. 0 is success.
	*/	
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris")
	virtual int CloseEphemeris() { check(0 && "You must override this"); return 0; };

	/*
	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

		Advanced use only. Use "Calculate Planet Position" for most cases when calculating visible poistion from earth!
	
	Returns a velocity and position vector of the selected planet, using the active ephemeris object. Should return equatorial rectangular
	cordinates in AU or Kilometers referred to the ICRS. Some ephemeris types may not return values referenced in ITRS (VSOP87 uses J2000), but those differences
	may be ignored unless very high precision is required. 
	@param ObjBody - The object to get state data (position and velicity) for.
	@param ObjOrigin - The origin object for the reference frame. 
	@param TDBjd1 - The julian date to get the state data for (TDB)
	@param Optionaljd2 - Optionally split jd dates into two variables for higher precision, although unlikely to have a signifigant difference on most systems.
	@param CoordinatePlane - Plane for coordinates to be returned in.
	@param Location - Location vector for ObjBody in equatorial rectangular coordinates in AU units, referenced to ITRS
	@param Velocity - Velocity vector for ObjBody in equatorial rectangular coordinates in AU units, referenced to ITRS
	@return Error code. 0 is success.
	*/		
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris")
	virtual int GetObjectState(EBodys ObjBody, EBodys ObjOrigin, FAstroDouble TDBjd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, const EPlane CoordinatePlane = EPlane::Equatorial, const EEphemerisUnits OutUnit = EEphemerisUnits::AU) { check(0 && "You must override this"); return 0;};

	/*
	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

	Returns true if the ephemeris has been initialized.
	*/
	UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris")
	virtual bool IsInitialized() { check(0 && "You must override this"); return 0; };

	/*
	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

	Returns true if the ephemeris date range covers the provided date (FDateTime).
	@return Returns true if the ephemeris date range covers the provided date.
	*/
	UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris")
	virtual bool ContainsDataForDateFromFDateTime(FDateTime InDate) { check(0 && "You must override this"); return 0; };

	/*
	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

	Returns true if the ephemeris date range covers the provided date. (JulianDate)
	*/
	UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris")
	virtual bool ContainsDataForDateFromJD(FAstroDouble JulianDate) { check(0 && "You must override this"); return 0; };

	/*
	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

	Returns the starting julian date for the ephemeris
	*/
	UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris")
	virtual FAstroDouble StartDate() { check(0 && "You must override this"); return 0; };

	/*
	Base function must be overridden. Make sure to cast the pointer reference to the approproate ephemeris type.

	Returns the ending julian date for the ephemeris
	*/
	UFUNCTION(BlueprintPure, Category = "AstroLib|Ephemeris")
	virtual FAstroDouble EndDate() { check(0 && "You must override this"); return 0; };



};
