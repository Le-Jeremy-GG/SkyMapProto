// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.
#pragma once

#include "AstroLibBPLibrary.h"
#include "NovasCFunctions.h"
#include "Time.h"
#include "string"



UAstroLibBPLibrary::UAstroLibBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	
}

FAstroDouble UAstroLibBPLibrary::GetJulianDate(FDateTime DateTime)
{

	double hour = DateTime.GetHour();
	hour += DateTime.GetMinute() / 60.l;
	hour += DateTime.GetSecond() / 3600.l;
	hour += DateTime.GetMillisecond() / 3600000.l;

	FAstroDouble jd = julian_date(DateTime.GetYear(), DateTime.GetMonth(), DateTime.GetDay(), hour);
	return jd;

}

FDateTime UAstroLibBPLibrary::GetCalendarDate(FAstroDouble JulianDate)
{

	short int year;
	short int month;
	double DecHours;
	short int day;


	cal_date(JulianDate, &year, &month, &day, &DecHours);

	int hours = DecHours;
	double minutesRemainder = (DecHours - hours) * 60;
	int minutes = minutesRemainder;
	double secondsRemainder = (minutesRemainder - minutes) * 60;
	int seconds = secondsRemainder;

	double MillisecondsRemainder = (secondsRemainder - seconds) * 100;
	int milliseconds = MillisecondsRemainder;



	FDateTime tempTime = FDateTime(year, month, day, hours, minutes, seconds, milliseconds);
	return tempTime;

}



FAstroDouble UAstroLibBPLibrary::GetTTJulianDatefromUTC(FDateTime UTCTime)
{

	double DeltaT = Time::getDeltaTByEspenakMeeus(UTCTime);

	double jd_ut1 = GetUT1JulianDatefromUTC(UTCTime);

	double jd_tt = jd_ut1 + (DeltaT / 86400);

	return jd_tt;





}

FAstroDouble UAstroLibBPLibrary::GetUT1JulianDatefromUTC(FDateTime UTCTime)
{


	double hour = UTCTime.GetHour();
	hour += UTCTime.GetMinute() / 60.l;
	hour += UTCTime.GetSecond() / 3600.l;
	hour += UTCTime.GetMillisecond() / 3600000.l;

	double jd_ut1 = julian_date(UTCTime.GetYear(), UTCTime.GetMonth(), UTCTime.GetDay(), hour);
	jd_ut1 = jd_ut1 + 0.0000052083; //0.45 seconds in julian date format. UT1 differs from UTC by between 0 and 0.9 seconds, so just using the average.
	return jd_ut1;
}

ECoordTypes UAstroLibBPLibrary::GetCoordsType(EWhereIsObserver LocationType, ECoordSystems RequestedCoordSystem)
{
	switch (LocationType)
	{
	case EWhereIsObserver::Geocenter:
		switch (RequestedCoordSystem)
		{
		case ECoordSystems::GCRS:
			return ECoordTypes::VirtualOrProperPlace;
			break;
		case ECoordSystems::TrueEquatorAndEquinoxOfDate:
			return ECoordTypes::ApparentPlace;
			break;
		case ECoordSystems::TrueEquatorAndCIOOfDate:
			return ECoordTypes::IntermediatePlace;
			break;
		case ECoordSystems::AstronomicPlace:
			return ECoordTypes::AstronomicPlace;

			break;

		}
		break;
	case EWhereIsObserver::EarthSurface:
		switch (RequestedCoordSystem)
		{
		case ECoordSystems::GCRS:
			return ECoordTypes::LocalPlace;
			break;
		case ECoordSystems::TrueEquatorAndEquinoxOfDate:
			return ECoordTypes::TopocentricPlace;
			break;
		case ECoordSystems::TrueEquatorAndCIOOfDate:
			return ECoordTypes::TopocentricIntermediatePlace;
			break;
		case ECoordSystems::AstronomicPlace:
			return ECoordTypes::TopocentricAstronomicPlace;
			break;
		}

		
	}
	return ECoordTypes::Unset;
			
}
int UAstroLibBPLibrary::CalculateStarPosition(const FStarCatalogEntry CatEntry, const FObserver Observer, FDateTime UTCTime, ECoordSystems RequestedCoordSystem, FSkyPosition &OutSkyPosition)
{

	
	double jd_tt;
	jd_tt = GetTTJulianDatefromUTC(UTCTime);
	
	FStarCatalogEntry TempCatEntry = CatEntry;
	cat_entry star = TempCatEntry.GetNovasStruct();
	

	object CelObj;
	char* StarNameCStr = TCHAR_TO_ANSI(*CatEntry.StarNameFString.Left(SIZE_OF_OBJ_NAME));
	make_object(2, CelObj.number, StarNameCStr, &star, &CelObj);
		
	FObserver TempObs = Observer;
	observer obs = TempObs.GetNovasStruct();

	double deltaT =0;
	if (TempObs.LocationType == EWhereIsObserver::EarthSurface || TempObs.LocationType == EWhereIsObserver::NearEarthSpaceCraft)
	{
		deltaT = Time::getDeltaTByEspenakMeeus(UTCTime);
	}
	
	short int error = 0;

	sky_pos SkyPosition;
	error = place(jd_tt, &CelObj, &obs, deltaT, short(RequestedCoordSystem), accuracy, &SkyPosition);

	if (error == 0)
	{
		OutSkyPosition.MakeUnrealStruct(SkyPosition);
		OutSkyPosition.CoordType = GetCoordsType(Observer.LocationType, RequestedCoordSystem);
		return 0;
	}

	ShowPlaceFunctionError(error);

	return error;
}

