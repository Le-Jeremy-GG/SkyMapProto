// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "Enumerations.h"
#include "AstroStructs.h"
#include "NovasCFunctions.h"
#include "AstroLibBPLibrary.generated.h"



UCLASS()
class UAstroLibBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	/*
	Returns the number of leap seconds added by the date
	* @param UTCTime - UTC Time 
	* @return Number of leap seconds added to UTC Time at InTime
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Number Of Leap Seconds", Keywords = "Astronomy leap seconds time"), Category = "AstroLib")
	static int GetNumberOfLeapSeconds(FDateTime UTCTime);

	//returns the Novas Int used to represent the ESolarSystemBody
	static int GetNovasIntFromSolarSystemBodys(ESolarSystemBodys InBody);

	//returns the EBody represented by the integer in Novas
	static EBodys GetBodyFromNovasInt(int InInt);

	/*
	Returns the Julian Date format from an FDateTime.
	* @param DateTime - The FDateTime
	* @return The Julian Date format
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Julian Date", Keywords = "Julian date Astronomy"), Category = "AstroLib")
	static FAstroDouble GetJulianDate(FDateTime DateTime);
	
	/*
	Returns the Julian Date format from an FDateTime.
	* @param JulianDate - The julian date 
	* @return The date in FDateTime format
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Calendar Date", Keywords = "Julian date Astronomy"), Category = "AstroLib")
	static FDateTime GetCalendarDate(FAstroDouble JulianDate);

	/*
	Returns the terrestrial time in Julian Date format
	* @param UTCTime - UTC Time
	* @return Terrestrial Time in Julian Date format
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Terrestrial Time Julian Date", Keywords = "Terrestrial time TT Julian date Astronomy"), Category = "AstroLib")
	static FAstroDouble GetTTJulianDatefromUTC(FDateTime UTCTime);

	/*
	Returns the approximate (within 0.5 seconds) UT1 time in Julian Date format
	* @param UTCTime - UTC Time
	* @return UT1 in Julian Date
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get UT1 Julian Date", Keywords = "UT1 time julian date Astronomy"), Category = "AstroLib")
	static FAstroDouble GetUT1JulianDatefromUTC(FDateTime UTCTime);

	
	/*
	Returns the name of the type of coordinate returned by Star or Planet poistion, based on observer location type and coord system selections.
	* @param LocationType - The type of location of the observer
	* @param ObserverType - The type of observer location.  (Center of earth, earth surface, in space). Only valid names returned for earth center and earth surface.
	* @return Common ame of the type of coordinate returned by Star or Planet poistion (i.e. 'Apparent Place', 'Topocentric place', etc)
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Coordinate Type Name", Keywords = "Ephemeris"), Category = "AstroLib")
	static ECoordTypes GetCoordsType(const EWhereIsObserver ObserverType, const ECoordSystems RequestedCoordSystem);
	
	/*
	Returns the celestial position of a star (or any object outside the solar system) in various coordinate systems (Calls Novas place function). 
	For example, to find the apparent position of a star from a place on earth, use this function with an 'on earth' observer and 'True Equator and Equinox of Date' coordinate system.
	Then pass the result into the Topocentric to AzEl Horizon function. Get a name for the outputted coordinate system with Get "Coordinate Type Name".
	@param CatEntry - The star (or other extra-solar system object) data.
	@param Observer - Location information of the observer.
	@param UTCTime - UTC Time for the observation.
	@param RequestedCoordSystem - Coord system for the output position.
	@param OutSkyPosition -  The objects place in the sky in the referenced to the RequestedCoordSystem system.
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Calculate Star Position", Keywords = "calculate Star Position Astronomy place"), Category = "AstroLib")
	static int CalculateStarPosition(const FStarCatalogEntry CatEntry, const FObserver Observer, const FDateTime UTCTime, ECoordSystems RequestedCoordSystem, FSkyPosition &OutSkyPosition);

	//Shows errors returned from the Novas function Place.
	static void ShowPlaceFunctionError(int error);

	/*
	Computes the Greenwich sidereal time, either mean or apparent, at the specified time.
	@param UTCTime - UTC Time for the calculation.
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Calculate Earth Sidereal Time", Keywords = "Sidereal time"), Category = "AstroLib")
	static int CalculateSideRealTime(const FDateTime UTCTime, const ESideRealTime SiderealTimeType, float &OutSideRealTimeInHours, const ECalculationMethod CalcMethod = ECalculationMethod::CIO);

	/*
	This function rotates a vector from the terrestrial (ITRS) to the
	celestial (GCRS) system. Specifically, it transforms a vector in the
	ITRS (rotating earth-fixed system, i.e. ECEF system or Geodedic coordinates) 
	to the GCRS (a local spacefixed system) by applying rotations for polar motion, 
	Earth rotation, nutation, precession, and the dynamical-to-GCRS frame tie.
	(Novas ter2cel function)
	@param UTCTime - UTC Time for the ITRS vector position.
	@param OutCoordSystem - GCRS or Equator/Equinox of Date. (MUST USE Equinox calculation method if Equator/Equinox is selected).
	@param xp - Polar motion xp value with respect to ITRS pole in arcseconds. Can safely be left at 0 unless very high accuracy is required.
	@param yp - Polar motion yp value with respect to ITRS pole in arcseconds. Can safely be left at 0 unless very high accuracy is required.
	@param InITRSPosition - Input ITRS vector position, geocentric equatorial rectangular coordinates
	@param OutGCRSVector - GCRS vector output.  Geocentric equatorial rectangular coordinates, referred to GCRS axes (celestial system) or with respect to the equator and equinox of date, depending on 'option'.
	@param CalcMethod - Method to use for the calculation. 
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Terrestrial To Celestial", Keywords = "GCRS ITRS Terrestrial celestrial"), Category = "AstroLib")
	static int TerrestrialToCelestial(const FDateTime UTCTime, const ETer2CelOptions OutCoordSystem, const float xp, const float yp, FAstroDoubleVector InITRSPosition, FAstroDoubleVector&  OutGCRSVector, const ECalculationMethod CalcMethod = ECalculationMethod::CIO);

	/*
	This function rotates a vector from the celestial (GCRS) system  to the
	terrestrial (ITRS). Specifically, it transforms a vector to the
	ITRS (rotating earth-fixed system, i.e. ECEF system or Geodedic coordinates)
	from the GCRS (a local spacefixed system) by applying rotations for polar motion,
	Earth rotation, nutation, precession, and the dynamical-to-GCRS frame tie.
	(Novas cel2ter function)
	@param UTCTime - UTC Time for the ITRS vector position.
	@param OutCoordSystem - GCRS or Equator/Equinox of Date. (MUST USE Equinox calculation method if Equator/Equinox is selected).
	@param xp - Polar motion xp value with respect to ITRS pole in arcseconds. Can safely be left at 0 unless very high accuracy is required.
	@param yp - Polar motion yp value with respect to ITRS pole in arcseconds. Can safely be left at 0 unless very high accuracy is required.
	@param InGCRSPosition - GCRS vector input.  Geocentric equatorial rectangular coordinates.
	@param OutITRSVector - Output ITRS vector position, geocentric equatorial rectangular coordinates, referred to GCRS axes (celestial system) or with respect to the equator and equinox of date, depending on 'option'.
	@param CalcMethod - Method to use for the calculation.
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Celestial To Terrestrial", Keywords = "GCRS ITRS Terrestrial celestrial"), Category = "AstroLib")
	static int CelestialToTerrestrial(const FDateTime UTCTime, const ETer2CelOptions OutCoordSystem, const float xp, const float yp, FAstroDoubleVector InGCRSPosition, FAstroDoubleVector&  OutITRSVector, const ECalculationMethod CalcMethod = ECalculationMethod::CIO);


	/*
	Transforms topocentric right ascension and declination to zenith distance and azimuth. 
	This function can also adjust coordinates for atmospheric refraction. (Novas equ2hor function).
	Call Star or Planet position with an 'on earth' observer and 'True Equator and Equinox of Date' coordinate system
	to get topocentric coordinates for this function.
	@param UTCTime - UTC Time for when the transformation is occuring.
	@param xp - Polar motion xp value with respect to ITRS pole in arcseconds. Can safely be left at 0 unless very high accuracy is required.
	@param yp - Polar motion yp value with respect to ITRS pole in arcseconds. Can safely be left at 0 unless very high accuracy is required.
	@param EarthSurfaceLocation - Observer location information. Tempature and pressure only used for refraction.
	@param InVec - Topocentric right ascension and declination. Call Star or Planet position with an 'on earth' observer and 'True Equator and Equinox of Date' coordinate system to get topocentric coordinates for this function.
	@param InRefraction - Refraction options. Use RefractionCorrectionUsingEarthLocationVals to use the tempature and pressure settings specified in EarthSurfaceLocation. Otherwise, you may select standard values for refraction or no refraction alterations.
	@param Azimuth - Azimuth in degrees, measured east from north.
	@param Elevation - Altitude from the horizon in degrees (Altitude).
	@param OutTopoSphereCoords - Refraction affected topocentric spherical coords (if refraction option is selected).
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Topocentric to AzEl Horizon", Keywords = "Azimuth Az Elevation El Horizon Topocentric equ2hor"), Category = "AstroLib")
	static void TopocentricToHorizon(const FDateTime UTCTime, const float xp, const float yp, const FEarthSurfaceLocation EarthSurfaceLocation, FSphereCoords InSphereCoords, const ETer2HorRefractionOptions InRefraction, FAstroDouble &Azimuth, FAstroDouble &Elevation, FSphereCoords &OutTopoSphereCoords);

	/*
	Transforms Azimuth and elevation to cartesian rectangular coordinates (left hand rule for Unreal compatibility).
	@param Azimuth - Azimuth
	@param Elevation - Elevation
	@param Distance - Distance
	@return Rectangular coords (x,y,z)
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Rectangular Coords from Az El", Keywords = "Get Rectangular Coords from Az El"), Category = "AstroLib")
	static FAstroDoubleVector AzElToRectangular(FAstroDouble Azimuth, FAstroDouble Elevation, FAstroDouble Distance);

	/*
	Transforms cartesian rectangular coordinates to Azimuth and elevation.
	@param InVector - Vector to determine the Az Elevation for.
	@param Elevation - Elevation of vector
	@param Azimuth - Azimuth of vector
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get AzEl from Rectangular", Keywords = "Get Rectangular Coords from Az El"), Category = "AstroLib")
	static void RectangularToAzEl(FVector InVector, float & Azimuth, float & Elevation);

	/*
	Transforms cartesian rectangular coordinates to Azimuth and elevation
	@param InVector - Vector to determine the Az Elevation for.
	@param Elevation - Elevation of vector
	@param Azimuth - Azimuth of vector
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get AzEl from Rectangular (Double)", Keywords = "Get Rectangular Coords from Az El"), Category = "AstroLib")
	static void RectangularToAzElDbl(FAstroDoubleVector InVector, FAstroDouble & Azimuth, FAstroDouble & Elevation);


	/*
	Transform a star's (or other extra-solar system object) catalog quantities for a change of epoch
	and/or equator and equinox. Also used to rotate catalog
	quantities on the dynamical equator and equinox of J2000.0 to the
	ICRS or vice versa. (transform_cat)
	@param Opts - Selects which asppects (epoch, equinox) to transform. (ChangeEquatorEquinoxandEpoch is most common).
	@param InCatEntry - Input star data.
	@param InEpochAndEquinoxTime - Time the input data eqpch/equinox is referenced by. Only used for ChangeEpochSameEquatorAndEquinox, ChangeEquatorAndEquinoxSameEpoch, ChangeEquatorEquinoxandEpoch.
	@param OutEpochAndEquinoxTime - Time the output data eqpch/equinox will be referenced by. Only used for ChangeEpochSameEquatorAndEquinox, ChangeEquatorAndEquinoxSameEpoch, ChangeEquatorEquinoxandEpoch.
	@param OutCatEntry - The transformed star data
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Transform Coord System", Keywords = "Coord System transform"), Category = "AstroLib")
	static int TransformCoordSystem(const ETransformCoordOptions Opts, FStarCatalogEntry InCatEntry, const FDateTime InEpochAndEquinoxTime, const FDateTime OutEpochAndEquinoxTime, FStarCatalogEntry &OutCatEntry);

	/*
	To convert Hipparcos catalog data at epoch J1991.25 to epoch
	J2000.0. To be used only for Hipparcos or
	Tycho stars with linear space motion. Both input and output data
	is in the ICRS. (transform_hip)
	@param InCatEntry - Input Hipparcos star data.
	@param OutCatEntry - The transformed star data in J2000
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Hipparcos To J2000", Keywords = "Hipparcos J2000 Coord system "), Category = "AstroLib")
	static void HipparcosToJ2000(FStarCatalogEntry InCatEntry, FStarCatalogEntry &OutCatEntry);

	/*
	Returns the celestial position of a planet in various coordinate systems (Calls Novas place function).
	For example, to find the apparent position of a planet from a plane on earth, use this function with an 'on earth' observer and 'True Equator and Equinox of Date' coordinate system.
	Then pass the result into the Topocentric to AzEl Horizon function. Get a name for the outputted coordinate system with Get "Coordinate Type Name".
	@param InBody - The planet to get data for.
	@param Observer - Location information of the observer.
	@param UTCTime - UTC Time for when to retrieve the planet location.
	@param RequestedCoordSystem - Coord system for the output position.
	@param OutSkyPosition -  The objects place in the sky in the referenced to the RequestedCoordSystem system.
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Calculate Planet Position", Keywords = "Planet Position Astronomy"), Category = "AstroLib")
	static int CalculatePlanetPosition(const ESolarSystemBodys InBody, const FObserver Observer, const FDateTime UTCTime, ECoordSystems RequestedCoordSystem, FSkyPosition &OutSkyPosition);

	/*
	Returns the direction towards a planet from earth surface. Calls CalculatePlanetPosition to get a topocentric celestial position, then passes the result into TopocentricToAzEl().
	@param InBody - The planet to get data for.
	@param Observer - Location information of the observer. Observer should be set to On Earth and the surface locaition information shoud be set.
	@param UTCTime - UTC Time for when to retrieve the planet location.
	@param OutDirection -  The objects place in the sky in the referenced from WorldLocation of 0,0,0.
	@param OutAz - Azimuth in degrees, measured east from north.
	@param OutEl - Altitude from the horizon in degrees (Altitude).
	@param OutDistance - Distance from Observer in AU.
	@param NorthOffset - Number of degrees to offset north from the x axis, rotated clockwise
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Calculate Planet Direction From Earth", Keywords = "Planet Position Astronomy"), Category = "AstroLib")
	static int PlanetDirectionFromEarth(const ESolarSystemBodys InBody, const FObserver Observer, const FDateTime UTCTime, FAstroDoubleVector & OutDirection, FAstroDouble & OutAz, FAstroDouble & OutEl, FAstroDouble & OutDistance, ETer2HorRefractionOptions RefractionMethod = ETer2HorRefractionOptions::NoRefractionCorrection, float NorthOffset = 0);

	/*
	Precesses equatorial rectangular coordinates from one epoch to
	another. One of the two epochs must be J2000.0 (2451545.0 exactly). The coordinates
	are referred to the mean dynamical equator and equinox of the two
	respective epochs. TT can be used instead of TDB in most cases.
	To precess a vector from one arbitrary date to another, call precession twice, using 
	J2000.0 as the middle date.
	Use 'Get Terrestrial Time Julian Date' to get the julian date
	@param TDBDate1 - TDB Date of the first epoch. 
	@param InCoords - The position vector in geocentric equatorial rectangular coordinates, referred to mean dynamical equator and equinox of first epoch.
	@param TDBDate2 - TDB Date of the second epoch.
	@param OutCoords - The position vector in geocentric equatorial rectangular coordinates, referred to mean dynamical equator and equinox of second epoch.
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Precession", Keywords = "Planet Position Astronomy Precession Coordinate"), Category = "AstroLib")
	static int PrecessionShift(FAstroDouble TDBDate1, FAstroDoubleVector InCoords, FAstroDouble TDBDate2, FAstroDoubleVector & OutCoords);

	/*
	To convert right ascension and declination to ecliptic longitude
	and latitude. converts the equatorial position of an object into the equivalent ecliptic
	position: equatorial coordinates ra and dec are converted to ecliptic coordinates.
	@param InEpochDate - Date to use for the equator, ecliptic, and equinox.
	@param CoordSystem - Specifies if InCoords is refered to the Mean equator, true equator, or GCRS.
	@param InCoords - Input Right Ascension and declination.
	@param OutLatLong - Output latitude and longitude in degrees, referred to specified ecliptic and equinox of date.
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert RA and Dec To Ecliptic LatLong", Keywords = "convert right ascension declination to ecliptic lat latirude long longitude astronomy"), Category = "AstroLib")
	static int ConvertSphericalToEclipticLatLong(const FDateTime InEpochDate, const ECelestialCoordSystems CoordSystem, FSphereCoords InCoords, FLatLong & OutLatLong);

	/*
	Computes the true right ascension of the celestial
	intermediate origin (CIO) at a given TT Julian date. This is
	the equation of the origins) (Novas function cio_ra).
	@param InEpochDate - Time for the CIO computation.
	@param OutRightAscension - Right ascension of the CIO, with respect to the true equinox of date, in hours
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "CIO Right Ascension", Keywords = "Equation of the origins CIO Right ascencion Astronony"), Category = "AstroLib")
	static int EquationOfOrigins(const FDateTime InEpochDate, FAstroDouble & OutRightAscension);

	/*
	This function returns the value of the Earth Rotation Angle
	(theta) for a given UTC Julian date. The expression used is
	taken from the note to IAU Resolution B1.8 of 2000.
	@param InUTCDate - UTC Date for the rotation angle to retrieve.
	@return Earth rotation angle in degrees.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Earth Rotation Angle", Keywords = "Earth Rotation Angle Astronony"), Category = "AstroLib")
	static FAstroDouble EarthRotationAngle(const FDateTime InUTCDate);

	/*
	This function specifies celestial pole
	offsets for very high - precision applications. Each set of offsets is
	a correction to the modeled position of the pole for a specific
	date, derived from observations and published by the IERS.
	If you use SetCelestialPoleOffsets, make sure it is called before any other
	functions for a given date. Values of the pole offsets that you specify by a call here
	will be used by until you explicitly change them.
	@param InUTCDate - Date the pole offset data describes.
	@param OffsetType - Type of pole offset value specified/
	@param Pole1 - Value of celestial pole offset in first coordinate, (delta-delta-psi or dx) in milliarcseconds.
	@param Pole2 - Value of celestial pole offset in second coordinate,(delta-delta-epsilon or dy) in milliarcseconds.
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Set Celestial Pole Offsets", Keywords = "Celestial pole offsets astronomy"), Category = "AstroLib")
	static int SetCelestialPoleOffsets(const FDateTime InUTCDate, const EPoleOffsetType OffsetType, FAstroDouble Pole1, FAstroDouble Pole2);

	/*
	This function computes various quantities related to the orientation of the Earth’s rotation
	axis (vector toward Celestial Intermediate Pole) with respect to the ecliptic plane at a specific
	time. The computation involves a call to function nutation_angles to evaluate the nutation
	series. (Novas function cel_pole).
	@param InUTCDate - Time to retrieve earth tild data
	@param MeanObliquity - Mean obliquity of the ecliptic in degrees
	@param TrueObliquity - True obliquity of the ecliptic in degrees
	@param EquationOfTheEquinoxes - Equation of the equinoxes in seconds of time
	@param NutationInLongitude - Nutation in longitude in arcseconds
	@param NutationInObliquity - Nutation in obliquity in arcseconds
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Earth Tilt Paramaters", Keywords = "Earth tilt precession nutation astronomy"), Category = "AstroLib")
	static void GetEarthTiltParameters(const FDateTime InUTCDate, FAstroDouble & MeanObliquity, FAstroDouble & TrueObliquity, FAstroDouble & EquationOfTheEquinoxes, FAstroDouble & NutationInLongitude, FAstroDouble & NutationInObliquity);


	/*
	Converts an equatorial position vector to an ecliptic position
	vector. To convert an ICRS vector to an ecliptic vector (mean ecliptic
	and equinox of J2000.0 only), set 'coord_sys' = 2; the value
	of 'jd_tt' can be set to anything, since J2000.0 is assumed.
	@param JulianDateTT - The julian date to use.
	@param CoordSystem - Specifies if InCoords is refered to the Mean equator, true equator, or GCRS.
	@param InEquatorialVector - Input Vector, referred to CoordSystem specified equator and equinox of date.
	@param OutEclipticalVector - Output Vector, referred to CoordSystem specified equator and equinox of date.
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert To Ecliptical", Keywords = "Ecliptical astronomy coordinate"), Category = "AstroLib")
		static int ConvertToEcliptical(FAstroDouble JulianDateTT, ECelestialCoordSystems CoordSystem, FAstroDoubleVector InEquatorialVector, FAstroDoubleVector & OutEclipticalVector);

	/*
	Converts an ecliptic position vector to an equatorial position
	vector. To convert an ecliptic vector (mean ecliptic and equinox of
	J2000.0 only) to an ICRS vector, set 'coord_sys' = 2; the value
	of 'jd_tt' can be set to anything, since J2000.0 is assumed.
	@param JulianDateTT - The julian date to use.
	@param CoordSystem - Specifies if InCoords is refered to the Mean equator, true equator, or GCRS.
	@param InEclipticalVector - Input Vector, referred to CoordSystem specified equator and equinox of date.
	@param OutEquatorialVector - Output Vector, referred to CoordSystem specified equator and equinox of date.
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Convert To Equatorial", Keywords = "Ecliptical astronomy coordinate"), Category = "AstroLib")
		static int ConvertToEquatorial(FAstroDouble JulianDateTT, ECelestialCoordSystems CoordSystem, FAstroDoubleVector InEclipticalVector, FAstroDoubleVector & OutEquatorialVector);


	/*
	Converts a Right Ascenstion and a declination to a vector. 
	(equatorial spherical coordinates to a vector (equatorial
      rectangular coordinates)
	@param RightAscension - Right ascension (hours) 
	@param Declination - Declination (degrees)
	@param Distance - Distance (AU)
	@param OutEquatorialVector - Position vector, equatorial rectangular coordinates (AU)
	@return Error code. Zero is success.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Spherical To Vector", Keywords = "Ecliptical astronomy coordinate"), Category = "AstroLib")
	void SphericalCoordsToVector(FAstroDouble RightAscension, FAstroDouble Declination, FAstroDouble Distance, FAstroDoubleVector & OutEquatorialVector);

	/*
	Returns the ratio of a planet's radius compared to earth.
	*/
	UFUNCTION(BlueprintPure, Category = "Astronomy")
	static float GetPlanetToEarthRadiusRatio(const ESolarSystemBodys InBody);

	/*
	Returns a rotator (Left hand coordinate system to be compatible with Unreal) representing the earths rotation.
	*/
	UFUNCTION(BlueprintPure, Category = "Astronomy")
	static FRotator GetEarthRotator(const FDateTime UTCTime, EPlane Plane = EPlane::Equatorial);
	
	/*
	Returns a FAstroDoubleVector that is in AU units in KM 
	*/
	UFUNCTION(BlueprintPure, Category = "Astronomy")
	static FAstroDoubleVector GetKMFromAU(FAstroDoubleVector InAuDoubleVector);

	/*
	Returns the azimuth (0-360 degrees) and elevation from a world direction vector.
	@param OutAzimuth - azimuth (0-360 degrees)
	@param OutElevation - elevation +/- 90 degrees
	*/
	UFUNCTION(BlueprintPure, Category = "Astronomy")
		static void GetAzElInDegrees(FVector WorldDirection, float& OutAzimuth, float& OutElevation, float NorthOffset = 0);

	/*
	Converts geodetic coordinates to geocentric position vector	@param OutAzimuth - azimuth (0-360 degrees) (Novas function terra)
	@param SiderealTime - Local apparent sidereal time at reference meridian in hours.
	@param OutPosition - Position vector of observer with respect to center of Earth, equatorial rectangular coordinates, referred to true equator and equinox of date, components in AU.
	@param OutVelocity - Velocity vector of observer with respect to center of Earth, equatorial rectangular coordinates, referred to true equator and equinox of date, components in AU per day.
	*/
	UFUNCTION(BlueprintPure, Category = "Astronomy")
	static void CoordsToPosition(FEarthSurfaceLocation EarthSurfaceLocation, FAstroDouble SiderealTime, FAstroDoubleVector& OutPosition, FAstroDoubleVector& OutVelocity);

	/*
	This function computes the geocentric position and velocity of an observer on the surface of the earth or on a near-earth spacecraft.  The final vectors are expressed in the GCRS.
	@param SiderealTime - Local apparent sidereal time at reference meridian in hours.
	@param OutPosition - Position vector of observer, with respect to origin at geocenter, referred to GCRS axes, components in AU.
	@param OutVelocity - Velocity vector of observer, with respect to origin at geocenter, referred to GCRS axes, components in AU/day.
	*/
	UFUNCTION(BlueprintPure, Category = "Astronomy")
	static void GeoPosAndVel(const FDateTime InUTCDate, FObserver Observer, FAstroDoubleVector& OutPosition, FAstroDoubleVector& OutVelocity);

	/*
	Returns an approximate value for a body's rise, setting, and transit time from Earth. From Astronomical Algorithms by Jean Meeus.
	@param InBody - The solar system body to obtain values for.
	@param EarthSurfaceLocation - Location of observer on earths surface. Altitude and temperature not used for this function.
	@param InUTCDate - A date in UTC to check obtain values for.
	@param OutRisingTime - Universal Time of the body rising above the horizon.
	@param OutSettingTime - Universal Time of the body setting below the horizon.
	@param OutTransitTime - Universal Time of the body crossing the local meridian when above the horizon.
	@param bAboveHorizonAllDay - Returns true if the object is above the horizon for the entire day (rise time and setting time are invalid)
	@param bBelowHorizonAllDay - Returns true if the object is below the horizon for the entire day (rise time and setting time are invalid)

	*/
	UFUNCTION(BlueprintPure, Category = "Astronomy rise transit set sunrise sunset")
	static void GetRisingTransitSettingTimes(const ESolarSystemBodys InBody, const FEarthSurfaceLocation EarthSurfaceLocation, const FDateTime InUTCDate, FDateTime& OutRisingTime, FDateTime & OutSettingTime, FDateTime& OutTransitTime, bool& bAboveHorizonAllDay, bool& bBelowHorizonAllDay);

	/*
	Interpolate based on 3 values.
	@param n - Interpolating factor, between -1 and 1.
	*/
	UFUNCTION(BlueprintPure, Category = "Astronomy")
	static float AstroInterpolate(const float y1, const float y2, const float y3, const float n);

};

