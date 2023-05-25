// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EphemerisObject.h"
#include "EphemerisManager.generated.h"


/*
This Ephemeris manager class controls how the library or user requests ephemeris. Use this class to create an Ephemeris object, which is the specific type of ephemeris and data that will be used.
*/
UCLASS()
class ASTROLIB_API UEphemerisManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UEphemerisManager(const FObjectInitializer& ObjectInitializer);
	~UEphemerisManager();

	/*
	This is a reference to the active ephemeris object. This is the ephemeris that will be used for library calculations.
	*/
	static UEphemerisObject * ActiveEphem;
	
	/*
	Stores a single globally used ephemeris that is used if user never creates their own ephem.
	*/
	static UEphemerisObject * DefaultGlobalEphem;
	static EEphemeris DefaultEphemType;
	

	/*
	Creates a new ephemeris object. After a new ephemeris object is created, call InitEphemeris on the object to ensure any required initilization is done.
	@param Ephem - The type of ephemeris object to create. EEphemeris enum.
	@return A pointer to the emphemeris object that was created. Cast this pointer to the approproate ephemeris object class before use.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Create New Ephemeris", Keywords = "Create new ephemeris"), Category = "AstroLib")
	static UEphemerisObject *  CreateNewEphemeris(EEphemeris Ephem, UObject * Outer = nullptr);
	
	/*
	Sets the specific ephemeris object to use when astronomical calculations are required by the library.
	@param InEphObj - Pointer to the ephemeris object to set as active, so that it will be used when ephemeris is requested.
	@return Error code. 0 is success.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Active Ephemeris", Keywords = "Active Ephemeris"), Category = "AstroLib")
	static int SetActiveEphemeris(UEphemerisObject * InEphObj);

	/*
	Gets a reference to the specific ephemeris object currently in use when astronomical calculations are required by the library.
	@return A pointer to the active ephemeris object that will be used when ephemeris is requested.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Active Ephemeris", Keywords = "get Active Ephemeris"), Category = "AstroLib")
	static UEphemerisObject * GetActiveEphemeris();

	/*
	Advanced use only. Use "Calculate Planet Position" for most cases when calculating visible poistion from earth!
	
	Returns a velocity and position vector of the selected planet, using the active ephemeris object. Should return equatorial rectangular
	cordinates in AU referred to the ICRS. Some ephemeris types may not return values referenced in ITRS (VSOP87 uses J2000), but those differences
	may be ignored unless very high precision is required. 
	@param ObjBody - The object to get state data (position and velicity) for.
	@param ObjOrigin - The origin object for the reference frame. 
	@param jd1 - The julian date to get the state data for
	@param Optionaljd2 - Optionally split jd dates into two variables for higher precision, although unlikely to have a signifigant difference on most systems.
	@param CoordinatePlane - Plane for coordinates to be returned in.
	@param Location - Location vector for ObjBody in equatorial rectangular coordinates in AU units, referenced to ITRS
	@param Velocity - Velocity vector for ObjBody in equatorial rectangular coordinates in AU units, referenced to ITRS
	@return Error code. 0 is success.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Planet State", Keywords = "planet velocity position"), Category = "AstroLib|Ephemeris")
	static int GetState(EBodys ObjBody, EBodys ObjOrigin, FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, const EPlane CoordinatePlane = EPlane::Equatorial, const EEphemerisUnits OutUnit = EEphemerisUnits::AU);
	
	/*
	Closes and unloads the active ephemeris object.
	@param EphemObjToClose - Ephemeris object to close and unload.
	*/
	UFUNCTION(BlueprintCallable, Category = "AstroLib|Ephemeris")
	static void CloseEphemeris(UEphemerisObject * EphemObjToClose);

	// Converts an ESolarSystemBody enumeration (planets, sun, and moon) to an EBodys enumeration (all ephemeris data available)
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToEBodys (ESolarSystemBodys)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast), Category = "AstroLib|Conversions")
	static EBodys Conv_ESolarSystemBodyToEBodys(ESolarSystemBodys InSolarSysytemBody);


};