void UAstroLibBPLibrary::ShowPlaceFunctionError(int error)
{

	UE_LOG(LogTemp, Warning, TEXT("Novas error: %i"), error);
	if (error == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid value of Coord System (function 'place')"));
	}
	else if (error == 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid value of accuracy (function 'place')"));
	}
	else if (error == 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Observer on earth cannot observe earth (function 'place')"));
	}
	else if (error == 11)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid value of origin"));
	}
	else if (error == 12)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid type in Cel Obj"));
	}
	else if (error == 13)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to allocate memory"));
	}
	else if (error > 20 && error < 30)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error with solarsystem function"));
	}
	else if (error > 30 && error < 40)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error with readeph function"));
	}
	else if (error > 40 && error < 50)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error with geo_posvel function"));
	}
	else if (error > 50 && error < 70)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error with lighttime function"));
	}
	else if (error > 70 && error < 80)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error with grav_def function"));
	}
	else if (error > 80 && error < 90)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error with cio_location function"));
	}
	else if (error > 90 && error < 100)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error with cio_basis function"));
	}
}

int UAstroLibBPLibrary::CalculateSideRealTime(const FDateTime UTCTime, ESideRealTime SiderealTimeType, float & OutSideRealTimeInHours, ECalculationMethod CalcMethod)
{
	double jd_ut;
	jd_ut = GetUT1JulianDatefromUTC(UTCTime);

	double deltaT = Time::getDeltaTByEspenakMeeus(UTCTime);

	double gst;
	short int error = sidereal_time(jd_ut, 0.0, deltaT, short(SiderealTimeType), short(CalcMethod), accuracy, &gst);

	OutSideRealTimeInHours = gst;

	return error;

}

int UAstroLibBPLibrary::TerrestrialToCelestial(const FDateTime UTCTime, const ETer2CelOptions OutCoordSystem, const float xp, const float yp, FAstroDoubleVector InITRSPosition, FAstroDoubleVector&  OutGCRSVector, const ECalculationMethod CalcMethod)
{
	double jd_ut1;
	jd_ut1 = GetUT1JulianDatefromUTC(UTCTime);

	double deltaT = Time::getDeltaTByEspenakMeeus(UTCTime);

	double ITRSPos[3];
	InITRSPosition.GetDoubleVector(ITRSPos);
	
	double GCRSPos[3];
	short int error = ter2cel(jd_ut1, 0, deltaT, short(CalcMethod), accuracy, short(OutCoordSystem), xp, yp, ITRSPos, GCRSPos);
	
	OutGCRSVector.SetDoubleVector(GCRSPos);

	return error;

}

int UAstroLibBPLibrary::CelestialToTerrestrial(const FDateTime UTCTime, const ETer2CelOptions OutCoordSystem, const float xp, const float yp, FAstroDoubleVector InGCRSPosition, FAstroDoubleVector&  OutITRSVector, const ECalculationMethod CalcMethod)
{
	double jd_ut1;
	jd_ut1 = GetUT1JulianDatefromUTC(UTCTime);

	double deltaT = Time::getDeltaTByEspenakMeeus(UTCTime);

	double GCRSPos[3];
	InGCRSPosition.GetDoubleVector(GCRSPos);

	double ITRSPos[3];
	short int error = cel2ter(jd_ut1, 0, deltaT, short(CalcMethod), accuracy, short(OutCoordSystem), xp, yp, GCRSPos, ITRSPos);

	OutITRSVector.SetDoubleVector(GCRSPos);

	return error;

}

void UAstroLibBPLibrary::TopocentricToHorizon(const FDateTime UTCTime, float xp, float yp, FEarthSurfaceLocation EarthSurfaceLocation, FSphereCoords InSphereCoords, ETer2HorRefractionOptions InRefraction, FAstroDouble &Azimuth, FAstroDouble &Elevation, FSphereCoords &OutTopoSphereCoords)
{
	
	double jd_ut1 = GetUT1JulianDatefromUTC(UTCTime);
	double DeltaT = Time::getDeltaTByEspenakMeeus(UTCTime);

	on_surface tmpOnSurface = EarthSurfaceLocation.GetNovasStruct();

	double zd, az, rar, decr;
	equ2hor(jd_ut1,DeltaT,accuracy,xp,yp,&tmpOnSurface, InSphereCoords.RightAscension, InSphereCoords.Declination, short(InRefraction),&zd,&az,&rar,&decr);
	
	OutTopoSphereCoords.RightAscension = rar;
	OutTopoSphereCoords.Declination = decr;

	Azimuth = az;
	Elevation = 90 - zd;

	return;
}

FAstroDoubleVector UAstroLibBPLibrary::AzElToRectangular(FAstroDouble Azimuth, FAstroDouble Elevation, FAstroDouble Distance)
{
		
	double ElevationRadians = Elevation * DEG2RAD;
	double AzimuthRadians = Azimuth * DEG2RAD;
	
	FAstroDouble SinInclination = sin(ElevationRadians);
	FAstroDouble CosAzimuth = cos(AzimuthRadians);
	FAstroDouble SinAzimuth = sin(AzimuthRadians);
	FAstroDouble CosInclination = cos(ElevationRadians);

	FAstroDouble y = SinAzimuth * CosInclination * Distance;
	FAstroDouble x = CosAzimuth * CosInclination * Distance;
	FAstroDouble z = SinInclination * Distance;

	FAstroDoubleVector TempVector;
	TempVector.SetDoubleVector(x, y, z);
	return TempVector;
}

void UAstroLibBPLibrary::RectangularToAzEl(FVector InVector, float &Azimuth, float &Elevation)
{
	double r = InVector.Size();
	Elevation = FMath::RadiansToDegrees(FMath::Asin(InVector.Z / r));
	Azimuth = FMath::RadiansToDegrees(FMath::Atan(InVector.Y / r));
}

