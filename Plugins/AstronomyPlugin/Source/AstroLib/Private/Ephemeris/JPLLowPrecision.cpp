// Copyright 2018 Ding Technologies, Inc. All Rights Reserved.

#pragma once

#include "Ephemeris/JPLLowPrecision.h"
#include "AstroLibBPLibrary.h"





//https://ssd.jpl.nasa.gov/txt/aprx_pos_planets.pdf
//Low precision formula for planets from JPL
//J2000 Equatorial

UJPLLowPrecision::UJPLLowPrecision()
{

	
}


void UJPLLowPrecision::CalcHelioLocation(EBodys ObjBody, FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location)
{

	double T = ((jd1+ Optionaljd2) - 2451545.l) / 36525.l;

	double a0, e0, I0, L0, Perihelion0, AscendingNode0;
	double at, et, It, Lt, Periheliont, AscendingNodet;

	a0 = 0;
	e0 = 0;
	I0 = 0;
	L0 = 0;
	Perihelion0 = 0;
	AscendingNode0 = 0;

	at = 0;
	et = 0;
	It = 0;
	Lt = 0;
	Periheliont = 0;
	AscendingNodet = 0;


	switch (ObjBody)
	{
	case EBodys::Sun:
	{
		Location.SetDoubleVector(0.l, 0.l, 0.l);
		return;
	}
	case EBodys::Mercury:
	{
		a0 = 0.38709927;
		e0 = 0.20563593;
		I0 = 7.00497902;
		L0 = 252.25032350;
		Perihelion0 = 77.45779628;
		AscendingNode0 = 48.33076593;

		at = 0.00000037;
		et = 0.00001906;
		It = -0.00594749;
		Lt = 149472.67411175;
		Periheliont = 0.16047689;
		AscendingNodet = -0.12534081;

		break;
	}
	case EBodys::Venus:
	{
		a0 = 0.72333566;
		e0 = 0.00677672;
		I0 = 3.39467605;
		L0 = 181.97909950;
		Perihelion0 = 131.60246718;
		AscendingNode0 = 76.67984255;

		at = 0.00000390;
		et = -0.00004107;
		It = -0.00078890;
		Lt = 58517.81538729;
		Periheliont = 0.00268329;
		AscendingNodet = -0.27769418;

		break;
	}
	case EBodys::Earth:
	{
		a0 = 1.00000261;
		e0 = 0.01671123;
		I0 = -0.00001531;
		L0 = 100.46457166;
		Perihelion0 = 102.93768193;
		AscendingNode0 = 0.0;

		at = 0.00000562;
		et = -0.00004392;
		It = -0.01294668;
		Lt = 35999.37244981;
		Periheliont = 0.32327364;
		AscendingNodet = 0.0;
		break;
	}
	case EBodys::Mars:
	{
		a0 = 1.52371034;
		e0 = 0.09339410;
		I0 = 1.84969142;
		L0 = -4.55343205;
		Perihelion0 = -23.94362959;
		AscendingNode0 = 49.55953891;

		at = 0.00001847;
		et = 0.00007882;
		It = -0.00813131;
		Lt = 19140.30268499;
		Periheliont = 0.44441088;
		AscendingNodet = -0.29257343;
		break;
	}
	case EBodys::Jupiter:
	{
		a0 = 5.20288700;
		e0 = 0.04838624;
		I0 = 1.30439695;
		L0 = 34.39644051;
		Perihelion0 = 14.72847983;
		AscendingNode0 = 100.47390909;

		at = -0.00011607;
		et = -0.00013253;
		It = -0.00183714;
		Lt = 3034.74612775;
		Periheliont = 0.21252668;
		AscendingNodet = 0.20469106;
		break;
	}
	case EBodys::Saturn:
	{
		a0 = 9.53667594;
		e0 = 0.05386179;
		I0 = 2.48599187;
		L0 = 49.95424423;
		Perihelion0 = 92.59887831;
		AscendingNode0 = 113.66242448;

		at = -0.00125060;
		et = -0.00050991;
		It = 0.00193609;
		Lt = 1222.49362201;
		Periheliont = -0.41897216;
		AscendingNodet = -0.28867794;
		break;
	}
	case EBodys::Uranus:
	{
		a0 = 19.18916464;
		e0 = 0.04725744;
		I0 = 0.77263783;
		L0 = 313.23810451;
		Perihelion0 = 170.95427630;
		AscendingNode0 = 74.01692503;

		at = -0.00196176;
		et = -0.00004397;
		It = -0.00242939;
		Lt = 428.48202785;
		Periheliont = 0.40805281;
		AscendingNodet = 0.04240589;
		break;
	}
	case EBodys::Neptune:
	{
		a0 = 30.06992276;
		e0 = 0.00859048;
		I0 = 1.77004347;
		L0 = -55.12002969;
		Perihelion0 = 44.96476227;
		AscendingNode0 = 131.78422574;

		at = 0.00026291;
		et = 0.00005105;
		It = 0.00035372;
		Lt = 218.45945325;
		Periheliont = -0.32241464;
		AscendingNodet = -0.00508664;
		break;
	}
	case EBodys::Pluto:
	{
		a0 = 39.48211675;
		e0 = 0.24882730;
		I0 = 17.14001206;
		L0 = 238.92903833;
		Perihelion0 = 224.06891629;
		AscendingNode0 = 110.30393684;

		at = -0.00031596;
		et = 0.00005170;
		It = 0.00004818;
		Lt = 145.20780515;
		Periheliont = -0.04062942;
		AscendingNodet = -0.01183482;
		break;
	}
	case EBodys::Moon:
	{
	
		FAstroDoubleVector MoonGeoEquatorialLoc;
		MoonGeocentricEquatorialPosition(jd1, Optionaljd2, MoonGeoEquatorialLoc);

		FAstroDoubleVector EarthLocation;
		CalcHelioLocation(EBodys::Earth, jd1, Optionaljd2, EarthLocation);

		FAstroDoubleVector MoonEclipticalLocation;
		UAstroLibBPLibrary::ConvertToEcliptical(jd1, ECelestialCoordSystems::TrueEquatorAndEquinoxOfDate, MoonGeoEquatorialLoc, MoonEclipticalLocation);

	
		Location = MoonEclipticalLocation + EarthLocation;


		return;
		break;
	}
	default:
	{
		FString EnumStrBody = GetEnumStr("EBodys", ObjBody);
		UE_LOG(LogTemp, Warning, TEXT("JPL Low Precision does not contain data for %s"), *EnumStrBody);
		Location.SetDoubleVector(0.l,0.l,0.l);
		return;
	}

	}

	double a, e, I, L, LongitudeOfPerihelion, LongitudeOfAscendingNode;

	a = a0 + at*T;
	e = e0 + et*T;
	I = I0 + It*T;
	L = L0 + Lt*T;
	LongitudeOfPerihelion = Perihelion0 + Periheliont*T;
	LongitudeOfAscendingNode = AscendingNode0 + AscendingNodet*T;

	double ArgumentOfPerihelion = LongitudeOfPerihelion - LongitudeOfAscendingNode;
	
	double MeanAnomaly = L - LongitudeOfPerihelion;

	MeanAnomaly = fmodl(MeanAnomaly, 360.l);
	if (MeanAnomaly > 180.l)
	{
		MeanAnomaly -= 360;
	}


	double deltaM, deltaE;
	double eStar = 180 / PI * e;

	double EccentricAnomaly = MeanAnomaly + eStar*sin(D2R(MeanAnomaly));

	do
	{
		deltaM = MeanAnomaly - (EccentricAnomaly - eStar*sin(D2R(EccentricAnomaly)));
		deltaE = deltaM / (1 - e * cos(D2R(EccentricAnomaly)));
		EccentricAnomaly += deltaE;
	} while (abs(deltaE) > 0.00001);



	double xPrime = a * (cos(D2R(EccentricAnomaly)) - e);
	double yPrime = a * (sqrt(1 - (e*e))) * sin(D2R(EccentricAnomaly));
	double zPrime = 0;

	ArgumentOfPerihelion = D2R(ArgumentOfPerihelion);
	I = D2R(I);
	LongitudeOfAscendingNode = D2R(LongitudeOfAscendingNode);

	double CosAoP = cos(ArgumentOfPerihelion);
	double SinAoP = sin(ArgumentOfPerihelion);
	double CosLAN = cos(LongitudeOfAscendingNode);
	double SinLAN = sin(LongitudeOfAscendingNode);
	double cosI = cos(I);

	double xVal1 =  CosAoP* CosLAN;
	double xVal2 = -SinAoP* SinLAN*cosI;
	double xVal3 = -SinAoP*CosLAN;
	double xVal4 = -CosAoP* SinLAN*cosI;
	double xVal = ((xVal1 + xVal2) * xPrime) + ((xVal3 + xVal4) * yPrime);

	double yVal1 = CosAoP* SinLAN;
	double yVal2 = SinAoP* CosLAN*cosI;
	double yVal3 = -SinAoP*SinLAN;
	double yVal4 = CosAoP* CosLAN*cosI;
	double yVal = ((yVal1 + yVal2) * xPrime) + ((yVal3 + yVal4) * yPrime);

	double zVal = (SinAoP* sin(I)* xPrime) + ((CosAoP*sin(I)) * yPrime);

	Location.SetDoubleVector(xVal, yVal, zVal);

}

