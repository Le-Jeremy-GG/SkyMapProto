// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "Enumerations.generated.h"

template<typename T>
static FString GetEnumStr(const FString& EnumStr, const T EnumVal)
{
	UEnum* EnumObjRef = FindObject<UEnum>((UObject*)ANY_PACKAGE, *EnumStr);
	if (EnumObjRef)
	{
		return *EnumObjRef->GetNameStringByIndex(static_cast<uint8>(EnumVal));
	}
	else
	return FString("null");
}

const double DoublePI = acosl(-1.0L);
const double DEG2RAD = 0.017453292519943295769;
const double AUToKm = 149597870.700;
				      


UENUM(BlueprintType)
enum class EEphemeris : uint8
{
	//JPLDE
	JPLDE,
	//Low precision data based on kepler equations of motions for the planets.
	JPLLowPrecision,
	//VSOP87 data (no pluto or moon available)
	VSOP
};

//Full list of all solar system objects or places used by the library or ephemeris.
UENUM(BlueprintType)
enum class EBodys : uint8
{
	Unset,
	Mercury,
	Venus,
	Earth,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
	Neptune,
	Pluto,
	Sun,
	Moon,
	SolarSystemBarycenter,
	EarthMoonBaryCenter,
	Nutations,
	Librations,
	LunarMantle,
	TTTDBDiff,
	
};

//A list of major solar system objects used by the astro library.
UENUM(BlueprintType)
enum class ESolarSystemBodys : uint8
{
	Unset,
	Mercury,
	Venus,
	Earth,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
	Neptune,
	Pluto,
	Sun,
	Moon,
	SolarSystemBarycenter,
};

UENUM(BlueprintType)
enum class EPlane : uint8
{
	//Sun - Earth Plane
	Ecliptical,
	//Plane that aligns to earths equator.
	Equatorial,
	
};

UENUM(BlueprintType)
enum class EWhereIsObserver : uint8
{
	//Center of the earth
	Geocenter = 0,
	//On the earth surface
	EarthSurface = 1,
	//In space nearby earth (i.e. a satellite)
	NearEarthSpaceCraft = 2,
};

//Coordinate system selection (novas Coord_sys struct).
UENUM(BlueprintType)
enum class ECoordSystems : uint8
{
	// Geocentric Celestial Reference System. Replaces the geocentric system based on the mean equator and equinox of J2000.0.
	GCRS = 0, 
	//Equinox based method using earths instantanious orientation parameters. 
	TrueEquatorAndEquinoxOfDate = 1,
	//Celestial Intermediate Reference System  - CIO based method using earths instantanious orientation parameters. 
	TrueEquatorAndCIOOfDate = 2,
	//ICRS with light bending and abberation ignored.
	AstronomicPlace = 3,
	Unset =4,
};

UENUM(BlueprintType)
enum class ECelestialCoordSystems : uint8
{
	//Coordinate axes defined by earths precession of date.
	MeanEquatorAndEquinoxOfDate = 0,
	//Coordinate axes defined by earths precession and nutation of date.
	TrueEquatorAndEquinoxOfDate = 1,
	// Geocentric Celestial Reference System. Replaces the geocentric system based on the mean equator and equinox of J2000.0.
	GCRS = 2,
};

UENUM(BlueprintType)
enum class EPoleOffsetType : uint8
{
	ModeledPoleReferredtoMeanEclipticOfDate = 0,
	ModeledPoleVectorReferredtoGCRS = 1,
};

//novas types, a result of where the observer is and the coord system used
UENUM(BlueprintType)
enum class ECoordTypes : uint8
{
	//GCRS with observer at Geocenter
	VirtualOrProperPlace,
	//GCRS with observer on earth surface
	LocalPlace,
	//True equator and equinox of date with observer at Geocenter
	ApparentPlace,
	//True equator and equinox of date with observer on earth surface
	TopocentricPlace,
	//Celestial Intermediate Reference System with observer at Geocenter
	IntermediatePlace,
	//true equator and CIO of date with observer on earth surface
	TopocentricIntermediatePlace,
	//astrometric coordinates (no light bending or abberation) with observer at Geocenter
	AstronomicPlace,
	//astrometric coordinates (no light bending or abberation) with observer on earths surface
	TopocentricAstronomicPlace,
	Unset,

};

//Mean or apparent Sidereal time
UENUM(BlueprintType)
enum class ESideRealTime : uint8
{
	//Mean sidereal time (no nutation)
	MeanSiderealTime = 0, 
	//Apparent sidereal time
	ApparentSideRealTime = 1,
};

//Calculation method
UENUM(BlueprintType)
enum class ECalculationMethod : uint8
{
	//CIO-based method
	CIO = 0, 
	//Equinox - based method
	Equinox = 1,
};

//Ter2Cel options
UENUM(BlueprintType)
enum class ETer2CelOptions : uint8
{
	GCRS = 0,
	EquatorAndEquinoxOfDate = 1,
};

//Ter2Hor options
UENUM(BlueprintType)
enum class ETer2HorRefractionOptions : uint8
{
	//No refraction correction will be performed.
	NoRefractionCorrection = 0,
	//Standard refraction values for an average spot on earth.
	RefractionCorrectionUsingStandardVals = 1,
	//Values will be obtained from earth location structure.
	RefractionCorrectionUsingEarthLocationVals = 2,
};

//transform_cat options
UENUM(BlueprintType)
enum class ETransformCoordOptions : uint8
{
	//The star’s data is updated to account for the star’s space motion between the first and second dates, within a fixed reference system.
	ChangeEpochSameEquatorAndEquinox = 0,
	//The reference frame within which the star’s coordinates and proper motion are expressed is rotated corresponding to precession between the firstand second dates.
	ChangeEquatorAndEquinoxSameEpoch = 1,
	//MOST COMMON. Epoch, Equator, and Equinox transformed.
	ChangeEquatorEquinoxandEpoch = 2,
	// convert catalog quantities from the mean equatorand equinox of J2000.0 (the J2000.0 system) to the ICRS.
	ChangeJ2000toICRS=3,
	//Convert catalog quantities from J2000.0 to the ICRS.
	ChangeICRStoJ2000=4,
};


//transform_cat options
UENUM(BlueprintType)
enum class EStarUpdateOptions : uint8
{
	//Fully updates the star's celestial and horizon position. Very accurate but very slow.
	Full,
	//Calculates earth rotation parameters and rotates the AstroLibHelper actor based on those values, which will rotate all stars with the AstroLibHelper actor as a parent. Fast but less accurate because individual star locations are not being calculated.
	FastUpdate,
	//ADVANCED USE ONLY. Only updates the star's celestial position in the selected coordinate system. This will not change the star's location, it will ONLY update the star's internal Sky Position data.
	OnlyCelestial,
	//Only updates the horizon azimuth and elevation (based on the observer lat, long, time, and refraction options if selected). Will first attempt to access the star interface GetCelestialCoordinates() function to recieve celestial coordinate data. If none exists, will generate it.
	OnlyHorizon,
};

//Units for ephemeris
UENUM(BlueprintType)
enum class EEphemerisUnits : uint8
{
	AU = 0,
	Kilometers = 1,
};