void UAstroLibBPLibrary::RectangularToAzElDbl(FAstroDoubleVector InVector, FAstroDouble &Azimuth, FAstroDouble &Elevation)
{

	double r = pow(InVector.x * InVector.x + InVector.y * InVector.y + InVector.z * InVector.z, 0.5);
	Elevation = asin(InVector.z / r) * 57.29577951308232087;
	Azimuth = atan(InVector.y / r) * 57.29577951308232087;
}

int UAstroLibBPLibrary::TransformCoordSystem(const ETransformCoordOptions Opts, FStarCatalogEntry InCatEntry, const FDateTime InEpochAndEquinoxTime, const FDateTime OutEpochAndEquinoxTime, FStarCatalogEntry & OutCatEntry)
{	
	

	double oldjd_tt = 0;
	double newjd_tt = 0;

	if (Opts == ETransformCoordOptions::ChangeEpochSameEquatorAndEquinox || Opts == ETransformCoordOptions::ChangeEquatorAndEquinoxSameEpoch || Opts == ETransformCoordOptions::ChangeEquatorEquinoxandEpoch)
	{
		oldjd_tt = GetTTJulianDatefromUTC(InEpochAndEquinoxTime);
		newjd_tt = GetTTJulianDatefromUTC(OutEpochAndEquinoxTime);
	}


	cat_entry newentry;
	short int error = 0;
	
	cat_entry old_entry = InCatEntry.GetNovasStruct();

	char* StarNameCStr = TCHAR_TO_ANSI(*InCatEntry.StarNameFString.Left(SIZE_OF_OBJ_NAME));
	error = transform_cat(short(Opts), oldjd_tt, &old_entry, newjd_tt, StarNameCStr, &newentry);

	FStarCatalogEntry NewCatEntry(newentry);
	OutCatEntry = NewCatEntry;

	return error;
}

void UAstroLibBPLibrary::HipparcosToJ2000(FStarCatalogEntry InCatEntry, FStarCatalogEntry & OutCatEntry)
{
	
	cat_entry hip_entry = InCatEntry.GetNovasStruct();
	cat_entry outentry;
	transform_hip(&hip_entry, &outentry);

	FStarCatalogEntry NewCatEntry(outentry);
	OutCatEntry = NewCatEntry;
}

int UAstroLibBPLibrary::CalculatePlanetPosition(const ESolarSystemBodys InBody, const FObserver Observer, const FDateTime UTCTime, ECoordSystems RequestedCoordSystem, FSkyPosition & OutSkyPosition)
{
	
	//UE_LOG(LogTemp, Warning, TEXT("ObserverLat: %f"), Observer.SurfaceLocation.Latitude);
	
	
	double jd_tt;
	jd_tt = GetTTJulianDatefromUTC(UTCTime);


	object CelObj;
	char * p = (char*)"";
	make_object(0, GetNovasIntFromSolarSystemBodys(InBody), p, NULL, &CelObj);

	FObserver TempObs = Observer;
	observer obs = TempObs.GetNovasStruct();

	double deltaT = 0;
	if (TempObs.LocationType == EWhereIsObserver::EarthSurface || TempObs.LocationType == EWhereIsObserver::NearEarthSpaceCraft)
	{
		deltaT = Time::getDeltaTByEspenakMeeus(UTCTime);
	}
	

	short int error = 0;

	//UE_LOG(LogTemp, Warning, TEXT("jdtt: %f"), jd_tt);
	//UE_LOG(LogTemp, Warning, TEXT("deltat: %f"), deltaT);
	//UE_LOG(LogTemp, Warning, TEXT("coordsys: %i"), int(RequestedCoordSystem));
	//UE_LOG(LogTemp, Warning, TEXT("cel obj type: %i"), CelObj.type);
	//UE_LOG(LogTemp, Warning, TEXT("cel obj num: %i"), CelObj.number);
	//UE_LOG(LogTemp, Warning, TEXT("obs where: %i"), obs.where);
	//UE_LOG(LogTemp, Warning, TEXT("obs lat: %f"), obs.on_surf.latitude);
	//UE_LOG(LogTemp, Warning, TEXT("obs long: %f"), obs.on_surf.longitude);
	

	sky_pos SkyPosition;
	error = place(jd_tt, &CelObj, &obs, deltaT, short(RequestedCoordSystem), accuracy, &SkyPosition);
	
	//UE_LOG(LogTemp, Warning, TEXT("RA %f DEC %f DIS%f"), SkyPosition.ra, SkyPosition.dec, SkyPosition.dis);


	if (error == 0)
	{
		OutSkyPosition.MakeUnrealStruct(SkyPosition);
		OutSkyPosition.CoordType = GetCoordsType(Observer.LocationType, RequestedCoordSystem);
		return 0;
	}

	ShowPlaceFunctionError(error);

	return error;


}

int UAstroLibBPLibrary::PlanetDirectionFromEarth(const ESolarSystemBodys InBody, const FObserver Observer, const FDateTime UTCTime, FAstroDoubleVector & OutDirection, FAstroDouble & OutAz, FAstroDouble & OutEl, FAstroDouble & OutDistanceInAu, ETer2HorRefractionOptions RefractionMethod, float NorthDirection)
{
	//UE_LOG(LogTemp, Warning, TEXT("UTCTime:%s"), *UTCTime.ToString());

	FSkyPosition CelestialPos;
	FSphereCoords OutTopoSphereCoords;
	CalculatePlanetPosition(InBody, Observer, UTCTime, ECoordSystems::TrueEquatorAndEquinoxOfDate, CelestialPos);
	TopocentricToHorizon(UTCTime, 0, 0, Observer.SurfaceLocation, CelestialPos.SphericalCoords, RefractionMethod, OutAz, OutEl, OutTopoSphereCoords);
	
	OutAz = OutAz + NorthDirection;
	OutAz = (double(OutAz) > 360.0) ? FAstroDouble((double(OutAz) - 360.0)) : FAstroDouble(OutAz);

	OutDirection = AzElToRectangular(OutAz, OutEl, 1);
	//OutDirection = OutDirection.RotateAngleAxisDbl(0, FVector(0, 0, 1));
	OutDistanceInAu = CelestialPos.Distance;
	return 0;
}