int UJPLLowPrecision::MoonGeocentricEquatorialPosition(FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location)
{
	double Astropi = acos(-1.0L);
	double T = ((jd1 + Optionaljd2) - 2451545.l) / 36525.l;

	double Lambda = 218.32l + 481267.881l*T
		+ 6.29l*sin(D2R(135.0l + 477198.87l*T))
		- 1.27l*sin(D2R(259.3l - 413335.36l*T))
		+ 0.66l*sin(D2R(235.7l + 890534.22l*T))
		+ 0.21l*sin(D2R(269.9l + 954397.74l*T))
		- 0.19l*sin(D2R(357.5l + 35999.05l*T))
		- 0.11l*sin(D2R(186.5l + 966404.03l*T));

	double Bravo = 5.13l*sin(D2R(93.3l + 483202.02l*T))
		+ 0.28*sin(D2R(228.2l + 960400.89l*T))
		- 0.28*sin(D2R(318.3l + 6003.15l*T))
		- 0.17*sin(D2R(217.6l - 407332.21l*T));


	double Pizza = 0.9508l
		+ 0.0518l*cos(D2R(135.0l + 477198.87l*T))
		+ 0.0095l*cos(D2R(259.3l - 413335.36l*T))
		+ 0.0078l*cos(D2R(235.7l + 890534.22l*T))
		+ 0.0028l*cos(D2R(269.9l + 954397.74l*T));

	double r = (1.l / sin(D2R(Pizza)))* 0.0000425875045560;

	double l = cos(D2R(Bravo))*cos(D2R(Lambda));
	double m = 0.9175l*cos(D2R(Bravo))*sin(D2R(Lambda)) - 0.3978*sin(D2R(Bravo));
	double n = 0.3978l*cos(D2R(Bravo))*sin(D2R(Lambda)) + 0.9175*sin(D2R(Bravo));


	double x = l*r;
	double y = m*r;
	double z = n*r;

	Location.SetDoubleVector(x, y, z);

	return 0;
}