int UAstroLibBPLibrary::PrecessionShift(FAstroDouble Date1, FAstroDoubleVector InCoords, FAstroDouble Date2, FAstroDoubleVector & OutCoords)
{
	double jd_tdb1 = Date1;
	double jd_tdb2 = Date2;

	double pos1[3];
	InCoords.GetDoubleVector(pos1);

	double pos2[3];

	short int error = precession(jd_tdb1, pos1, jd_tdb2, pos2);
	
	if (error == 0)
	{
		OutCoords.SetDoubleVector(pos2);
	}
	else if (error == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Function requires to or from date to be J2000 (JD 2451545). To precess a vector from one arbitrary date to another, call precession twice, using J2000.0 as the middle date. Date1: %F Date 2: %f"),float(jd_tdb1),float(jd_tdb2));
	}

	return error;
}

int UAstroLibBPLibrary::ConvertSphericalToEclipticLatLong(const FDateTime InEpochDate, const ECelestialCoordSystems CoordSystem, FSphereCoords InCoords, FLatLong & OutLatLong)
{


		double jd_tt = GetTTJulianDatefromUTC(InEpochDate);

		double elon, elat;
		int error = equ2ecl(jd_tt, short(CoordSystem), accuracy, InCoords.RightAscension, InCoords.Declination, &elon, &elat);

		OutLatLong.SetLatLong(elat, elon);

		return error;

}

int UAstroLibBPLibrary::EquationOfOrigins(const FDateTime InEpochDate, FAstroDouble & OutRightAscension)
{
	
	double jd_tt = GetTTJulianDatefromUTC(InEpochDate);
	double ra_cio;
	short int error = cio_ra(jd_tt, accuracy, &ra_cio);
	
	if (error == 0)
	{
		OutRightAscension = ra_cio;
		return 0;
	}
	else if (error > 10 && error < 20)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error from function CIO Location"));

	}
	else if (error > 20)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error from Function CIO Basis"));

	}
		
	return error;
}

FAstroDouble UAstroLibBPLibrary::EarthRotationAngle(const FDateTime InUTCDate)
{
	double jd_high = GetUT1JulianDatefromUTC(InUTCDate);
	FAstroDouble OutEra = era(jd_high, 0);
	return OutEra;
}

int UAstroLibBPLibrary::SetCelestialPoleOffsets(const FDateTime InUTCDate, const EPoleOffsetType OffsetType, FAstroDouble Pole1, FAstroDouble Pole2)
{

	double tjd = GetTTJulianDatefromUTC(InUTCDate);
	short int error = cel_pole(tjd, short(OffsetType), Pole1, Pole2);
	return error;
}

void UAstroLibBPLibrary::GetEarthTiltParameters(const FDateTime InUTCDate, FAstroDouble & MeanObliquity, FAstroDouble & TrueObliquity, FAstroDouble & EquationOfTheEquinoxes, FAstroDouble & NutationInLongitude, FAstroDouble & NutationInObliquity)
{

	double jd_tdb = GetTTJulianDatefromUTC(InUTCDate);

	double mobl =0;
	double tobl = 0;
	double ee = 0;
	double dpsi = 0;
	double deps = 0;

	e_tilt(jd_tdb, accuracy, &mobl, &tobl, &ee, &dpsi, &deps);

	MeanObliquity = mobl;
	TrueObliquity = tobl;
	EquationOfTheEquinoxes = ee;
	NutationInLongitude = dpsi;
	NutationInObliquity = deps;
}

int UAstroLibBPLibrary::ConvertToEcliptical(FAstroDouble JulianDateTT, ECelestialCoordSystems CoordSystem, FAstroDoubleVector InEquatorialVector, FAstroDoubleVector & OutEclipticalVector)
{
	double PosVec[3];
	InEquatorialVector.GetDoubleVector(PosVec);

	double OutVec[3];
	int error = equ2ecl_vec(JulianDateTT, short(CoordSystem), accuracy, PosVec, OutVec);

	OutEclipticalVector.SetDoubleVector(OutVec);
	return error;
}

int UAstroLibBPLibrary::ConvertToEquatorial(FAstroDouble JulianDateTT, ECelestialCoordSystems CoordSystem, FAstroDoubleVector InEclipticalVector, FAstroDoubleVector & OutEquatorialVector)
{

	double PosVec[3];
	InEclipticalVector.GetDoubleVector(PosVec);

	double OutVec[3];
	int error = ecl2equ_vec(JulianDateTT, short(CoordSystem), accuracy, PosVec, OutVec);

	OutEquatorialVector.SetDoubleVector(OutVec);
	return error;
}

void UAstroLibBPLibrary::SphericalCoordsToVector(FAstroDouble RightAscension, FAstroDouble Declination, FAstroDouble Distance, FAstroDoubleVector & OutEquatorialVector)
{
	double OutVec[3];
	radec2vector(RightAscension, Declination, Distance, OutVec);
	OutEquatorialVector.SetDoubleVector(OutVec);
	
}






int UAstroLibBPLibrary::GetNumberOfLeapSeconds(FDateTime UTCTime)
{
	
	if (UTCTime > FDateTime(2016, 12, 31, 23, 59, 59))
	{
		return 37;
	}
	else if (UTCTime > FDateTime(2015, 6, 30, 23, 59, 59))
	{
		return 36;
	}
	else if (UTCTime > FDateTime(2012, 6, 30, 23, 59, 59))
	{
		return 35;
	}
	else if (UTCTime > FDateTime(2008, 12, 31, 23, 59, 59))
	{
		return 34;
	}
	else if (UTCTime > FDateTime(2005, 12, 31, 23, 59, 59))
	{
		return 33;
	}
	else if (UTCTime > FDateTime(1998, 12, 31, 23, 59, 59))
	{
		return 32;
	}
	else if (UTCTime > FDateTime(1997, 6, 30, 23, 59, 59))
	{
		return 31;
	}
	else if (UTCTime > FDateTime(1995, 12, 31, 23, 59, 59))
	{
		return 30;
	}
	else if (UTCTime > FDateTime(1994, 6, 30, 23, 59, 59))
	{
		return 29;
	}
	else if (UTCTime > FDateTime(1993, 6, 30, 23, 59, 59))
	{
		return 28;
	}
	else if (UTCTime > FDateTime(1992, 6, 30, 23, 59, 59))
	{
		return 27;
	}
	else if (UTCTime > FDateTime(1990, 12, 31, 23, 59, 59))
	{
		return 26;
	}
	else if (UTCTime > FDateTime(1989, 12, 31, 23, 59, 59))
	{
		return 25;
	}
	else if (UTCTime > FDateTime(1987, 12, 31, 23, 59, 59))
	{
		return 24;
	}
	else if (UTCTime > FDateTime(1985, 6, 30, 23, 59, 59))
	{
		return 23;
	}
	else if (UTCTime > FDateTime(1983, 6, 30, 23, 59, 59))
	{
		return 22;
	}
	else if (UTCTime > FDateTime(1982, 6, 30, 23, 59, 59))
	{
		return 21;
	}
	else if (UTCTime > FDateTime(1981, 6, 30, 23, 59, 59))
	{
		return 20;
	}
	else if (UTCTime > FDateTime(1979, 12, 31, 23, 59, 59))
	{
		return 19;
	}
	else if (UTCTime > FDateTime(1978, 12, 31, 23, 59, 59))
	{
		return 18;
	}
	else if (UTCTime > FDateTime(1977, 12, 31, 23, 59, 59))
	{
		return 17;
	}
	else if (UTCTime > FDateTime(1976, 12, 31, 23, 59, 59))
	{
		return 16;
	}
	else if (UTCTime > FDateTime(1975, 12, 31, 23, 59, 59))
	{
		return 15;
	}
	else if (UTCTime > FDateTime(1974, 12, 31, 23, 59, 59))
	{
		return 14;
	}
	else if (UTCTime > FDateTime(1973, 12, 31, 23, 59, 59))
	{
		return 13;
	}
	else if (UTCTime > FDateTime(1972, 12, 31, 23, 59, 59))
	{
		return 12;
	}
	else if (UTCTime > FDateTime(1972, 6, 30, 23, 59, 59))
	{
		return 11;
	}
	else
	{
		return 10;
	}

	return 0;
	

}


int UAstroLibBPLibrary::GetNovasIntFromSolarSystemBodys(ESolarSystemBodys InBody)
{
	/*
	number = object number
	For 'type' = 0: Mercury = 1, ..., Pluto = 9,
	Sun = 10, Moon = 11
	*/
	switch (InBody)
	{
	case ESolarSystemBodys::Mercury:
		return 1;
		break;
	case ESolarSystemBodys::Venus:
		return 2;
		break;
	case ESolarSystemBodys::Earth:
		return 3;
		break;
	case ESolarSystemBodys::Mars:
		return 4;
		break;
	case ESolarSystemBodys::Jupiter:
		return 5;
		break;
	case ESolarSystemBodys::Saturn:
		return 6;
		break;
	case ESolarSystemBodys::Uranus:
		return 7;
		break;
	case ESolarSystemBodys::Neptune:
		return 8;
		break;
	case ESolarSystemBodys::Pluto:
		return 9;
		break;
	case ESolarSystemBodys::Sun:
		return 10;
		break;
	case ESolarSystemBodys::Moon:
		return 11;
		break;
	}

	return -1;
}


EBodys UAstroLibBPLibrary::GetBodyFromNovasInt(int InInt)
{
	switch (InInt)
	{
	case 1:
		return EBodys::Mercury;
		break;
	case 2:
		return EBodys::Venus;
		break;
	case 3:
		return EBodys::Earth;
		break;
	case 4:
		return EBodys::Mars;
		break;
	case 5:
		return EBodys::Jupiter;
		break;
	case 6:
		return EBodys::Saturn;
		break;
	case 7:
		return EBodys::Uranus;
		break;
	case 8:
		return EBodys::Neptune;
		break;
	case 9:
		return EBodys::Pluto;
		break;
	case 10:
		return EBodys::Sun;
		break;
	case 11:
		return EBodys::Moon;
		break;
	}

	return EBodys::Unset;
}

float UAstroLibBPLibrary::GetPlanetToEarthRadiusRatio(const ESolarSystemBodys InBody)
{
	switch (InBody)
	{
	case  ESolarSystemBodys::Sun:
		return 109.2;
		break;
	case  ESolarSystemBodys::Mercury:
		return 0.383;
		break;
	case  ESolarSystemBodys::Venus:
		return 0.949;
		break;
	case  ESolarSystemBodys::Earth:
		return 1;
		break;
	case  ESolarSystemBodys::Moon:
		return 0.2724;
		break;
	case  ESolarSystemBodys::Mars:
		return 0.532;
		break;
	case  ESolarSystemBodys::Jupiter:
		return 11.21;
		break;
	case  ESolarSystemBodys::Saturn:
		return 9.45;
		break;
	case  ESolarSystemBodys::Uranus:
		return 4.01;
		break;
	case  ESolarSystemBodys::Neptune:
		return 3.88;
		break;
	case  ESolarSystemBodys::Pluto:
		return 0.186;
		break;
	}

	return 0.f;
}