int UJPLLowPrecision::GetObjectState(EBodys ObjBody, EBodys ObjOrigin, FAstroDouble jd1, FAstroDouble Optionaljd2, FAstroDoubleVector & Location, FAstroDoubleVector & Velocity, EPlane CoordinatePlane, const EEphemerisUnits OutUnit)
{
	if (ObjOrigin == EBodys::Earth && ObjBody == EBodys::Moon)
	{
		MoonGeocentricEquatorialPosition(jd1, Optionaljd2, Location);

		FAstroDoubleVector NewLoc;
		MoonGeocentricEquatorialPosition(jd1 + 1, Optionaljd2, NewLoc);

		Velocity.SetDoubleVector(NewLoc.x - Location.x, NewLoc.y - Location.y, NewLoc.z - Location.z);

		if (CoordinatePlane == EPlane::Ecliptical)
		{
			FAstroDoubleVector NewEclipticalVectorLocation;
			FAstroDoubleVector NewEclipticalVectorVelocity;
			UAstroLibBPLibrary::ConvertToEcliptical(jd1, ECelestialCoordSystems::TrueEquatorAndEquinoxOfDate, Location, NewEclipticalVectorLocation);
			UAstroLibBPLibrary::ConvertToEcliptical(jd1, ECelestialCoordSystems::TrueEquatorAndEquinoxOfDate, Velocity, NewEclipticalVectorVelocity);
			Location = NewEclipticalVectorLocation;
			Velocity = NewEclipticalVectorVelocity;
		}
	}
	
	else if (ObjOrigin == EBodys::Sun)
	{

		CalcHelioLocation(ObjBody, jd1, Optionaljd2, Location);

		FAstroDoubleVector NewLoc;
		CalcHelioLocation(ObjBody, jd1 + 1, Optionaljd2, NewLoc);

		Velocity.SetDoubleVector(NewLoc.x - Location.x, NewLoc.y - Location.y, NewLoc.z - Location.z);

		if (CoordinatePlane == EPlane::Equatorial)
		{
			FAstroDoubleVector NewEclipticalVectorLocation;
			FAstroDoubleVector NewEclipticalVectorVelocity;
			UAstroLibBPLibrary::ConvertToEquatorial(jd1, ECelestialCoordSystems::TrueEquatorAndEquinoxOfDate, Location, NewEclipticalVectorLocation);
			UAstroLibBPLibrary::ConvertToEquatorial(jd1, ECelestialCoordSystems::TrueEquatorAndEquinoxOfDate, Velocity, NewEclipticalVectorVelocity);
			Location = NewEclipticalVectorLocation;
			Velocity = NewEclipticalVectorVelocity;
		}
	}
	else if (ObjOrigin == EBodys::SolarSystemBarycenter)
	{
		//UE_LOG(LogTemp, Log, TEXT("Warning: Barycenter not available as a central body for JPLLowPrecision. Using Sun Centered instead."));
		GetObjectState(ObjBody, EBodys::Sun,jd1,Optionaljd2,Location, Velocity, CoordinatePlane, OutUnit);

	}
	else
	{
		FAstroDoubleVector LocationCoord, VelocityCoord;
		GetObjectState(ObjBody, EBodys::Sun, jd1, Optionaljd2, LocationCoord, VelocityCoord, CoordinatePlane);
		
		FAstroDoubleVector BodyLocationCoord, BodyVelocityCoord;
		GetObjectState(ObjOrigin, EBodys::Sun, jd1, Optionaljd2, BodyLocationCoord, BodyVelocityCoord, CoordinatePlane);

		Location.x = LocationCoord.x - BodyLocationCoord.x;
		Location.y = LocationCoord.y - BodyLocationCoord.y;
		Location.z = LocationCoord.z - BodyLocationCoord.z;

		Velocity.x = VelocityCoord.x - BodyVelocityCoord.x;
		Velocity.y = VelocityCoord.y - BodyVelocityCoord.y;
		Velocity.z = VelocityCoord.z - BodyVelocityCoord.z;

		

	}

	if (OutUnit == EEphemerisUnits::Kilometers)
	{
		Location.SetDoubleVector(Location.x * AUToKm, Location.y * AUToKm, Location.z * AUToKm);
		Velocity.SetDoubleVector(Velocity.x * AUToKm, Velocity.y * AUToKm, Velocity.z * AUToKm);
	}

	return 0;
}



double UJPLLowPrecision::D2R(double InDeg)
{
	return InDeg * DEG2RAD;
}

double UJPLLowPrecision::R2D(double InRad)
{
	return InRad * 180.l / DoublePI;
}

FAstroDouble UJPLLowPrecision::GetLowPrecisionJulianDate(FDateTime inDate)
{
	return FAstroDouble(inDate.GetJulianDay());
}