FRotator UAstroLibBPLibrary::GetEarthRotator(const FDateTime UTCTime, EPlane Plane)
{

	double GCRSForward[3];
	double GCRSRight[3];
	double GCRSUp[3];
	
	GCRSForward[0] = 1;
	GCRSForward[1] = 0;
	GCRSForward[2] = 0;

	GCRSRight[0] = 0;
	GCRSRight[1] = 1;
	GCRSRight[2] = 0;

	GCRSUp[0] = 0;
	GCRSUp[1] = 0;
	GCRSUp[2] = 1;

	double jd_ut1 = GetUT1JulianDatefromUTC(UTCTime);

	double delta_t = float(Time::getDeltaTByEspenakMeeus(UTCTime));
	double OutCelVec[3];

	FAstroDoubleVector TerrestrialXAxisInCelestialSpace;
	FAstroDoubleVector TerrestrialYAxisInCelestialSpace;
	FAstroDoubleVector TerrestrialZAxisInCelestialSpace;

	cel2ter(jd_ut1, 0.0, delta_t, 1, 0, 0, 0, 0, GCRSForward, OutCelVec);
	TerrestrialXAxisInCelestialSpace.SetDoubleVector(OutCelVec);

	cel2ter(jd_ut1, 0.0, delta_t, 1, 0, 0, 0, 0, GCRSRight, OutCelVec);
	TerrestrialYAxisInCelestialSpace.SetDoubleVector(OutCelVec);

	cel2ter (jd_ut1, 0.0, delta_t, 1, 0, 0, 0, 0, GCRSUp, OutCelVec);
	TerrestrialZAxisInCelestialSpace.SetDoubleVector(OutCelVec);


	if (Plane == EPlane::Ecliptical)
	{
		ConvertToEquatorial(jd_ut1, ECelestialCoordSystems::GCRS, TerrestrialXAxisInCelestialSpace, TerrestrialXAxisInCelestialSpace);
		ConvertToEquatorial(jd_ut1, ECelestialCoordSystems::GCRS, TerrestrialYAxisInCelestialSpace, TerrestrialYAxisInCelestialSpace);
		ConvertToEquatorial(jd_ut1, ECelestialCoordSystems::GCRS, TerrestrialZAxisInCelestialSpace, TerrestrialZAxisInCelestialSpace);
	}

	FMatrix RotationMatrix(TerrestrialXAxisInCelestialSpace.GetFVector().GetUnsafeNormal(), TerrestrialYAxisInCelestialSpace.GetFVector().GetUnsafeNormal(), TerrestrialZAxisInCelestialSpace.GetFVector().GetUnsafeNormal(), FVector::ZeroVector);


	FRotator Rot = RotationMatrix.Rotator();
	return Rot;
}

FAstroDoubleVector UAstroLibBPLibrary::GetKMFromAU(FAstroDoubleVector InAuDoubleVector)
{
	FAstroDoubleVector OutVec;
	OutVec.SetDoubleVector(InAuDoubleVector.x* AUToKm, InAuDoubleVector.y* AUToKm, InAuDoubleVector.z* AUToKm);
	return OutVec;
}

void UAstroLibBPLibrary::GetAzElInDegrees(FVector WorldDirection, float & OutAzimuth, float & OutElevation, float NorthOffset)
{
	//Azimuth
	FVector NorthDirection = FVector(1, 0, 0);
	NorthDirection = NorthDirection.RotateAngleAxis(NorthOffset, FVector(0, 0, 1));


	//FVector ZPlaneVector = WorldDirection;
	FVector ZPlaneVector = FVector::VectorPlaneProject(WorldDirection, FVector(0, 0, 1));

	OutAzimuth = FMath::Acos(FVector::DotProduct(ZPlaneVector.GetUnsafeNormal() , NorthDirection));
	OutAzimuth = FMath::RadiansToDegrees(OutAzimuth);

	FVector CrossProdDirection = FVector::CrossProduct(ZPlaneVector, NorthDirection);

	if (CrossProdDirection.Z > 0)
	{
		OutAzimuth = 360 - OutAzimuth;
	}

	//Elevation
	OutElevation = FMath::Acos(FVector::DotProduct(WorldDirection.GetSafeNormal(), FVector(0,0,1)));
	OutElevation = 90 - FMath::RadiansToDegrees(OutElevation);

}

void UAstroLibBPLibrary::CoordsToPosition(FEarthSurfaceLocation EarthSurfaceLocation, FAstroDouble SiderealTime, FAstroDoubleVector& OutPosition, FAstroDoubleVector& OutVelocity)
{
	on_surface tmpOnSurface = EarthSurfaceLocation.GetNovasStruct();

	double PosVec[3];
	double VelVec[3];

	terra(&tmpOnSurface, SiderealTime, PosVec, VelVec);

	OutPosition.SetDoubleVector(PosVec);
	OutVelocity.SetDoubleVector(VelVec);



}

void UAstroLibBPLibrary::GeoPosAndVel(const FDateTime InUTCDate, FObserver Observer, FAstroDoubleVector& OutPosition, FAstroDoubleVector& OutVelocity)
{
	observer tmpObserver = Observer.GetNovasStruct();
	double jd_tt = GetTTJulianDatefromUTC(InUTCDate);

//	double DeltaT = Time::getDeltaTByEspenakMeeus(UTCTime);
	double PosVec[3];
	double VelVec[3];

	double DeltaT = Time::getDeltaTByEspenakMeeus(InUTCDate);


	geo_posvel(jd_tt, DeltaT, accuracy, &tmpObserver, PosVec, VelVec);
	OutPosition.SetDoubleVector(PosVec);
	OutVelocity.SetDoubleVector(VelVec);
	
}

void UAstroLibBPLibrary::GetRisingTransitSettingTimes(const ESolarSystemBodys InBody, const FEarthSurfaceLocation EarthSurfaceLocation, const FDateTime InUTCDate, FDateTime& OutRisingTime, FDateTime& OutSettingTime, FDateTime& OutTransitTime, bool& bAboveHorizonAllDay, bool& bBelowHorizonAllDay)
{
	float Longitude = EarthSurfaceLocation.Longitude * -1;
	float Latitude = EarthSurfaceLocation.Latitude;
	float DeltaT = Time::getDeltaTByEspenakMeeus(InUTCDate);
	
	float h0 = 0.f; //Standard Altitude in degrees
		switch (InBody)
		{
		case ESolarSystemBodys::Sun:
			h0 = -0.8333;
			break;
		case ESolarSystemBodys::Moon:
			h0 = 0.125;
			break;
		default:
			h0 = -0.5667;
		}

		//UE_LOG(LogTemp, Warning, TEXT("h0: %f"), h0);


	FObserver Observer;
	Observer.LocationType = EWhereIsObserver::EarthSurface;
	Observer.SurfaceLocation = EarthSurfaceLocation;
	
	FSkyPosition SkyPos1, SkyPos2, SkyPos3;
	
	FDateTime PreviousDay, CurrentDay, NextDay;
	CurrentDay = FDateTime(InUTCDate.GetYear(), InUTCDate.GetMonth(), InUTCDate.GetDay(),0,0,0,0);
	PreviousDay = CurrentDay - FTimespan(1, 0, 0, 0);
	NextDay = CurrentDay + FTimespan(1, 0, 0, 0);

	float SiderealTime;
	SiderealTime = 0;
	CalculateSideRealTime(CurrentDay, ESideRealTime::ApparentSideRealTime, SiderealTime);
	SiderealTime = SiderealTime * 15;
	//UE_LOG(LogTemp, Warning, TEXT("SiderealTime: %f"), SiderealTime);

	CalculatePlanetPosition(InBody, Observer, PreviousDay, ECoordSystems::TrueEquatorAndEquinoxOfDate, SkyPos1);
	float Ra1 = SkyPos1.SphericalCoords.RightAscension * 15.f;
	float Dec1 = SkyPos1.SphericalCoords.Declination;
	//UE_LOG(LogTemp, Warning, TEXT("Ra1: %f"), Ra1);
	//UE_LOG(LogTemp, Warning, TEXT("Dec1: %f"), Dec1);

	CalculatePlanetPosition(InBody, Observer, CurrentDay, ECoordSystems::TrueEquatorAndEquinoxOfDate, SkyPos2);
	float Ra2 = SkyPos2.SphericalCoords.RightAscension * 15.f;
	float Dec2 = SkyPos2.SphericalCoords.Declination;
	//UE_LOG(LogTemp, Warning, TEXT("Ra2: %f"), Ra2);
	//UE_LOG(LogTemp, Warning, TEXT("Dec2: %f"), Dec2);

	CalculatePlanetPosition(InBody, Observer, NextDay, ECoordSystems::TrueEquatorAndEquinoxOfDate, SkyPos3);
	float Ra3 = SkyPos3.SphericalCoords.RightAscension * 15.f;
	float Dec3 = SkyPos3.SphericalCoords.Declination;
	//UE_LOG(LogTemp, Warning, TEXT("Ra3: %f"), Ra3);
	//UE_LOG(LogTemp, Warning, TEXT("Dec3: %f"), Dec3);
	
	float Degh0 = FMath::DegreesToRadians(h0);
	float DegLat = FMath::DegreesToRadians(Latitude);
	float DegLong = FMath::DegreesToRadians(Longitude);
	float DegDev2 = FMath::DegreesToRadians(Dec2);

	float H0 = FMath::Sin(Degh0) - (FMath::Sin(DegLat) * FMath::Sin(DegDev2));
	H0 = H0 / (FMath::Cos(DegLat) * FMath::Cos(DegDev2));
	//UE_LOG(LogTemp, Warning, TEXT("cos(H0): %f"), H0);
	
	//Check if object is under horizon at some point in the day
	bAboveHorizonAllDay = false;
	bBelowHorizonAllDay = false;

	if (H0 <= -1.f)
	{
		bAboveHorizonAllDay = true;
		return;
	}
	else if (H0 >= 1.f)
	{
		bBelowHorizonAllDay = true;
		return;
	}
	
	H0 = FMath::RadiansToDegrees(FMath::Acos(H0));
	//UE_LOG(LogTemp, Warning, TEXT("H0: %f"), H0);

	float m0 = (Ra2 + Longitude - SiderealTime) / 360.f;
	m0 = (m0 > 1.f) ? (m0 - 1.f) : ((m0 < 0.f) ? m0 + 1.f : m0);
	//UE_LOG(LogTemp, Warning, TEXT("m0: %f"), m0);

	float m1 = m0 - ((H0) / 360.f);
	m1 = (m1 > 1.f) ? (m1 - 1.f) : ((m1 < 0.f) ? m1 + 1.f : m1);
	//UE_LOG(LogTemp, Warning, TEXT("m1: %f"), m1);

	float m2 = m0 + ((H0) / 360.f);
	m2 = (m2 > 1.f) ? (m2 - 1.f) : ((m2 < 0.f) ? m0 + 1.f : m2);
	//UE_LOG(LogTemp, Warning, TEXT("m2: %f"), m2);


	float SiderealM0 = SiderealTime + 360.985647 * m0;
	float SiderealM1 = SiderealTime + 360.985647 * m1;
	float SiderealM2 = SiderealTime + 360.985647 * m2;
	SiderealM0 = (SiderealM0 > 360.f) ? (SiderealM0 - 360.f) : SiderealM0;
	SiderealM1 = (SiderealM1 > 360.f) ? (SiderealM1 - 360.f) : SiderealM1;
	SiderealM2 = (SiderealM2 > 360.f) ? (SiderealM2 - 360.f) : SiderealM2;
	//UE_LOG(LogTemp, Warning, TEXT("SiderealM0: %f"), SiderealM0);
	//UE_LOG(LogTemp, Warning, TEXT("SiderealM1: %f"), SiderealM1);
	//UE_LOG(LogTemp, Warning, TEXT("SiderealM2: %f"), SiderealM2);

	float M0n = m0 + (DeltaT / 86400.f);
	float M1n = m1 + (DeltaT / 86400.f);
	float M2n = m2 + (DeltaT / 86400.f);
	//UE_LOG(LogTemp, Warning, TEXT("DeltaT: %f"), DeltaT);
	//UE_LOG(LogTemp, Warning, TEXT("M0n: %f"), M0n);
	//UE_LOG(LogTemp, Warning, TEXT("M1n: %f"), M1n);
	//UE_LOG(LogTemp, Warning, TEXT("M2n: %f"), M2n);

	float InterpolateRAM0 = AstroInterpolate(Ra1, Ra2, Ra3, M0n);
	float InterpolateRAM1 = AstroInterpolate(Ra1, Ra2, Ra3, M1n);
	float InterpolateRAM2 = AstroInterpolate(Ra1, Ra2, Ra3, M2n);
	//UE_LOG(LogTemp, Warning, TEXT("InterpolateRAM0: %f"), InterpolateRAM0);
	//UE_LOG(LogTemp, Warning, TEXT("InterpolateRAM1: %f"), InterpolateRAM1);
	//UE_LOG(LogTemp, Warning, TEXT("InterpolateRAM2: %f"), InterpolateRAM2);

	float InterpolateDecM1 = AstroInterpolate(Dec1, Dec2, Dec3, M1n);
	float InterpolateDecM2 = AstroInterpolate(Dec1, Dec2, Dec3, M2n);
	//UE_LOG(LogTemp, Warning, TEXT("InterpolateDecM1: %f"), InterpolateDecM1);
	//UE_LOG(LogTemp, Warning, TEXT("InterpolateDecM2: %f"), InterpolateDecM2);

	float LocalHourAngleM0 = SiderealM0 - Longitude - InterpolateRAM0;
	float LocalHourAngleM1 = SiderealM1 - Longitude - InterpolateRAM1;
	float LocalHourAngleM2 = SiderealM2 - Longitude - InterpolateRAM2;
	//UE_LOG(LogTemp, Warning, TEXT("LocalHourAngleM0: %f"), LocalHourAngleM0);
	//UE_LOG(LogTemp, Warning, TEXT("LocalHourAngleM1: %f"), LocalHourAngleM1);
	//UE_LOG(LogTemp, Warning, TEXT("LocalHourAngleM2: %f"), LocalHourAngleM2);

	float hAltM1 = FMath::Sin(DegLat) * FMath::Sin(FMath::DegreesToRadians(InterpolateDecM1)) + FMath::Cos(DegLat) * FMath::Cos(FMath::DegreesToRadians(InterpolateDecM1)) * FMath::Cos(FMath::DegreesToRadians(LocalHourAngleM1));
	float hAltM2 = FMath::Sin(DegLat) * FMath::Sin(FMath::DegreesToRadians(InterpolateDecM2)) + FMath::Cos(DegLat) * FMath::Cos(FMath::DegreesToRadians(InterpolateDecM2)) * FMath::Cos(FMath::DegreesToRadians(LocalHourAngleM2));
	hAltM1 = FMath::RadiansToDegrees(FMath::Asin(hAltM1));
	hAltM2 = FMath::RadiansToDegrees(FMath::Asin(hAltM2));
	//UE_LOG(LogTemp, Warning, TEXT("hAltM1: %f"), hAltM1);
	//UE_LOG(LogTemp, Warning, TEXT("hAltM2: %f"), hAltM2);

	float deltaM0 = -1 * (LocalHourAngleM0 / 360.f);
	float deltaM1 = (hAltM1 - h0) / (360 * FMath::Cos(FMath::DegreesToRadians(InterpolateDecM1)) * FMath::Cos(DegLat) * FMath::Sin(FMath::DegreesToRadians(LocalHourAngleM1)));
	float deltaM2 = (hAltM2 - h0) / (360 * FMath::Cos(FMath::DegreesToRadians(InterpolateDecM2)) * FMath::Cos(DegLat) * FMath::Sin(FMath::DegreesToRadians(LocalHourAngleM2)));
	//UE_LOG(LogTemp, Warning, TEXT("deltaM0: %f"), deltaM0);
	//UE_LOG(LogTemp, Warning, TEXT("deltaM1: %f"), deltaM1);
	//UE_LOG(LogTemp, Warning, TEXT("deltaM2: %f"), deltaM2);

	m0 = m0 + deltaM0;
	m1 = m1 + deltaM1;
	m2 = m2 + deltaM2;

	if (bAboveHorizonAllDay || bBelowHorizonAllDay)
	{
		OutRisingTime = FDateTime::MinValue();
		OutSettingTime = FDateTime::MinValue();
	}
	else
	{
		OutTransitTime = CurrentDay + FTimespan(0, 0, m0 * 86400);
		OutRisingTime = CurrentDay + FTimespan(0, 0, m1 * 86400);
		OutSettingTime = CurrentDay + FTimespan(0, 0, m2 * 86400);
	}
}

float UAstroLibBPLibrary::AstroInterpolate(const float y1, const float y2, const float y3, const float n)
{
	float a = y2 - y1;
	float b = y3 - y2;
	float c = b - a;

	return y2 + ((n / 2) * (a + b + (n * c)));
}

